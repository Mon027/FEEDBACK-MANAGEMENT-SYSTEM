#ifndef ALLFEEDBACKWINDOW_H
#define ALLFEEDBACKWINDOW_H

#include <QDialog>
#include <QVector>
#include "mainwindow.h"  // For the Feedback struct

namespace Ui {
class AllFeedbackWindow;
}

class AllFeedbackWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor: feedbackList is the current list; currentEmail is the logged-in user's email.
    explicit AllFeedbackWindow(const QVector<Feedback> &feedbackList, const QString &currentEmail, QWidget *parent = nullptr);
    ~AllFeedbackWindow();

    // Update the local feedback list and refresh the UI.
    void setFeedbackList(const QVector<Feedback> &list);
    void populateList();

signals:
    // Emitted when the Delete button is pressed.
    void deleteFeedbackRequested(int index);
    // Emitted when the Save button (from edit mode) is pressed.
    void editFeedbackRequested(int index, const QString &newMessage);

private:
    Ui::AllFeedbackWindow *ui;
    QVector<Feedback> m_feedbackList; // Local copy of feedback items.
    QString m_currentEmail;           // Email of the current user.
};

#endif // ALLFEEDBACKWINDOW_H
