#include "allfeedbackwindow.h"
#include "ui_allfeedbackwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QLineEdit>

AllFeedbackWindow::AllFeedbackWindow(const QVector<Feedback> &feedbackList,
                                     const QString &currentEmail,
                                     bool isAdmin,
                                     QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AllFeedbackWindow),
    m_feedbackList(feedbackList),
    m_currentEmail(currentEmail),
    m_isAdmin(isAdmin)
{
    ui->setupUi(this);
    setWindowTitle("All Feedback");
    // Connect search line edit.
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &AllFeedbackWindow::onSearchTextChanged);
    populateList();
}

AllFeedbackWindow::~AllFeedbackWindow()
{
    delete ui;
}

void AllFeedbackWindow::setFeedbackList(const QVector<Feedback> &list)
{
    m_feedbackList = list;
    populateList();
}

void AllFeedbackWindow::populateList()
{
    ui->listWidget->clear();
    // For search filtering.
    QString searchText = ui->searchLineEdit->text().trimmed();

    for (int i = 0; i < m_feedbackList.size(); ++i) {
        const Feedback &fb = m_feedbackList.at(i);

        // Prepare display text (only username and message).
        QString displayText = QString("Username: %1\nMessage: %2")
                                  .arg(fb.username)
                                  .arg(fb.message);
        if (fb.resolved)
            displayText.append(" (RESOLVED)");

        // Filter by search text if any.
        if (!searchText.isEmpty() && !displayText.contains(searchText, Qt::CaseInsensitive))
            continue;

        QWidget *itemWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        QLabel *label = new QLabel(displayText);
        layout->addWidget(label);

        // Show Delete button if either this feedback belongs to current user OR if admin.
        if ((fb.email == m_currentEmail) || m_isAdmin) {
            QPushButton *deleteButton = new QPushButton("Delete");
            layout->addWidget(deleteButton);
            deleteButton->setProperty("index", i);
            connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
                int index = deleteButton->property("index").toInt();
                emit deleteFeedbackRequested(index);
            });
        }

        // Show Edit button only if the feedback belongs to the current user (not for admin viewing others).
        if (fb.email == m_currentEmail) {
            QPushButton *editButton = new QPushButton("Edit");
            layout->addWidget(editButton);
            editButton->setProperty("index", i);
            connect(editButton, &QPushButton::clicked, this, [this, itemWidget, label, editButton]() {
                label->hide();
                editButton->hide();

                QLineEdit *editLine = new QLineEdit(itemWidget);
                QString fullText = label->text();
                QString currentMsg = fullText.section("\nMessage: ", 1, 1);
                currentMsg = currentMsg.replace(" (RESOLVED)", "");
                editLine->setText(currentMsg);

                QPushButton *saveButton = new QPushButton("Save", itemWidget);
                QPushButton *cancelButton = new QPushButton("Cancel", itemWidget);

                QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(itemWidget->layout());
                if (layout) {
                    layout->addWidget(editLine);
                    layout->addWidget(saveButton);
                    layout->addWidget(cancelButton);
                    layout->activate();
                }

                int index = editButton->property("index").toInt();
                connect(saveButton, &QPushButton::clicked, this, [this, editLine, label, editButton, saveButton, cancelButton, index]() {
                    QString newMessage = editLine->text().trimmed();
                    emit editFeedbackRequested(index, newMessage);
                    editLine->deleteLater();
                    saveButton->deleteLater();
                    cancelButton->deleteLater();
                    label->setText(QString("Username: %1\nMessage: %2")
                                       .arg(m_feedbackList.at(index).username)
                                       .arg(newMessage));
                    label->show();
                    editButton->show();
                });
                connect(cancelButton, &QPushButton::clicked, this, [this, editLine, label, editButton, saveButton, cancelButton]() {
                    editLine->deleteLater();
                    saveButton->deleteLater();
                    cancelButton->deleteLater();
                    label->show();
                    editButton->show();
                });
            });
        }

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(itemWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, itemWidget);
    }
}

void AllFeedbackWindow::onSearchTextChanged(const QString &text)
{
    populateList();
}
