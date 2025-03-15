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
    // Constructor: receives the feedback list, the current user's email,
    // and an isAdmin flag.
    explicit AllFeedbackWindow(const QVector<Feedback> &feedbackList,
                               const QString &currentEmail,
                               bool isAdmin,
                               QWidget *parent = nullptr);
    ~AllFeedbackWindow();

    // Update the local feedback list and refresh the UI.
    void setFeedbackList(const QVector<Feedback> &list);
    void populateList();

signals:
    void deleteFeedbackRequested(int index);
    void editFeedbackRequested(int index, const QString &newMessage);

private slots:
    void onSearchTextChanged(const QString &text);

private:
    Ui::AllFeedbackWindow *ui;
    QVector<Feedback> m_feedbackList; // Local copy of feedback items.
    QString m_currentEmail;           // Email of the current user.
    bool m_isAdmin;                   // True if the current user is admin.
};

#endif // ALLFEEDBACKWINDOW_H
