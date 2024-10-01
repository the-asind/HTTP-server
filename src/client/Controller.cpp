#include <iostream>
#include "HttpClient.h"
#include "Menu.hpp"
#include "Controller.hpp"
#include "json.hpp"

void controller::createUser(std::string url) {
    std::string login, password;

    std::cout << "Enter login and password of new user:" << std::endl;
    ui::inputUserData(&login, &password);

    nlohmann::json req_body;
    req_body["login"] = login;
    req_body["password"] = password;
    
    hv::HttpClient client;
    HttpRequest req;    
    HttpResponse resp;
    
    req.method = HTTP_POST;
    req.url = url + "/user";
    req.body = req_body.dump();

    if (client.send(&req, &resp) != 0) {
        std::cout << "request error" << std::endl;
        return;
    }

    std::cout << resp.body << std::endl;
}

void controller::getUser(std::string url) {
    hv::HttpClient client;
    HttpRequest req;
    HttpResponse resp;

    std::string id;
    std::cout << "Enter ID of user you want to get" << std::endl;
    std::cin >> id;

    req.method = HTTP_GET;
    req.url = url + "/user/" + id;
    
    std::cout << "Authorization. Enter your login and password" << std::endl;
    req.headers["Authorization"] = "Basic " + ui::inputAuth();

    if (client.send(&req, &resp) != 0) {
        std::cout << "Request error" << std::endl;
        return;
    }

    switch (resp.status_code)
    {
    case 404:
        std::cout << "User does not exists" << std::endl;
        break;
    case 403:
        std::cout << "Access forbidden" << std::endl;
        break;
    case 200:
        std::cout << resp.body << std::endl;
        break;
    default:
        break;
    }
}

void controller::getUsers(std::string url) {
        hv::HttpClient client;
    HttpRequest req;
    HttpResponse resp;

    req.method = HTTP_GET;
    req.url = url + "/users";

    if (client.send(&req, &resp) != 0) {
        std::cout << "request error" << std::endl;
        return;
    }

    if (resp.status_code == 401) {
        std::cout << "Authorization. Enter login and password" << std::endl;
        req.headers["Authorization"] = "Basic " + ui::inputAuth();

        if (client.send(&req, &resp) != 0) {
            std::cout << "Request error" << std::endl;
            return;
        }
    }

    switch (resp.status_code)
    {
    case 404:
        std::cout << "User does not exists" << std::endl;
        break;
    case 403:
        std::cout << "Access forbidden" << std::endl;
        break;
    case 200:
        std::cout << resp.body << std::endl;
        break;
    default:
        break;
    }
}

void controller::updateUser(std::string url) {
    nlohmann::json req_body, return_data;
    
    hv::HttpClient client;
    HttpResponse resp;
    HttpRequest req;

    std::string id, newUsername, newPassword;
    std::cout << "Enter id of user you want to update:" << std::endl;
    std::cin >> id;

    req.method = HTTP_PUT;
    req.url = url + "/user/" + id;
    
    nlohmann::json authUserData;

    nlohmann::json userData;
    std::string login, password;
    std::cout << "Enter login and password of updated user:" << std::endl;
    ui::inputUserData(&login, &password);

    userData["login"] = login;
    userData["password"] = password;

    while (true) {
        std::cout << "Select user role" << std::endl;
        std::cout << "[1] admin" << std::endl;
        std::cout << "[2] user" << std::endl;
        std::string input;
        int inp;
        std::cin >> input;
        if (ui::checkSelectionInput(input, &inp, 1, 2)) {
            if (inp == 1) 
                userData["role"] = "admin";
            else if (inp == 2)
                userData["role"] = "user";
            break;
        }
    }
    req.body = userData.dump();

    std::cout << "Authorization. Enter login and password" << std::endl;
    req.headers["Authorization"] = "Basic " + ui::inputAuth();

    if (client.send(&req, &resp) != 0) {
        std::cout << "Request error" << std::endl;
        return;
    }

    switch (resp.status_code)
    {
    case 404:
        std::cout << "User does not exists" << std::endl;
        return;
    case 403:
        std::cout << "Access forbidden" << std::endl;
        return;
    }

    std::cout << resp.body << std::endl;    
}

void controller::deleteUser(std::string url) {
    hv::HttpClient client;
    HttpRequest req;
    HttpResponse resp;

    std::string id;
    std::cout << "Delete user. Enter id" << std::endl;
    std::cin >> id;

    req.method = HTTP_DELETE;
    req.url = url + "/user/" + id;
    
    if (client.send(&req, &resp) != 0) {
        std::cout << "request error" << std::endl;
        return;
    }

    if (resp.status_code == 401) {
        std::cout << "Authorization. Enter login and password" << std::endl;
        req.headers["Authorization"] = "Basic " + ui::inputAuth();

        if (client.send(&req, &resp) != 0) {
            std::cout << "Request error" << std::endl;
            return;
        }
    }

    switch (resp.status_code)
    {
    case 404:
        std::cout << "User does not exists" << std::endl;
        break;
    case 403:
        std::cout << "Access forbidden" << std::endl;
        break;
    case 200:
        std::cout << resp.body << std::endl;
        break;
    default:
        break;
    }
}
