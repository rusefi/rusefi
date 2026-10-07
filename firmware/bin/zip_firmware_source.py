#!/usr/bin/env python3
"""Zip tracked firmware, libfirmware, and wiki Markdown from the working trees.

Requires Python 3 and Git. Includes local edits and staged new files, skips
deleted files, and excludes images, firmware/ext/cmsis-svd/, the cypress and
kinetis board directories, and untracked/ignored build output. Includes the
libfirmware submodule; all other submodules (including nested ones) are excluded.
The libfirmware submodule must be initialized before running this script.
The sibling rusefi_documentation checkout is also required; use
--documentation-dir to select another checkout. Its tracked .md files are
included under rusefi_documentation/, preserving their relative paths.
Keeps other tracked firmware assets (including build scripts and configuration)
and the firmware/ directory layout. Symlinks are stored without following them.

Usage: python3 firmware/bin/zip_firmware_source.py [output.zip]
Default: build/firmware-source.zip at the repository root.
Existing output files are never overwritten.
"""

import argparse
import os
from pathlib import Path
import stat
import subprocess
import zipfile


IMAGE_EXTENSIONS = {
    ".avif", ".bmp", ".gif", ".heic", ".heif", ".ico", ".jfif", ".jpeg",
    ".jpg", ".png", ".psd", ".svg", ".svgz", ".tga", ".tif", ".tiff",
    ".webp", ".xcf",
}

EXCLUDED_DIRECTORIES = (
    "firmware/ext/cmsis-svd/",
    "firmware/config/boards/cypress/",
    "firmware/config/boards/kinetis/",
)


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

    if not files:
        parser.error("no tracked firmware files found")
    if not documentation_count:
        parser.error("no tracked Markdown files found in {}".format(documentation))
    output.parent.mkdir(parents=True, exist_ok=True)
    try:
        archive = zipfile.ZipFile(output, "x", compression=zipfile.ZIP_DEFLATED)
    except FileExistsError:
        parser.error("output already exists: {} (choose another path)".format(output))
    try:
        with archive:
            for name, source in files:
                if source.is_symlink():
                    info = zipfile.ZipInfo(name)
                    info.create_system = 3
                    info.external_attr = (stat.S_IFLNK | 0o777) << 16
                    archive.writestr(info, os.fsencode(os.readlink(source)))
                else:
                    archive.write(source, name)
    except BaseException:
        output.unlink()
        raise
    print("Created {} ({} files, including {} wiki Markdown files, {:.1f} MiB)".format(
        output, len(files), documentation_count, output.stat().st_size / (1024 * 1024)))


if __name__ == "__main__":
    main()
