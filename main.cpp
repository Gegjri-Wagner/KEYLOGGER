#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <windows.h>
#include <fstream>
#include <thread>
#include <conio.h>
#include <io.h>
#include "Time.h"
#include "logger.h"
#include "Window.h"
#include "Hotkey.h"

// Функция конвертации wstring (UTF-16) в string (UTF-8)
std::string WideToUtf8(const std::wstring &wstr)
{
    if (wstr.empty())
        return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0,
                                          nullptr, nullptr);
    if (size_needed <= 0)
        return std::string();

    std::string resultStr(size_needed, 0);
    int writtenBytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
                                           &resultStr[0], size_needed, nullptr, nullptr);
    if (writtenBytes == 0)
        return {};
    if (size_needed != writtenBytes)
        return {};
    return resultStr;
}

int main()
{
    // Настройка UTF-8 для консоли Windows
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "KEYLOGGER запущен.\n";

    if (!RegisterglobalHotkey())
    {
        DWORD errorRegister = GetLastError();
        std::cerr << "Ошибка при регистрации горячей клав";
        std::cerr << "Ошибка Windows: " << errorRegister << '\n';
        return 1;
    }

    // Запоминаем первоначальное активное окно
    std::string previousTitle = WideToUtf8(GetActiveWindowTitle());
    while (true)
    {
        // 1. Проверяем активное окно
        std::string currentTitle = WideToUtf8(GetActiveWindowTitle());

        if (!currentTitle.empty() && currentTitle != previousTitle)
        {
            previousTitle = currentTitle;

            std::string logMessage = "[" + currentTitle + "] [" + GetFormattedTime() + "] SWITCH";

            std::cout << logMessage << '\n';
            WriteLog(logMessage);
        }

        // 2. Проверяем ввод в собственной консоли
        if (_kbhit())
        {
            wint_t ch = _getwch();

            std::string key;

            // Специальные клавиши могут возвращать два кода
            if (ch == 0 || ch == 224)
            {
                (void)_getwch();
            }
            else
            {
                if (ch == 13)
                    key = "ENTER";
                else if (ch == 8)
                    key = "BACKSPACE";
                else if (ch == 9)
                    key = "TAB";
                else if (ch == 27)
                    key = "ESC";
                else if (ch >= 32)
                {
                    std::wstring wideKey(1, static_cast<wchar_t>(ch));
                    key = WideToUtf8(wideKey);
                }

                if (!key.empty())
                {
                    std::string logMessage = "[CONSOLE INPUT] [" + GetFormattedTime() + "] " + key;

                    std::cout << logMessage << '\n';
                    WriteLog(logMessage);
                }
            }
        }
        if (ProcessMessage())
        {
            std::cout << " Нажата Crtl + Shift + K\n";
            break;
        }

        // 3. Задержка между проверками
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    UnregisterHotKey();

    return 0;
}
