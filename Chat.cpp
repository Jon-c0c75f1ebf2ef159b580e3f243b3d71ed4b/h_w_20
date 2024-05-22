#include "Chat.h"
#include <iostream>
//#include <memory>
//#include <exception>


void Chat::startChat()
{
	work_ = true;
	Info_system info_system;
	info_system.something();
	loadUsers();
	loadMessages();
	if (userArr_.empty()) {
		test_contetnt();
	}	
}

shared_ptr <User> Chat::getUserLog(const string& login) const
{
	for (auto& user : userArr_)
	{
		if (login == user.getlog())
		{
			return make_shared<User>(user);
		}

	}
	return nullptr;
}

shared_ptr <User> Chat::getUserName(const string& name) const
{
	for (auto& user : userArr_)
	{
		if (name == user.getName())
		{
			return make_shared<User>(user);
		}

	}
	return nullptr;
}


void Chat::userLogin()
{
	string login, password;
	char op;

	do
	{
		cout << "Login menu" << endl;
		cout << "Login: " << endl;
		cin >> login;
		cout << "Password: " << endl;
		cin >> password;

		currentUser_ = getUserLog(login);

		Hashing hashing;
		size_t hash_pas = hashing.hashing(password, login);

		if (currentUser_ == nullptr || hash_pas != currentUser_->getPass())
		{
			currentUser_ = nullptr;
			cout << "0 - exit, or any key " << endl;
			cin >> op;

			if (op == '0')
				break;
		}

	} while (!currentUser_);
}
void Chat::userRegistration()
{
	cout << "Registation" << endl;
	string login, password, name;

	cout << "Login:  " << endl;
	cin >> login;
	cout << "Password:   " << endl;
	cin >> password;
	cout << "Name: " << endl;
	cin >> name;

	if (getUserLog(login) || login == "all")
	{
		throw UserLoginEx();
	}

	Hashing hashing;
	size_t hash_pas = hashing.hashing(password, login);

	User user = User(login, hash_pas, name);
	userArr_.push_back(user);
	currentUser_ = make_shared <User>(user);

}
void Chat::showUserMenu()
{
	char op;
	cout << "Hello, " << currentUser_->getlog() << endl;
	while (currentUser_)
	{
		cout << "1 - Show chat, 2 - Add Message, 3 - showAllUser, 0 - exit" << endl;
		cin >> op;

		switch (op)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsers();
			break;
		case '0':
			currentUser_ = nullptr;
			break;

		default:
			cout << "Enter 1,2,3" << endl;
		}
	}

}

void Chat::addMessage()
{
	string to, text;

	cout << "Enter name, or all" << endl;
	cin >> to;
	cout << "Enter message" << endl;
	cin.ignore();
	getline(cin, text);

	if (!((to == "all") || getUserName(to)))
	{
		cout << "error the user is missing: " << to << endl;
		return;
	}

	if (to == "all")
	{
		messageArr_.push_back(Message{ currentUser_->getlog(), "all", text });
	}

	else
	{
		messageArr_.push_back(Message{ currentUser_->getlog(), getUserName(to)->getlog(), text });
	}
	
	string sender = currentUser_->getlog();
	TCP_client run_client;
    run_client.client_start(sender, to, text);
}

void Chat::showLoginMenu()
{
	currentUser_ = nullptr;
	char op;
	do
	{
		cout << " 1 - Login " << endl;
		cout << " 2 - Registration " << endl;
		cout << " 0 - exit " << endl;
		cin >> op;

		switch (op)
		{
		case '1':
			userLogin();
			break;
		case '2':
			try
			{
				userRegistration();
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;
			}
			break;
		case '0':
			work_ = false;
			saveUsers();
			saveMessages();
			break;

		default:
			cout << "vvedite 1, 2, 0" << endl;
		}
	} while (!currentUser_ && work_);
}

void Chat::showChat() const
{
	//load message from server by buffer_messahes.dat
	{   
	    fstream file("buffer_messahes.dat");
    	if (file.is_open()) {
    		while (!file.eof()) {
    			Message message2("all", "", "");
    			file >> message2;
    			messageArr_.push_back(message2);
    			//for clearing buffer_messahes.dat
    			ofstream ofs;
                ofs.open("buffer_messahes.dat", ofstream::out | ofstream::trunc);
                ofs.close();
    		}
    		file.close();
    		if (messageArr_.back().getFrom() == "all")
    			messageArr_.pop_back();
    	}
    	if (messageArr_.back().getText() == ( messageArr_[messageArr_.size() - 2] ).getText() ) {
    	    messageArr_.pop_back();
    	}
	}

	string from, to;
	cout << "Chat " << endl;

	for (auto& message : messageArr_)
	{
		if (currentUser_->getlog() == message.getFrom() || currentUser_->getlog() == message.getTo() || message.getTo() == "all")
		{
			from = (currentUser_->getlog() == message.getFrom()) ? "me" : getUserLog(message.getFrom())->getName();
		}

		if (message.getTo() == "all")
		{
			to = "(all)";
		}

		else
		{
			to = (currentUser_->getlog() == message.getTo()) ? "me" : getUserLog(message.getTo())->getName();
		}

		cout << "from: " << from << " to: " << to << endl;
		cout << " text " << message.getText() << endl;
	}

}

void Chat::showAllUsers() const
{
	cout << "users" << endl;

	for (auto& user : userArr_)
	{
		cout << user.getName();
		cout << endl;
		if (currentUser_->getlog() == user.getlog())
		{
			cout << "(me)";
			cout << endl;
		}
		cout << "" << endl;
	}
}

void Chat::test_contetnt()
{
	Hashing hashing;

	size_t hash_pas = hashing.hashing("123", "Jon");
	User user = User("Jon", hash_pas, "Jon");
	userArr_.push_back(user);

	hash_pas = hashing.hashing("123", "Sam");
	user = User("Sam", hash_pas, "Sam");
	userArr_.push_back(user);

	hash_pas = hashing.hashing("123", "Pet");
	user = User("Pet", hash_pas, "Pet");
	userArr_.push_back(user);

	cout << "Jon - 123 Sam - 123 Pet - 123\n";

	Message message("Jon", "all", "hello_everyone");
	messageArr_.push_back(message);

	message = Message("Sam", "Jon", "hello,_Jon!");
	messageArr_.push_back(message);
}

void Chat::saveUsers()
{
	std::fstream users_file = std::fstream("users.dat", std::ios::in | std::ios::out);
	if (!users_file) {
		// Для создания файла используем параметр ios::trunc
		users_file = std::fstream("users.dat", std::ios::in | std::ios::out | std::ios::trunc);
	}

	if (users_file) {
		for (const auto& elem : userArr_) {
			users_file << elem << "\n";
		}
	}
}

void Chat::saveMessages()
{
	fstream messages_file("messahes.dat", ios::in | ios::out);
	if (!messages_file) {
		// Для создания файла используем параметр ios::trunc
		messages_file = fstream("messahes.dat", ios::in | ios::out | ios::trunc);
	}

	if (messages_file) {
		for (const auto& elem : messageArr_) {
			messages_file << elem << "\n";
		}
	}
}

void Chat::loadUsers()
{
	fstream file("users.dat");
	if (file.is_open()) {
		cout << "initialization users ... ok\nFor debugging: Jon - 123 Sam - 123 Pet - 123\n";
		while (!file.eof()) {
			User user("", 0, "");
			file >> user;
			userArr_.push_back(user);
		}
		file.close();
		if (userArr_.back().getPass() == 0)
			userArr_.pop_back();
	}
}

void Chat::loadMessages()
{
	fstream file("messahes.dat");
	if (file.is_open()) {
		cout << "initialization messahes ... ok\n";
		while (!file.eof()) {
			Message message("all", "", "");
			file >> message;
			messageArr_.push_back(message);
		}
		file.close();
		if (messageArr_.back().getFrom() == "all")
			messageArr_.pop_back();
	}
}
