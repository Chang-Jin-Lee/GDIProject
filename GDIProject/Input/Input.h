#pragma once

#include <Windows.h>

namespace Input
{
    static void Update();
    static bool IsKeyDown(int vKey);
    static bool IsKeyPressed(int vKey);
    static bool IsKeyReleased(int vKey);
}