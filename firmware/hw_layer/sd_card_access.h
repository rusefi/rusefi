#pragma once

// Optional board-wide SD ownership. Default hooks allow access. Boards may refuse
// while a multi-command transfer or flash operation owns the filesystem. These
// hooks must be nonblocking; no thread-owned lock is retained between callers.
bool boardSdCardTryAccess();
void boardSdCardReleaseAccess();
void boardSdCardAccessFailed();

class SdCardBoardAccess {
    bool acquired;
public:
    SdCardBoardAccess() : acquired(boardSdCardTryAccess()) {}
    ~SdCardBoardAccess() { if (acquired) boardSdCardReleaseAccess(); }
    explicit operator bool() const { return acquired; }
    SdCardBoardAccess(const SdCardBoardAccess&) = delete;
    SdCardBoardAccess& operator=(const SdCardBoardAccess&) = delete;
};
