#pragma once
#include <string>
#include <map>

namespace ui
{
    [[noreturn]] void run();
    void inputServerConnection(std::string &url, std::string &port);
    void clientMenu(const std::string url, const std::string port);
    void inputUserData(std::string *login, std::string *password);
    bool checkSelectionInput(const std::string &input, int *inp, int min, int max);
    std::string inputAuth();
}