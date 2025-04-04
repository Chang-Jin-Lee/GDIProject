// Input.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Input.h"

namespace Input
{
    SHORT prevState[256] = { 0 };
    SHORT currState[256] = { 0 };

    static void Update() {
        memcpy_s(prevState, sizeof(prevState), currState, sizeof(currState));
        for (int i = 0; i < 256; i++) {
            currState[i] = GetAsyncKeyState(i);
        }
    }

    static bool IsKeyDown(int vKey) {
        return (currState[vKey] & 0x8000) != 0;
    }

    static bool IsKeyPressed(int vKey) {
        return (!(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000));
    }

    static bool IsKeyReleased(int vKey) {
        return ((prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000));
    }
}