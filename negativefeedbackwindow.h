// negativefeedbackwindow.h
#ifndef NEGATIVEFEEDBACKWINDOW_H
#define NEGATIVEFEEDBACKWINDOW_H

#include <QDialog>
#include <QVector>
#include <QString>

namespace Ui {
class NegativeFeedbackWindow;
}

class NegativeFeedbackWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NegativeFeedbackWindow(const QVector<QString> &negativeFeedback, QWidget *parent = nullptr);
    ~NegativeFeedbackWindow();

private:
    Ui::NegativeFeedbackWindow *ui;
};

#endif // NEGATIVEFEEDBACKWINDOW_H
