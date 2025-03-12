#include "negativefeedbackwindow.h"
#include "ui_negativefeedbackwindow.h"

NegativeFeedbackWindow::NegativeFeedbackWindow(const QVector<QString> &negativeFeedbacks, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::NegativeFeedbackWindow)
{
    ui->setupUi(this);
    setWindowTitle("Negative Feedback");
    // Populate the list widget.
    for (const QString &feedback : negativeFeedbacks) {
        ui->listWidget->addItem(feedback);
    }
}

NegativeFeedbackWindow::~NegativeFeedbackWindow()
{
    delete ui;
}
