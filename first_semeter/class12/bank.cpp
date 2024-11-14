#include <bits/stdc++.h>
using namespace std;

User::User(string name): name(name), deposit(0) {}

User::User(string name, int deposit): name(name), deposit(deposit) {}

string User::getName() const {
    return name;
}

void User::setDeposit(int deposit) {
    this->deposit = deposit;
}

int User::getDeposit() const {
    return deposit;
}

void User::depositMoney(int money) {
    deposit += money;
}

void User::withdrawMoney(int money) {
    deposit -= money;
}

bool Bank::addUser(User user) {
    for (const auto& existingUser : users) {
        if (existingUser.getName() == user.getName()) {
            return false; 
        }
    }
    users.push_back(user);
    return true; 
}

bool Bank::depositMoney(std::string name, int money) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].getName() == name) {
            users[i].depositMoney(money);
            return true;
        }
    }
    return false;
}

bool Bank::withdrawMoney(std::string name, int money) {
    for (auto &user : users) {
        if (user.getName() == name) {
            if (user.getDeposit() >= money) {
                user.withdrawMoney(money);
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

bool Bank::transferMoney(std::string name1, std::string name2, int money) {
    for (auto &user : users) {
        if (user.getName() == name1) {
            if (user.getDeposit() >= money) {
                user.withdrawMoney(money);
                for (auto &user : users) {
                    if (user.getName() == name2) {
                        user.depositMoney(money);
                        return true;
                    }
                }
            } else {
                return false;
            }
        }
    }
    return false;  
}