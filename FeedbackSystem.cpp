#include "FeedbackSystem.h"

FeedbackSystem::FeedbackSystem() {
    loadFeedbackFromFile();
}

void FeedbackSystem::saveFeedbackToFile() {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving feedback to file.\n";
        return;
    }
    for (const auto &fb : feedbacks) {
        file << fb.username << "|" << fb.message << "|" << fb.isUrgent << "|" << fb.isAddressed << "\n";
    }
    file.close();
}

void FeedbackSystem::loadFeedbackFromFile() {
    ifstream file(filename);
    if (!file) {
        return;
    }
    feedbacks.clear();
    string line, username, message;
    bool isUrgent, isAddressed;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, username, '|');
        getline(ss, message, '|');
        ss >> isUrgent >> isAddressed;
        feedbacks.push_back({username, message, isUrgent, isAddressed});
    }
    file.close();
}

bool FeedbackSystem::analyzeSentiment(const string &message) {
    return message.find("bad") != string::npos || message.find("terrible") != string::npos || message.find("urgent") != string::npos;
}

void FeedbackSystem::submitFeedback(const string &username) {
    string message;
    cout << "Enter your feedback: ";
    cin.ignore();
    getline(cin, message);
    bool urgent = analyzeSentiment(message);
    feedbacks.push_back({username, message, urgent, false});
    saveFeedbackToFile();
    cout << "Feedback submitted successfully!\n";
}

void FeedbackSystem::viewFeedback() {
    if (feedbacks.empty()) {
        cout << "No feedback available.\n";
        return;
    }
    cout << "\n=== Feedback List ===\n";
    for (size_t i = 0; i < feedbacks.size(); i++) {
        cout << i + 1 << ". User: " << feedbacks[i].username << "\n   Message: " << feedbacks[i].message
             << "\n   Urgent: " << (feedbacks[i].isUrgent ? "Yes" : "No")
             << " | Addressed: " << (feedbacks[i].isAddressed ? "Yes" : "No") << "\n";
    }
}

void FeedbackSystem::markAsAddressed() {
    int index;
    cout << "Enter feedback number to mark as addressed: ";
    cin >> index;
    if (index > 0 && index <= feedbacks.size()) {
        feedbacks[index - 1].isAddressed = true;
        saveFeedbackToFile();
        cout << "Feedback marked as addressed.\n";
    } else {
        cout << "Invalid selection.\n";
    }
}

void FeedbackSystem::deleteFeedback() {
    int index;
    cout << "Enter feedback number to delete: ";
    cin >> index;
    if (index > 0 && index <= feedbacks.size()) {
        feedbacks.erase(feedbacks.begin() + index - 1);
        saveFeedbackToFile();
        cout << "Feedback deleted.\n";
    } else {
        cout << "Invalid selection.\n";
    }
}