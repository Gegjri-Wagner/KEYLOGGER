#include "Hotkey.h"
#include <windows.h>
#include <iostream>

bool RegisterglobalHotkey()
{
    if (RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_SHIFT, 'K'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ProcessMessage()
{
    MSG msg;
    if (PeekMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY, PM_REMOVE))
    {
        if (msg.message == WM_HOTKEY && msg.wParam == 1)
        {
            return true;
        }
    }
    return false;
}

bool UnregisterHotKey()

{
    if (UnregisterHotKey(nullptr, 1))
    {
        std::cout << "Удаление горячих клавиш завершено\n";
        return true;
    }
    else
    {
        DWORD errorCode = GetLastError();
        std::cerr << "Ошибка при удалении горячих клавиш\n";
        std::cerr << "Код ошибки Windows: " << errorCode << '\n';
        return false;
    }
}
