#!/usr/bin/env python3
"""Zip tracked firmware, libfirmware, AI guides, and wiki Markdown.

Requires Python 3 and Git. Includes local edits and staged new files, skips
deleted files, and excludes images, firmware/ext/cmsis-svd/, the cypress and
kinetis board directories, and untracked/ignored build output. Includes the
libfirmware submodule; all other submodules (including nested ones) are excluded.
The libfirmware submodule must be initialized before running this script.
The sibling rusefi_documentation checkout is also required; use
--documentation-dir to select another checkout. Its tracked .md files are
included under rusefi_documentation/, preserving their relative paths.
Includes root docs/AI/, selected technical guides, licenses, a searchable index,
and a versioned manifest with repository revisions, tracked-edit flags and file hashes.
The README links the included guides. PDF and CHM collections are excluded.
Keeps other tracked firmware assets (including build scripts and configuration)
and the firmware/ directory layout. Symlinks are stored without following them.

Usage: python3 firmware/bin/zip_firmware_source.py [output.zip]
Default: build/firmware-source.zip at the repository root.
Existing output files are never overwritten.
"""

import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import stat
import subprocess
from urllib.parse import quote
import zipfile


IMAGE_EXTENSIONS = {
    ".avif", ".bmp", ".gif", ".heic", ".heif", ".ico", ".jfif", ".jpeg",
    ".jpg", ".png", ".psd", ".svg", ".svgz", ".tga", ".tif", ".tiff",
    ".webp", ".xcf", ".pdf", ".chm",
}

EXCLUDED_DIRECTORIES = (
    "firmware/ext/cmsis-svd/",
    "firmware/config/boards/cypress/",
    "firmware/config/boards/kinetis/",
)


TECHNICAL_DOCS = (
    "docs/hellen-board-mapping.md", "docs/hardware-reinit-and-power-cycle.md",
    "docs/board-configuration-override-hooks.md", "docs/sensor-rate-of-change-filtering.md",
    "docs/calibration-compatibility.md", "docs/adding-new-trigger.md",
    "docs/offchip-adc.md", "docs/h7-adc-mux.md", "docs/ethernet-console.md",
    "docs/firmware-flash-usage.md", "docs/firmware_stack_usage.md",
)
MANIFEST_NAME = "knowledge-manifest.json"


def repository_identity(repository):
    revision = subprocess.check_output(
        ["git", "rev-parse", "HEAD"], cwd=repository, text=True).strip()
    # Tracked working-tree edits matter: this archive deliberately includes them.
    dirty = bool(subprocess.check_output(
        ["git", "status", "--porcelain", "--untracked-files=no"], cwd=repository))
    return {"revision": revision, "dirty": dirty}


def payload_hash(files):
    # Shared with KnowledgeManifest.java. UTF-8 encoding with Java String (UTF-16) path ordering.
    return hashlib.sha256("".join(
        name + "\0" + files[name]["sha256"] + "\n" for name in sorted(files, key=lambda name: name.encode("utf-16-be"))
    ).encode("utf-8")).hexdigest()


def write_archive(output, files, repositories):
    entries = {}
    ai_names = [name for name, _ in files if name.startswith("docs/AI/")]
    index = archive_index(ai_names)
    with zipfile.ZipFile(output, "x", compression=zipfile.ZIP_DEFLATED) as archive:
        def add(name, content, info=None):
            if name in entries or name == MANIFEST_NAME:
                raise ValueError("Duplicate or reserved archive path: " + name)
            archive.writestr(info or name, content)
            entries[name] = {"sha256": hashlib.sha256(content).hexdigest(), "size": len(content)}

        add("README.md", index.encode("utf-8"))
        # Also expose the index through the existing docs/ knowledge allowlist.
        add("docs/knowledge-index.md", index.replace(
            "](firmware/", "](../firmware/").replace(
            "](rusefi_documentation/", "](../rusefi_documentation/").replace(
            "](docs/", "](").encode("utf-8"))
        for name, source in sorted(files):
            if source.is_symlink():
                info = zipfile.ZipInfo(name)
                info.create_system = 3
                info.external_attr = (stat.S_IFLNK | 0o777) << 16
                add(name, os.fsencode(os.readlink(source)), info)
            else:
                add(name, source.read_bytes())
        manifest = {"schema_version": 1, "created_at": datetime.now(timezone.utc).isoformat(),
                    "repositories": repositories, "files": entries, "payload_sha256": payload_hash(entries)}
        archive.writestr(MANIFEST_NAME, json.dumps(manifest, sort_keys=True, indent=2) + "\n")


def archive_index(ai_names):
    lines = [
        "# rusEFI firmware source and documentation",
        "",
        "Tracked working-tree content, including local edits, exported by",
        "`firmware/bin/zip_firmware_source.py`.",
        "",
        "## Contents",
        "",
        "- [Firmware](firmware/): controllers, board configuration, Lua examples, and build scripts.",
        "- [libfirmware](firmware/libfirmware/): reusable firmware library.",
        "- [AI guides](docs/AI/): subsystem explanations and diagnostic notes, indexed below.",
        "- [Wiki Markdown](rusefi_documentation/): setup, wiring, and operating documentation.",
        "",
        "## Provenance and licenses",
        "",
        "- knowledge-manifest.json records firmware, libfirmware and wiki revisions, tracked edits, and payload hashes.",
        "- A matching INI signature is not proof of matching firmware source; no ECU/source match is asserted.",
        "- [rusEFI license](docs/licenses/rusefi.txt) and [wiki license](docs/licenses/wiki.txt).",
        "- Per-file notices, including libfirmware and third-party notices, remain with their sources.",
        "",
        "## Technical references",
        "",
    ]
    for name in TECHNICAL_DOCS:
        lines.append("- [{}]({})".format(name[len("docs/"):], quote(name)))
    lines += [
        "",
        "## AI guide index",
        "",
    ]
    for name in sorted(ai_names):
        label = name[len("docs/AI/"):].replace("[", "\\[").replace("]", "\\]")
        lines.append("- [{}]({})".format(label, quote(name)))
    return "\n".join(lines) + "\n"


def main():
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", nargs="?", type=Path,
                        default=root / "build" / "firmware-source.zip")
    parser.add_argument("--documentation-dir", type=Path,
                        default=root.parent / "rusefi_documentation",
                        help="wiki Git checkout (default: ../rusefi_documentation)")
    args = parser.parse_args()
    output = args.output.absolute()
    if output.suffix.lower() != ".zip":
        parser.error("output must have a .zip extension")

    libfirmware = root / "firmware" / "libfirmware"
    if not (libfirmware / ".git").exists():
        parser.error("libfirmware is not initialized; run from the repository root: "
                     "git submodule update --init firmware/libfirmware")

    documentation = args.documentation_dir.absolute()
    if not (documentation / ".git").exists():
        parser.error("rusefi_documentation checkout not found at {}; clone "
                     "https://github.com/rusefi/rusefi_documentation there or pass "
                     "--documentation-dir PATH".format(documentation))

    # Explicitly include libfirmware, without recursing into other submodules.
    repositories = (
        (root, "firmware/", "", False),
        (root, "docs/AI/", "", False),
        (libfirmware, ".", "firmware/libfirmware/", False),
        (documentation, ".", "rusefi_documentation/", True),
    )
    files = []
    documentation_count = 0
    for repository, pathspec, prefix, markdown_only in repositories:
        entries = subprocess.check_output(
            ["git", "ls-files", "--stage", "-z", "--", pathspec], cwd=repository
        )
        for entry in entries.split(b"\0"):
            if not entry:
                continue
            metadata, name = entry.split(b"\t", 1)
            mode, _, stage = metadata.split()
            if stage != b"0":
                parser.error("resolve merge conflicts in {} before archiving".format(repository))
            # Mode 160000 is a gitlink, not a file.
            if mode == b"160000":
                continue
            relative_name = os.fsdecode(name)
            name = prefix + relative_name
            if name.startswith(EXCLUDED_DIRECTORIES):
                continue
            source = repository / relative_name
            if markdown_only and source.suffix.lower() != ".md":
                continue
            if source.suffix.lower() in IMAGE_EXTENSIONS:
                continue
            if source.is_symlink() or source.is_file():
                files.append((name, source))
                if markdown_only:
                    documentation_count += 1

    tracked = set(os.fsdecode(name) for name in subprocess.check_output(
        ["git", "ls-files", "-z"], cwd=root).split(b"\0") if name)
    for name in TECHNICAL_DOCS:
        if name in tracked and (root / name).is_file():
            files.append((name, root / name))
    for name, source in (("docs/licenses/rusefi.txt", root / "license.txt"),
                         ("docs/licenses/wiki.txt", documentation / "LICENSE")):
        if not source.is_file():
            parser.error("Required license missing: {}".format(source))
        files.append((name, source))
    identities = {"firmware": repository_identity(root), "libfirmware": repository_identity(libfirmware),
                  "wiki": repository_identity(documentation)}

    if not files:
        parser.error("no tracked firmware files found")
    if not documentation_count:
        parser.error("no tracked Markdown files found in {}".format(documentation))
    ai_names = [name for name, _ in files if name.startswith("docs/AI/")]
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        parser.error("output already exists: {} (choose another path)".format(output))
    try:
        write_archive(output, files, identities)
    except FileExistsError:
        parser.error("output already exists: {} (choose another path)".format(output))
    except BaseException:
        output.unlink(missing_ok=True)
        raise
    print("Created {} ({} files, including {} AI files and {} wiki Markdown files, {:.1f} MiB)".format(
        output, len(files) + 3, len(ai_names), documentation_count,
        output.stat().st_size / (1024 * 1024)))


if __name__ == "__main__":
    main()
