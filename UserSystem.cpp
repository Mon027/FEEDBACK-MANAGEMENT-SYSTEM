#include "UserSystem.h"

UserSystem::UserSystem() {
    loadUsersFromFile();
    if (users.empty()) {
        cout << "No users found. Creating default admin account.\n";
        users.push_back({"admin", "admin123", "admin"});
        saveUsersToFile();
    }
}

bool UserSystem::isAdminAvailable() {
    for (const auto &user : users) {
        if (user.role == "admin") {
            return true;
        }
    }
    return false;
}

void UserSystem::registerUser(bool firstAdmin) {
    string username, password, role = "customer";
    cout << "\n=== Registration ===\n";
    cout << "Enter new username: ";
    cin >> username;

    for (const auto &u : users) {
        if (u.username == username) {
            cout << "Error: Username already exists.\n";
            return;
        }
    }

    cout << "Enter password: ";
    cin >> password;

    if (firstAdmin || users.empty()) {
        cout << "Auto-assigning role: Admin.\n";
        role = "admin";
    }
    
    users.push_back({username, password, role});
    saveUsersToFile();
    cout << "Registration successful! Your role: " << role << "\n";
}

bool UserSystem::authenticate(string &username, string &role) {
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (const auto &u : users) {
        if (u.username == username && u.password == password) {
            role = u.role;
            cout << "Login successful! Welcome, " << username << " (" << role << ").\n";
            return true;
        }
    }
    cout << "Invalid credentials.\n";
    return false;
}

void UserSystem::saveUsersToFile() {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving users to file.\n";
        return;
    }
    for (const auto &user : users) {
        file << user.username << " " << user.password << " " << user.role << "\n";
    }
    file.close();
}

void UserSystem::loadUsersFromFile() {
    ifstream file(filename);
    if (!file) {
        return;
    }
    users.clear();
    string line, username, password, role;
    while (getline(file, line)) {
        stringstream ss(line);
        ss >> username >> password >> role;
        users.push_back({username, password, role});
    }
    file.close();
}
