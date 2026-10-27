#include "logger.h"
#include <fstream>
#include <iostream>

void WriteLog(const std::string &text)
{
    std::ofstream out("logs.txt", std::ios::app);
    if (out.is_open())
    {
        out << text << '\n';
        out.close();
    }
    else
    {
        std::cerr << "Не удалось открыть файл";
    }
}
