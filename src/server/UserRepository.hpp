#pragma once
#include <map>
#include "User.hpp"
#include <mutex>
#include <thread>

class UserRepository
{
private:
	int next_id;
	std::map<int, User> users;
public:
	UserRepository();
	UserRepository(const UserRepository& userRepository) = delete;
	UserRepository(UserRepository&& userRepository) = delete;

	int createUser(User user);
	const User& getUser(int id) const;
	const User& getUser(const std::string login) const;
	const std::map<int, User>& getUsers() const;
	void updateUser(int id, User user);
	void removeUser(int id);
	bool isUserExist(int id) const;
};

