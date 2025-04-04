#pragma once

#include <Windows.h>

#define VK_Q 0x41 + 'q' - 'a'
#define VK_W 0x41 + 'w' - 'a'
#define VK_E 0x41 + 'e' - 'a'
#define VK_R 0x41 + 'r' - 'a'
#define VK_T 0x41 + 't' - 'a'
#define VK_R 0x41 + 'r' - 'a'
#define VK_Z 0x41 + 'z' - 'a'
#define VK_X 0x41 + 'x' - 'a'
#define VK_C 0x41 + 'c' - 'a'

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39 

namespace Input
{
    void Update();
    bool IsKeyDown(int vKey);
    bool IsKeyPressed(int vKey);
    bool IsKeyReleased(int vKey);
}


