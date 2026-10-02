"""Check real bundle.mk artifact names without compiling firmware or Java.

Run with: python3 firmware/bin/test_bundle_naming.py
"""

import os
from pathlib import Path
import subprocess
import tempfile
import unittest


BUNDLE_MK = Path(__file__).resolve().parents[1] / "bundle.mk"


class BundleNamingTest(unittest.TestCase):
    def names(self, custom_image, **metadata):
        with tempfile.TemporaryDirectory() as directory:
            work = Path(directory)
            signature = work / "controllers/generated/signature_test_board.h"
            signature.parent.mkdir(parents=True)
            signature.write_text("#define SIGNATURE_HASH 1234567890\n")
            makefile = work / "Makefile"
            makefile.write_text(f"""
include {BUNDLE_MK}
.PHONY: names
names:
\t@printf '%s\\n' '$(SREC_TARGET)' '$(WHITE_LABEL_BUNDLE_NAME).zip'
""")
            env = dict(os.environ)
            for name in ("MAKEFLAGS", "MFLAGS", "MAKEOVERRIDES", "BUNDLE_DATE", "GITHUB_SHA"):
                env.pop(name, None)
            result = subprocess.run(
                ["make", "-r", "--no-print-directory", "names",
                 "PROJECT=rusefi", "BUILDDIR=build", "SHORT_BOARD_NAME=test_board",
                 "BUNDLE_NAME=test_variant", "BUNDLE_SIMULATOR=false",
                 "META_OUTPUT_ROOT_FOLDER=", "WHITE_LABEL=rusefi",
                 "USE_OPENBLT=" + ("" if custom_image else "yes"),
                 "BOARD_IMAGE_SCRIPT=" + ("custom-image.py" if custom_image else ""),
                 *[f"{key}={value}" for key, value in metadata.items()]],
                cwd=work, env=env, capture_output=True, text=True, check=True,
            )
            return result.stdout.splitlines()

    def test_metadata_in_both_image_paths(self):
        for custom in (False, True):
            for lts in (False, True):
                with self.subTest(custom_image=custom, lts=lts):
                    folder = "lts-test" if lts else "snapshot"
                    release = "lts-test" if lts else "development"
                    filename = f"rusefi_{release}_2026-10-02_test_variant_1234567890_abc123_update.srec"
                    self.assertEqual(
                        [f"rusefi.{folder}.test_variant/{filename}",
                         "rusefi_bundle_test_variant.zip"],
                        self.names(custom, AUTOMATION_LTS="true" if lts else "false",
                                   AUTOMATION_REF="lts-test", BUNDLE_DATE="2026-10-02",
                                   GITHUB_SHA="abc123"),
                    )

    def test_local_defaults(self):
        for custom in (False, True):
            with self.subTest(custom_image=custom):
                filename = "rusefi_development_yymmdd_test_variant_1234567890_local_update.srec"
                self.assertEqual(
                    [f"rusefi.snapshot.test_variant/{filename}",
                     "rusefi_bundle_test_variant.zip"],
                    self.names(custom, AUTOMATION_LTS="false", AUTOMATION_REF=""),
                )


if __name__ == "__main__":
    unittest.main()
