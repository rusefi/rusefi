# Building rusEFI natively on macOS (Apple Silicon)

Everything that previously required the `rusefi_build` Docker container can now be
built directly on the macOS host. The scripts were adapted for macOS; this page
documents the required local tools.

## One-time setup

### 1. Homebrew packages

```bash
brew install coreutils make mtools dosfstools flock python3 openjdk@25
```

- `coreutils` - GNU `realpath`/`nproc` (used as `grealpath`/`gnproc`, detected automatically)
- `make` - GNU Make 4.x as `gmake` (macOS ships ancient GNU Make 3.81)
- `mtools dosfstools` - `mcopy`/`mkfs.fat`/`fatlabel` for the mass-storage ramdisk image
- `flock` - serializes parallel gradle invocations (optional; the build falls back to unlocked)
- `openjdk@25` - the project's Java toolchain (Gradle 9.x runs on and compiles with JDK 25)

### 2. ARM cross toolchain (from ARM, NOT brew)

The Homebrew `arm-none-eabi-gcc` formula ships without newlib (no target
`stdint.h`), so it cannot compile the firmware. Use ARM's official GNU toolchain,
which matches the container's compiler version (14.2.1):

```bash
curl -L -o /tmp/arm-gnu-toolchain.tar.xz \
  "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi.tar.xz"
sudo mkdir -p /opt/arm-gnu-toolchain
sudo tar -xJf /tmp/arm-gnu-toolchain.tar.xz -C /opt/arm-gnu-toolchain --strip-components=1
```

### 3. Environment (add to `~/.zshrc`)

```bash
export PATH="/opt/arm-gnu-toolchain/bin:$PATH"
export JAVA_HOME=/opt/homebrew/opt/openjdk@25/libexec/openjdk.jdk/Contents/Home
export PATH="$JAVA_HOME/bin:$PATH"
```

## Building

```bash
cd firmware
bash bin/compile.sh -b config/boards/m74_9/meta-info.env -j12
```

Outputs:
- `firmware/deliver/rusefi.bin` - full image
- `firmware/rusefi.snapshot.m74_9/` - bundle (console jar, ini, programmer CLI)

The Windows simulator is skipped on macOS by default (it needs mingw-w64); pass
`BUNDLE_SIMULATOR=true` and `brew install mingw-w64` to opt in.

## Unit tests

```bash
cd unit_tests
./test.sh            # build + run all
./test.sh TestName   # run matching tests only
```

Coverage: `./run_coverage.sh` (uses a local venv, needs `python3` from brew).

## Java console

```bash
./gradlew clean :ui:shadowJar
```

produces `console/rusefi_console.jar`.

## Notes

- The scripts pick GNU tools automatically (`gmake`, `grealpath`, `gnproc`,
  brew's `sbin` for FAT tools, `ln -fs` fallback for BSD ln). No PATH surgery
  beyond the toolchain + JDK above.
- `firmware/bin/compile.sh` and friends remain compatible with Linux; macOS
  branches are guarded by `uname -s`.
- Gradle 9.7 runs on JVM 17-26; the project toolchain is Java 25
  (`languageVersion=25` in the root build.gradle) and the produced bytecode is
  Java 25 (class major 69) - the console and the tools require a JRE 25 to run.
  Note: the TunerStudio plugin modules no longer load inside TunerStudio's
  bundled Java 8 runtime; TS would need a newer JVM.
- The repo directory is shared with the Docker container if you still use it:
  `rm -rf build .gradle firmware/build` before switching build environments,
  because gradle's configuration cache and make's dependency files embed
  absolute paths from the previous environment.
