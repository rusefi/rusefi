@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0..\.."

git reset --hard
REM we are avoiding '--recursive' since we do not really need complete git dependency tree
REM for instance wideband/ext/openblt is not needed
git submodule update --init

REM a directory holding files tracked by this repo must never be its own git repo: such a '.git' is a
REM leftover of a submodule converted to plain files (e.g. cmsis-svd) and would hide the directory
REM from 'git status' and 'git clean' forever; purge the stale metadata so the clean below reaches it
for /f "delims=" %%G in ('git rev-parse --absolute-git-dir') do set "topgit=%%G"
for /f "delims=" %%F in ('git ls-files') do (
  set "f=%%F"
  REM only consider files that live in a subdirectory (skip top-level files -> never touch the top .git)
  if not "!f!"=="!f:/=!" (
    set "b=!f:/=\!"
    for %%D in ("!b!") do set "dir=%%~dpD"
    if exist "!dir!.git" (
      pushd "!dir!"
      set "gitdir="
      for /f "delims=" %%X in ('git rev-parse --absolute-git-dir 2^>nul') do set "gitdir=%%X"
      popd
      if defined gitdir if not "!gitdir!"=="!topgit!" rmdir /s /q "!gitdir!" 2>nul
      if exist "!dir!.git\" (rmdir /s /q "!dir!.git") else (del /f /q "!dir!.git")
      echo removed stale git metadata from !dir!
    )
  )
)

REM remove untracked/ignored files, '-ff' also removes nested git checkouts which are not submodules
git clean -ffxd
REM top-level 'git clean' does not descend into submodule working trees, clean those explicitly
git submodule foreach --recursive "git reset --hard && git clean -ffxd"
