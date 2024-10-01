#include "UserRepository.hpp"

#include <utility>

std::mutex map_access;

UserRepository::UserRepository() {
    next_id = -1;
    User root("root", "root", Role::ADMIN);
    createUser(root);
}

int UserRepository::createUser(User user)
{
    std::lock_guard<std::mutex> lockGuard(map_access);
    users[next_id] = std::move(user);
    return next_id++;
}

const User& UserRepository::getUser(int id) const
{
    if (!isUserExist(id)) {
        throw std::out_of_range("no user found with id: " + std::to_string(id));
    }
    
    std::lock_guard<std::mutex> lockGuard(map_access);
    return users.at(id);
}

const User &UserRepository::getUser(const std::string login) const
{
    auto it = std::find_if(users.begin(), users.end(), [&](const auto& pair) {
        return pair.second.getLogin() == login;
    });

    if (it == users.end()) {
        throw std::out_of_range("no user found with login: " + login);
    }

    return it->second;
}

const std::map<int, User>& UserRepository::getUsers() const
{
    std::lock_guard<std::mutex> lockGuard(map_access);
    return users;
}

void UserRepository::updateUser(int id, User user)
{
    if (!isUserExist(id))
        throw std::out_of_range("no user found with id: " + std::to_string(id));

    std::lock_guard<std::mutex> lockGuard(map_access);
    users[id] = std::move(user);
}

void UserRepository::removeUser(int id)
{
    if (!isUserExist(id))
        throw std::out_of_range("no user found with id: " + std::to_string(id));

    std::lock_guard<std::mutex> lockGuard(map_access);
    users.erase(id);
}

bool UserRepository::isUserExist(int id) const
{
    return users.find(id) != users.end();
}