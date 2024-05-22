#pragma once
#include <string>
#include <fstream>
using namespace std;

class User
{
public:
	User(const string& login, const size_t& password, const string& name) : login_(login), password_(password), name_(name) {}
	const string& getlog() const { return login_; }
	const string& getName() const { return name_; }
	const size_t& getPass() const { return password_; }

	void setName(const string& name) { name_ = name; }
	void setPass(const size_t& password) { password_ = password; }

	friend std::fstream& operator >>(std::fstream& is, User& obj) {
		is >> obj.login_;
		is >> obj.password_;
		is >> obj.name_;
		return is;
	}

	friend std::ostream& operator <<(std::ostream& os, const User& obj) {
		os << obj.login_;
		os << ' ';
		os << obj.password_;
		os << ' ';
		os << obj.name_;
		return os;
	}

private:
	string login_;      //login is salt for hash of the password
	size_t password_;   //hash of the salt and password
	string name_;

};
