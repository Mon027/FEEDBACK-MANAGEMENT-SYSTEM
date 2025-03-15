#ifndef NEGATIVEFEEDBACKWINDOW_H
#define NEGATIVEFEEDBACKWINDOW_H

#include <QDialog>
#include <QVector>
#include "mainwindow.h"  // For the Feedback struct

namespace Ui {
class NegativeFeedbackWindow;
}

class NegativeFeedbackWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor: receives the negative feedback list (as Feedback objects) and an admin flag.
    explicit NegativeFeedbackWindow(const QVector<Feedback> &negativeFeedbacks, bool isAdmin, QWidget *parent = nullptr);
    ~NegativeFeedbackWindow();

    // Update the local copy and refresh the list.
    void setFeedbackList(const QVector<Feedback> &feedbacks);

private slots:
    void onSearchTextChanged(const QString &text);

private:
    void populateList(const QVector<Feedback> &feedbacks);

    Ui::NegativeFeedbackWindow *ui;
    QVector<Feedback> m_allFeedbacks; // Local copy of feedback items.
    bool m_isAdmin;
signals:
    // New signal to propagate that a feedback item should be marked resolved.
    void markResolvedRequested(int index);
};

#endif // NEGATIVEFEEDBACKWINDOW_H
