#include <iostream>
#include "json.hpp"
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <filesystem>
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
	void saveData() {
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
		string id = to_string(rand() % 1000000);
		return id;
	}
public:
	json user;
	TodoList() {
		if (fs::exists("accounts.json")) {
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
		return true;
	}
	bool signin(const string email, const string password) {
		for (const auto& account : accounts) {
			if(account["email"] != email && account["password"] != password) {
				cerr << "Account was not found" << endl;
				return false;
			}
		}
		for (const auto& account : accounts) {
			if (account["email"] == email && account["password"] == password) {
				user = account;
				return true;
			}
		}
	}
};

int main() {
	cout << "hello world" << endl;
	return 0;
}