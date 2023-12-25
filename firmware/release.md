# How to Release

1. Create a release branch off master in the format `release_YYMMDD`, for example `release_230410` for a release in 10 April 2023. Run `git checkout -b release_YYMMDD`
1. Create a tag on that branch in the format `release_YYMMDD_nn` where `nn` is a sequence number (so we can release a patch off the same branch later and they sort correctly). Run `git tag release_YYMMDD_nn`
1. Push the branch and tags
1. Manually run the `Firmware at GHA` build:
    * Use workflow from: tag name created above, `release_YYMMDD_nn`
    * Check `Upload build outputs`
    * Release Build Name: `release_YYMM`
1. Create a new release, upload all artifacts from the build
    * Title the release `MMMMM YYYY Release` (like `April 2023 Release`)

# How to Release a Patch

_patch releases should contain no new features or config changes, only bug fixes that actually matter_

1. Check out the previous release branch (created for the original release above)
1. Cherry pick the bugfix(es) you want from master (or make the fix by hand)
1. Create a tag in the format `release_YYMMDD_nn` where `nn` is incremented from the previous tag on that branch. For example, if the last tag was `release_230410_02`, create a tag `release_230410_03` for the new patch.
1. Push the branch & tag
1. Manually run the `Firmware at GHA` build:
    * Use workflow from: tag name created above, `release_YYMMDD_nn`
    * Check `Upload build outputs`
    * Release Build Name: `release_YYMM`
1. Create a new release, upload all artifacts from the build
    * Title the release `MMMMM YYYY Release Patch n` (like `April 2023 Release Patch 4`)
