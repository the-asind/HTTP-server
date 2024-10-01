#include "User.hpp"
#include <regex>

User::User(const std::string& login, const std::string& password, Role role)
{
    std::regex r("[A-Za-z0-9]*");
    if (!std::regex_match(login, r) || !std::regex_match(password, r)) {
        throw std::invalid_argument("invalid argument format");
    }
    _login = login;
    _password = password;
    _role = role;
}

User::User() { }

std::string User::getLogin() const
{
    return _login;
}

Role User::getRole() const
{
    return _role;
}

void User::setRole(Role role)
{
    _role = role;
}

bool User::validatePassword(const std::string& password) const
{
    return _password == password;
}

bool User::operator==(const User &user) const
{
    return _login == user._login &&
        _password == user._password &&
        _role == user._role;
}

bool User::operator!=(const User &user) const
{
    return !(*this == user);
}

nlohmann::json User::toJson() const
{
    nlohmann::json j;
    j["login"] = _login;
    j["password"] = _password;
    if (_role == Role::ADMIN) {
        j["role"] = "admin";
    }
    else {
        j["role"] = "user";
    }
    return j;
}