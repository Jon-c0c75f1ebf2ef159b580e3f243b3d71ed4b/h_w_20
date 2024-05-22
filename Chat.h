#pragma once
#include <vector>
#include<memory>
#include <exception>
#include "Message.h"
#include "info_system.h"
#include "Hashing.h"
#include "TCP_client.h"

using namespace std;

struct UserLoginEx : public exception
{
	const char* what() const noexcept override { return "Error: user login is busy"; }
};

class Chat
{
public:
	void startChat();
	void showLoginMenu();
	void showUserMenu();
	bool work() const { return work_; }
	shared_ptr <User> getcurrentUser() const { return currentUser_; }

private:
	vector <User> userArr_;
	mutable vector <Message> messageArr_;
	shared_ptr <User> currentUser_ = nullptr;
	bool work_ = false;

	void userLogin();
	void userRegistration();
	void showChat() const;
	void showAllUsers() const;
	void addMessage();  //
	void saveUsers();
	void saveMessages();
	void loadUsers();
	void loadMessages();

	vector <User>& getAlluser() { return userArr_; }
	vector <Message>& getAllmessage() { return messageArr_; }
	shared_ptr <User> getUserLog(const string& login) const;
	shared_ptr <User> getUserName(const string& name) const;

	void test_contetnt();   //
};
