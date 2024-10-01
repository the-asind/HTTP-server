#include <iostream>
#include <regex>
#include "Menu.hpp"
#include "Controller.hpp"
#include "Utility.hpp"

typedef void (*function_pointer)(std::string);
std::map<int, function_pointer> controllerMap;

[[noreturn]] void ui::run() {
    controllerMap[1] = &controller::createUser;
    controllerMap[2] = &controller::getUsers;
    controllerMap[3] = &controller::getUser;
    controllerMap[4] = &controller::updateUser;
    controllerMap[5] = &controller::deleteUser;

    while(true) {
        std::string ip, port;
        inputServerConnection(ip, port);
        clientMenu(ip, port);
    }
}

void ui::inputServerConnection(std::string &ip, std::string &port) {
    std::regex ip_r("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
    std::regex port_r("[0-9]{1,5}");
    
    std::cout << "Input server url and port below." << std::endl;
    std::cout << "IPv4-address: ";
    std::getline(std::cin, ip);
    std::cout << "Port: ";
    std::getline(std::cin, port);

    while(!std::regex_match(ip, ip_r) || !std::regex_match(port, port_r)) {
        system("cls");
        std::cout << "Invalid ip or port format." << std::endl;
        std::cout << "Input server url and port below." << std::endl;
        std::cout << "IPv4-address: ";
        std::getline(std::cin, ip);
        std::cout << "Port: ";
        std::getline(std::cin, port);
    }
    
    system("cls");
}

void ui::clientMenu(const std::string ip, const std::string port) {
    std::string url = "http://" + ip + ":" + port;
    std::string input;
    int selection;
    while(true) {
        std::cout << std::endl;
        std::cout << "Server: " << url << std::endl;
        std::cout << "[1] Create new user" << std::endl;
        std::cout << "[2] Get all users" << std::endl;
        std::cout << "[3] Get user" << std::endl;
        std::cout << "[4] Update user" << std::endl;
        std::cout << "[5] Remove user" << std::endl;
        std::cout << "[6] Close connection" << std::endl;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (!ui::checkSelectionInput(input, &selection, 1, 6))
            continue;

        if (selection == 6)
            return;

        controllerMap[selection](url);
        std::cout.flush();
        std::cout << "Press enter to continue" << std::endl;
        std::string temp;
        getchar();
        std::getline(std::cin, temp);
    }
}

void ui::inputUserData(std::string * login, std::string * password) {
    std::regex r("[A-Za-z0-9]*");

    std::cout << "Allowed characters: A-Z, a-z, 0-9" << std::endl;    
    std::cout << "Login: ";
    std::cin >> *login;
    std::cout << "Password: ";
    std::cin >> *password;
    while(!std::regex_match(*login, r) || !std::regex_match(*password, r)) {
        std::cout << "Error. Try again." << std::endl;
        std::cout << "login: ";
        std::cin >> *login;
        std::cout << "password: ";
        std::cin >> *password;
    }
}

bool ui::checkSelectionInput(const std::string &input, int *selection, int min, int max) {
    size_t pos;
    *selection = -1;
    try {
        *selection = std::stoi(input, &pos);
        if (pos != input.size())
            throw std::invalid_argument(input);
    }
    catch (...) {
        return false;
    }

    if (*selection < min || *selection > max)
        return false;
    
    return true;
}

std::string ui::inputAuth()
{
    std::string login;
    std::string password;
    inputUserData(&login, &password);

    return utils::EncodeBase64(login) + ":" + utils::EncodeBase64(password); 
}
