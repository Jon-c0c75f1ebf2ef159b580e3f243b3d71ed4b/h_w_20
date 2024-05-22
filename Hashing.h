#pragma once
#include "User.h"
//#include <fstream> //User.h


class Message
{
public:
	Message(const string& messageFrom, const string& messageTo, const string& text) : messageFrom_(messageFrom), messageTo_(messageTo), text_(text) {}

	const string& getFrom() const { return messageFrom_; }
	const string& getTo() const { return messageTo_; }
	const string& getText() const { return text_; }

	friend std::fstream& operator >>(std::fstream& is, Message& obj) {
		is >> obj.messageFrom_;
		is >> obj.messageTo_;
		is >> obj.text_;
		return is;
	}

	friend std::ostream& operator <<(std::ostream& os, const Message& obj) {
		os << obj.messageFrom_;
		os << ' ';
		os << obj.messageTo_;
		os << ' ';
		os << obj.text_;
		return os;
	}

private:
	string messageFrom_;
	string messageTo_;
	string text_;

};
