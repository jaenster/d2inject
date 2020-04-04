#pragma once

#include "../Globals.h"

class CriticalRoom {
  private:
    bool bEnteredCriticalSection;

  public:
    CriticalRoom() : bEnteredCriticalSection(false) {
    }
    ~CriticalRoom() {
        LeaveSection();
    }

    void EnterSection() {
        InterlockedIncrement(&global.SectionCount);
        EnterCriticalSection(&global.cGameLoopSection);
        bEnteredCriticalSection = true;
        InterlockedDecrement(&global.SectionCount);
    }

    void LeaveSection() {
        if (bEnteredCriticalSection) {
            bEnteredCriticalSection = false;
            LeaveCriticalSection(&global.cGameLoopSection);
        }
    }
};

class AutoCriticalRoom {
  private:
    bool bEnteredCriticalSection;
    void EnterSection() {
        InterlockedIncrement(&global.SectionCount);
        bEnteredCriticalSection = true;
        EnterCriticalSection(&global.cGameLoopSection);
        InterlockedDecrement(&global.SectionCount);
    }

    void LeaveSection() {
        if (bEnteredCriticalSection) {
            bEnteredCriticalSection = false;
            LeaveCriticalSection(&global.cGameLoopSection);
        }
    }

  public:
    AutoCriticalRoom() : bEnteredCriticalSection(false) {
        EnterSection();
    }
    ~AutoCriticalRoom() {
        LeaveSection();
    }
};