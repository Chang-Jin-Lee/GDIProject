// Input.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Input.h"

namespace Input
{
    SHORT prevState[256] = { 0 };
    SHORT currState[256] = { 0 };
    HWND m_hwnd;
    POINT m_point;
    int m_wheelDelta = 0; // 누적 휠 델타(프레임 단위로 소비)

    void Initialize(HWND hwnd)
    {
        m_hwnd = hwnd;
    }

    void Update()
    {
        memcpy_s(prevState, sizeof(prevState), currState, sizeof(currState));
        for (int i = 0; i < 256; i++) 
        {
            currState[i] = GetAsyncKeyState(i);
        }

        GetCursorPos(&m_point);
        ScreenToClient(m_hwnd, &m_point);
    }

    bool IsKeyDown(int vKey) 
    {
        return (currState[vKey] & 0x8000) != 0;
    }

    bool IsKeyPressed(int vKey) 
    {
        return (!(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000));
    }

    bool IsKeyReleased(int vKey) 
    {
        return ((prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000));
    }

    FVector2 GetMousePosition()
    {
        return FVector2(float(m_point.x), float(m_point.y));
    }

    FVector2 GetMouseWorldPosition(std::weak_ptr<ACameraActor> camera)
    {
        FVector2 screenPos = GetMousePosition();
        FVector2 cameraPos = FVector2(0,0);
        if (const auto cameraRef = camera.lock())
        {
            cameraPos = cameraRef->GetActorLocation();
        }
        return screenPos + cameraPos;
    }

    int GetWheelDelta()
    {
        int delta = m_wheelDelta;
        m_wheelDelta = 0; // 사용 후 리셋합니다
        return delta;
    }

    void OnMouseWheel(int delta)
    {
        m_wheelDelta += delta;
    }
}