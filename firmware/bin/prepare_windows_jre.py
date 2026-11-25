#!/usr/bin/env python3
"""Download or stage the pinned Windows JRE into a fresh output directory."""

import argparse
import hashlib
import http.client
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import socket
import stat
import sys
import tempfile
import time
import urllib.error
import urllib.request
import zipfile


DEFAULT_MANIFEST = Path(__file__).with_name("windows_jre.json")
REQUIRED_FILES = ("bin/java.exe", "bin/javaw.exe", "bin/server/jvm.dll", "release")
MAX_DOWNLOAD_ATTEMPTS = 4
RETRYABLE_HTTP_CODES = {408, 429, 500, 502, 503, 504}
NETWORK_ERRORS = (
    TimeoutError, ConnectionError, socket.gaierror, socket.herror,
    http.client.IncompleteRead,
)


def fail(message):
    raise ValueError(message)


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_manifest(path):
    data = json.loads(path.read_text(encoding="utf-8"))
    for key in ("version", "url", "sha256", "archive", "runtime_dir"):
        if not isinstance(data.get(key), str) or not data[key]:
            fail(f"manifest requires non-empty string {key}")
    if not re.fullmatch(r"[0-9a-f]{64}", data["sha256"]):
        fail("manifest sha256 must be 64 lowercase hex characters")
    if Path(data["archive"]).name != data["archive"] or any(char in data["archive"] for char in "/\\:"):
        fail("manifest archive must be a filename")
    if any(char in data["runtime_dir"] for char in "/\\:") or data["runtime_dir"] in (".", ".."):
        fail("manifest runtime_dir must be a single safe directory name")
    return data


def retryable_download_error(error):
    if isinstance(error, urllib.error.HTTPError):
        return error.code in RETRYABLE_HTTP_CODES
    if isinstance(error, urllib.error.URLError):
        error = error.reason
    return isinstance(error, NETWORK_ERRORS)


def download_retry_delay(error, attempt):
    delay = 2 ** attempt
    if isinstance(error, urllib.error.HTTPError):
        try:
            delay = max(delay, float(error.headers.get("Retry-After", delay)))
        except (AttributeError, TypeError, ValueError):
            pass
    return min(delay, 30)


def get_archive(manifest, cache_dir, offline_archive):
    if offline_archive is not None:
        archive = offline_archive
        if not archive.is_file():
            fail(f"archive does not exist: {archive}")
    else:
        cache_dir.mkdir(parents=True, exist_ok=True)
        archive = cache_dir / manifest["archive"]
        if archive.exists() and sha256(archive) != manifest["sha256"]:
            archive.unlink()
        if not archive.exists():
            fd, name = tempfile.mkstemp(prefix=archive.name + ".", suffix=".part", dir=cache_dir)
            os.close(fd)
            partial = Path(name)
            try:
                for attempt in range(MAX_DOWNLOAD_ATTEMPTS):
                    try:
                        request = urllib.request.Request(
                            manifest["url"], headers={"User-Agent": "rusEFI-JRE-preparer"},
                        )
                        with urllib.request.urlopen(request, timeout=30) as response:
                            with partial.open("wb") as destination:
                                shutil.copyfileobj(response, destination)
                                content_length = getattr(response, "headers", {}).get("Content-Length")
                                if content_length is not None:
                                    missing = int(content_length) - destination.tell()
                                    if missing > 0:
                                        raise http.client.IncompleteRead(b"", missing)
                        break
                    except (urllib.error.URLError, TimeoutError, ConnectionError,
                            socket.gaierror, socket.herror,
                            http.client.IncompleteRead) as error:
                        retry = retryable_download_error(error)
                        if isinstance(error, urllib.error.HTTPError):
                            error.close()
                        if not retry or attempt + 1 == MAX_DOWNLOAD_ATTEMPTS:
                            raise
                        time.sleep(download_retry_delay(error, attempt))
                if sha256(partial) != manifest["sha256"]:
                    fail("downloaded archive SHA-256 mismatch")
                os.replace(partial, archive)
            finally:
                partial.unlink(missing_ok=True)
    if sha256(archive) != manifest["sha256"]:
        fail("archive SHA-256 mismatch")
    return archive


def inspect_archive(archive, runtime_dir):
    expected_top = runtime_dir + "/"
    with zipfile.ZipFile(archive) as zipped:
        files = {}
        for info in zipped.infolist():
            name = info.filename
            if "\\" in name or ":" in name:
                fail(f"unsafe ZIP path: {name!r}")
            path = PurePosixPath(name)
            if path.is_absolute() or any(part in ("", ".", "..") for part in path.parts):
                fail(f"unsafe ZIP path: {name!r}")
            if not name.startswith(expected_top):
                fail(f"unexpected ZIP top-level entry: {name!r}")
            mode = info.external_attr >> 16
            file_type = stat.S_IFMT(mode)
            if file_type == stat.S_IFLNK:
                fail(f"symlink in ZIP: {name!r}")
            if file_type not in (0, stat.S_IFREG, stat.S_IFDIR):
                fail(f"unsupported ZIP entry type: {name!r}")
            if not info.is_dir():
                if name in files:
                    fail(f"duplicate ZIP entry: {name!r}")
                files[name] = info
        prefix = expected_top
        for required in REQUIRED_FILES:
            if prefix + required not in files:
                fail(f"JRE archive missing {required}")
        if not any(name.startswith(prefix + "legal/") and not item.is_dir() for name, item in files.items()):
            fail("JRE archive has no legal notices under legal/")
        release = zipped.read(files[prefix + "release"]).decode("utf-8").replace("\r\n", "\n")
        if not re.search(r'^JAVA_VERSION="17(?:\.[^"\r\n]*)?"$', release, re.MULTILINE):
            fail("JRE release metadata does not identify Java 17")
        if not re.search(r'^OS_ARCH="x86_64"$', release, re.MULTILINE):
            fail("JRE release metadata does not identify x86_64")
        if not re.search(r'^OS_NAME="Windows"$', release, re.MULTILINE):
            fail("JRE release metadata does not identify Windows")
        return files


def prepare(output, cache_dir, offline_archive, manifest_path):
    manifest = read_manifest(manifest_path)
    if output.exists():
        fail(f"output already exists: {output}")
    output.parent.mkdir(parents=True, exist_ok=True)
    archive = get_archive(manifest, cache_dir, offline_archive)
    files = inspect_archive(archive, manifest["runtime_dir"])
    staging = Path(tempfile.mkdtemp(prefix=output.name + ".", dir=output.parent))
    try:
        with zipfile.ZipFile(archive) as zipped:
            for name, info in files.items():
                relative = PurePosixPath(name).relative_to(manifest["runtime_dir"])
                destination = staging.joinpath(*relative.parts)
                destination.parent.mkdir(parents=True, exist_ok=True)
                with zipped.open(info) as source, destination.open("xb") as target:
                    shutil.copyfileobj(source, target)
                destination.chmod((info.external_attr >> 16) & 0o777 or 0o644)
        staging.rename(output)
    finally:
        if staging.exists():
            shutil.rmtree(staging)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output_dir", type=Path)
    parser.add_argument("--cache-dir", required=True, type=Path)
    parser.add_argument("--archive", type=Path, help="use an already-downloaded archive")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    args = parser.parse_args()
    try:
        prepare(args.output_dir, args.cache_dir, args.archive, args.manifest)
    except (OSError, ValueError, KeyError, zipfile.BadZipFile,
            json.JSONDecodeError, http.client.IncompleteRead) as error:
        print(f"prepare_windows_jre.py: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
