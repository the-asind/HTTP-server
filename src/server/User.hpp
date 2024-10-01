#pragma once
#include <iostream>
#include <string>
#include "json.hpp"

enum class Role {
    ADMIN = 0,
    USER
};

class User {
private:
    std::string _login;
    std::string _password;
    Role _role;
public:
    User(const std::string& login, const std::string& password, Role role);
    User();
    
    std::string getLogin() const;
    Role getRole() const;
    void setRole(Role role);
    bool validatePassword(const std::string& password) const;
    bool operator == (const User& user) const;
    bool operator != (const User& user) const;

    nlohmann::json toJson() const;
};