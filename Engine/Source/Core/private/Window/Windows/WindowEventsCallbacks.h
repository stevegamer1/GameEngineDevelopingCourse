#pragma once

#include <Camera.h>
#include <Constants.h>
#include <Window/IWindow.h>
#include <Window.h>
#include <Window/KeyListenersKeeper.h>

namespace GameEngine::Core
{
    void OnMouseDown(WPARAM btnState, int x, int y, Window* window)
    {
        window->SetMousePos(x, y);

        SetCapture(GetPlatformWindowHandle(window->GetWindowHandle()));
    }

    void OnMouseUp(WPARAM btnState, int x, int y)
    {
        ReleaseCapture();
    }

    void OnMouseMove(WPARAM btnState, int x, int y, Camera* camera, Window* window)
    {
        Math::Vector2i mousePos = window->GetMousePos();
        if ((btnState & MK_LBUTTON) != 0)
        {
            float dx = 0.25 * static_cast<float>(x - mousePos.x) * Math::Constants::PI / 180.f;
            float dy = 0.25 * static_cast<float>(y - mousePos.y) * Math::Constants::PI / 180.f;

            dy = -dy; // To avoid inverse movement

            camera->Rotate(dx, dy);
        }
        else if ((btnState & MK_RBUTTON) != 0)
        {
            float dx = 0.05f * static_cast<float>(x - mousePos.x);
            float dy = 0.05f * static_cast<float>(y - mousePos.y);

            Math::Vector3f offset = camera->GetViewDir() * (dx - dy);

            Math::Vector3f position = camera->GetPosition();
            position = position + offset;

            camera->SetPosition(position);
        }

        window->SetMousePos(x, y);
    }

    void OnKeyDown(WPARAM wParam, LPARAM lParam)
    {
        const LPARAM IS_EVENT_REPEATED_BIT = 1 << 30;
        if (lParam & IS_EVENT_REPEATED_BIT) {
          return;
        }

        WPARAM key = wParam;
        switch (key) {
        case 'W':
            KeyListenersKeeper::CallDownListeners(KeyListenersKeeper::Key::W);
            break;
        case 'A':
            KeyListenersKeeper::CallDownListeners(KeyListenersKeeper::Key::A);
            break;
        case 'S':
            KeyListenersKeeper::CallDownListeners(KeyListenersKeeper::Key::S);
            break;
        case 'D':
            KeyListenersKeeper::CallDownListeners(KeyListenersKeeper::Key::D);
            break;
        }
    }

    void OnKeyUp(WPARAM wParam, LPARAM lParam)
    {
        WPARAM key = wParam;
        switch (key) {
        case 'W':
            KeyListenersKeeper::CallUpListeners(KeyListenersKeeper::Key::W);
            break;
        case 'A':
            KeyListenersKeeper::CallUpListeners(KeyListenersKeeper::Key::A);
            break;
        case 'S':
            KeyListenersKeeper::CallUpListeners(KeyListenersKeeper::Key::S);
            break;
        case 'D':
            KeyListenersKeeper::CallUpListeners(KeyListenersKeeper::Key::D);
            break;
        }
    }
}