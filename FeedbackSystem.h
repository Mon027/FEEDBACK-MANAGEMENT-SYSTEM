// FeedbackSystem.h (Header File)
#ifndef FEEDBACKSYSTEM_H
#define FEEDBACKSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct Feedback {
    string username;
    string message;
    bool isUrgent;
    bool isAddressed;
};

class FeedbackSystem {
private:
    vector<Feedback> feedbacks;
    const string filename = "feedback.txt";
    void saveFeedbackToFile();
    void loadFeedbackFromFile();
    bool analyzeSentiment(const string &message);
public:
    FeedbackSystem();
    void submitFeedback(const string &username);
    void viewFeedback();
    void markAsAddressed();
    void deleteFeedback();
};

#endif // FEEDBACKSYSTEM_H
