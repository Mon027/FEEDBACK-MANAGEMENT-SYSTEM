#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Feedback struct holds email, username, message, and negative flag.
struct Feedback {
    QString email;
    QString username;
    QString message;
    bool negative;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAcceptClicked();
    void onSubmitFeedbackClicked();
    void onViewNegativeClicked();
    void onViewAllClicked();

private:
    bool isNegative(const QString &msg);

    Ui::MainWindow *ui;
    QString currentEmail;
    QString currentUsername;
    QVector<Feedback> m_feedbackList;
};

#endif // MAINWINDOW_H
