#include "Window.h"
#include <windows.h>

std::wstring GetActiveWindowTitle()
{
    wchar_t title[256]{};

    HWND window = GetForegroundWindow();

    GetWindowTextW(window, title, 256);

    return title;
}
