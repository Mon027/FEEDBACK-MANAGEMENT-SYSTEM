#include <iostream>
#include "UserSystem.h"
#include "FeedbackSystem.h"

using namespace std;

int main() {
    UserSystem userSystem;
    FeedbackSystem feedbackSystem;
    string username, role;
    
    cout << "\n=== Welcome to the System ===\n";
    while (true) {
        int choice;
        cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
        cin >> choice;
        
        if (choice == 1) {
            if (!userSystem.isAdminAvailable()) {
                userSystem.registerUser(true); // First user must be an admin
            } else {
                userSystem.registerUser(false); // Regular users can register as customers
            }
        } else if (choice == 2) {
            if (userSystem.authenticate(username, role)) {
                break;
            }
        } else if (choice == 3) {
            cout << "Exiting...\n";
            return 0;
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }
    
    while (true) {
        int option;
        cout << "\n=== Main Menu ===\n";
        cout << "1. Submit Feedback\n2. View Feedback\n";
        if (role == "admin") {
            cout << "3. Mark Feedback as Addressed\n4. Delete Feedback\n";
        }
        cout << "5. Logout\n6. Exit\nChoice: ";
        cin >> option;
        
        if (option == 1) {
            feedbackSystem.submitFeedback(username);
        } else if (option == 2) {
            feedbackSystem.viewFeedback();
        } else if (option == 3 && role == "admin") {
            feedbackSystem.markAsAddressed();
        } else if (option == 4 && role == "admin") {
            feedbackSystem.deleteFeedback();
        } else if (option == 5) {
            cout << "Logging out...\n";
            return main(); // Restart login process
        } else if (option == 6) {
            cout << "Exiting program...\n";
            return 0;
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }
    return 0;
}
