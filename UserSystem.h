#ifndef USERSYSTEM_H
#define USERSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct User {
    string username;
    string password;
    string role;
};

class UserSystem {
private:
    vector<User> users;
    const string filename = "users.txt";
    void saveUsersToFile();
    void loadUsersFromFile();
public:
    UserSystem();
    bool isAdminAvailable();
    void registerUser(bool firstAdmin = false);
    bool authenticate(string &username, string &role);
};

#endif // USERSYSTEM_H
