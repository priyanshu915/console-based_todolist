#include <iostream>
#include "json.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <conio.h>
using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;
class TodoList {
private:
	json accounts;
	void loadData() {
		ifstream file("accounts.json");
		if (file.is_open()) {
			file >> accounts;
			file.close();
		}
		else {
			cout << "Unable to open file" << endl;
		}
		file.close();
	}
	void saveData(json user) {
		for(auto& account: accounts) {
			if (account["id"] == user["id"]) {
				account = user;
			}
		}
		ofstream file("accounts.json");
		if (file.is_open()) {
			file << accounts.dump(4);
			file.close();
		}
		else {
			cout << "Unable to open file" << endl;
		}
		file.close();
	}
	string uid() {
		string letters = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY";
		string id;
		for (int i = 0; i < 8; i++) {
			int index = rand() % letters.length();
			id += letters[index];
		}
		return id;
	}
public:
	json user;
	TodoList() {
		if (!fs::exists("accounts.json")) {
			ofstream file("accounts.json");
			file << "[]" << endl;
			file.close();
		}
		loadData();
	}

	bool signup(const string username, const string email, const string password) {
		for(const auto& account: accounts) {
			if (account["email"] == email || account["password"]==password) {
				if (account["email"] == email) {
					cerr << "Email already exists" << endl;
					return false;
				}
				else {
					cerr << "Password already exists" << endl;
					return false;
				}
			}
		}
		json newAccount = {
			{"id", "_" + uid()},
			{"username", username},
			{"email", email},
			{"password", password},
			{"todos", json::array()}
		};
		user = newAccount;
		accounts.push_back(newAccount);
		saveData(user);
		return true;
	}
	bool signin(const string email, const string password) {
		for (const auto& account : accounts) {
			if (account["email"] == email && account["password"] == password) {
				user = account;
				return true;
			}
		}
		return false;
	}
	bool addTodo(const string todo) {
		pair<string, bool> todoPair = { todo, false };
		user["todos"].push_back(todoPair);
		saveData(user);
		return true;
	}

	void ViewTodos() {
		cout << left << setw(10) << "No." << setw(50) << "Todo" << setw(10) << "Status" << endl;
		cout << string(70, '-') << endl;

		for (int i = 0; i < user["todos"].size(); i++) {
			string status = user["todos"][i][1] ? "Done" : "Pending";
			cout << left
				<< setw(10) << i + 1
				<< setw(50) << user["todos"][i][0]
				<< setw(10) << status
				<< endl;
		}
	}

	bool MarkTodo(int todoNumber) {
		if (user["todos"][todoNumber - 1][1]) user["todos"][todoNumber - 1][1] = false;
		else user["todos"][todoNumber - 1][1] = true;
		saveData(user);
		return true;
	}

	bool DeleteTodo(int todoNumber) {
		//cout << (user["todos"][todoNumber-1]) << endl;
		user["todos"].erase(todoNumber-1);
		return true;
	}

	bool EditTodo(int todoNumber, string updatedTodo) {
		//cout << (user["todos"][todoNumber - 1]) << endl;
		user["todos"][todoNumber - 1][0] = updatedTodo;
		saveData(user);
		return true;
	}
};

void help() {
	cout << "Commands:" << endl;
	cout << "1. Add Todos" << endl;
	cout << "2. View Todos" << endl;
	cout << "3. Mark Todo as complete or pending" << endl;
	cout << "4. Delete Todo" << endl;
	cout << "5. Edit Todo" << endl;
	cout << "6. Sign out" << endl;
	cout << "7. Exit" << endl;
	cout << "8. Help" << endl;
}
int main() {
	TodoList todoList;
	
	while (true) {
		string auth;
		cout << "Please type ( signup / signin ): ";
		cin >> auth;
		if (auth == "signup") {
			string username, email, password;
			cout << "Enter username: ";
			cin >> username;
			cout << "Enter email: ";
			cin >> email;
			cout << "Enter password: ";
			cin >> password;
			if (todoList.signup(username, email, password)) {
				cout << "Account created successfully" << endl;
				break;
			}
		}
		else if (auth == "signin") {
			string email, password;
			cout << "Enter email: ";
			cin >> email;
			cout << "Enter password: ";
			cin >> password;
			if (todoList.signin(email, password)) {
				cout << "Welcome back Sir/Madam" << todoList.user["username"] << endl;
				break;
			}
			else {
				cerr << "Invalid email or password" << endl;
			}
		}
		else {
			cerr << "Please type only signup or signin" << endl;
			break;
		}
	}

	system("cls");
	cout << "Welcome to Console Based Todo List by Code with Priyanshu" << endl;
	help();
	while (true) {
		int option = 0;
		cout << "Enter your option number: ";
		cin >> option;
		if (option > 0 && option <= 8) {
			if (option == 1) {
				string todo;
				cout << "Enter your todo: ";
				cin.ignore();
				getline(cin, todo);
				if (todoList.addTodo(todo)) {
					cout << "Todo added successfully" << endl;
				}
			}
			else if (option == 2) {
				todoList.ViewTodos();
			}
			else if (option == 3) {
				int todoNumber = 0;
				cout << "Enter todo Number: ";
				cin >> todoNumber;
				todoList.MarkTodo(todoNumber);
			}
			else if (option == 4) {
				int todoNumber = 0;
				cout << "Enter todo Number: ";
				cin >> todoNumber;
				todoList.DeleteTodo(todoNumber);
			}
			else if (option == 5) {
				int todoNumber = 0;
				cout << "Enter todo Number: ";
				cin >> todoNumber;
				string updatedTodo;
				cout << "Enter the updated todo: ";
				cin.ignore();
				getline(cin, updatedTodo);
				todoList.EditTodo(todoNumber, updatedTodo);
			}
			else if (option == 6) {
				cout << "Are you sure for sign out" << endl;
				string query;
				cout << "Enter yes or no: ";
				cin.ignore();
				cin >> query;
				if (query == "yes") {
					cout << "Sign out Succesfull, " << todoList.user["username"] << "!" << endl;
					break;
				}
				else if (query == "no") {
					continue;
				}
				else cout << "Please enter only yes or no" << endl;
			}
			else if (option == 7) {
				cout << "Are you sure for exit" << endl;
				string query;
				cout << "Enter yes or no: ";
				cin.ignore();
				cin >> query;
				if (query == "yes") {
					cout << "Okay bye bye! " << todoList.user["username"] << endl;
					cout << "Email: " << todoList.user["email"] << endl;
					cout << "Password: " << todoList.user["password"] << endl;
					cout << "You can again sign in into your account" << endl;
					break;
				}
			}
			else if (option == 8) {
				system("cls");
				help();
				continue;
			}
		}
			
		else {
			cerr << "Please enter a valid option" << endl;
		}
	}
	return 0;
}