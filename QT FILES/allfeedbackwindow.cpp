#include "allfeedbackwindow.h"
#include "ui_allfeedbackwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QLineEdit>

AllFeedbackWindow::AllFeedbackWindow(const QVector<Feedback> &feedbackList, const QString &currentEmail, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AllFeedbackWindow),
    m_feedbackList(feedbackList),
    m_currentEmail(currentEmail)
{
    ui->setupUi(this);
    setWindowTitle("All Feedback");
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

    // Loop over each feedback item.
    for (int i = 0; i < m_feedbackList.size(); ++i) {
        const Feedback &fb = m_feedbackList.at(i);

        // Create a custom widget for the list item.
        QWidget *itemWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Create a label that displays the username and message.
        QLabel *label = new QLabel(QString("Username: %1\nMessage: %2")
                                       .arg(fb.username)
                                       .arg(fb.message));
        layout->addWidget(label);

        // If this feedback belongs to the current user, add Edit and Delete buttons.
        if (fb.email == m_currentEmail) {
            // Create the Edit button.
            QPushButton *editButton = new QPushButton("Edit");
            layout->addWidget(editButton);
            // Create the Delete button.
            QPushButton *deleteButton = new QPushButton("Delete");
            layout->addWidget(deleteButton);

            // Store the current index in both buttons.
            editButton->setProperty("index", i);
            deleteButton->setProperty("index", i);

            // Connect the Delete button.
            connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
                int index = deleteButton->property("index").toInt();
                emit deleteFeedbackRequested(index);
            });

            // Connect the Edit button.
            connect(editButton, &QPushButton::clicked, this, [this, itemWidget, label, editButton]() {
                // Hide the label and Edit button to enter edit mode.
                label->hide();
                editButton->hide();

                // Create a QLineEdit for editing; parent it to itemWidget.
                QLineEdit *editLine = new QLineEdit(itemWidget);
                // Extract the current message from the label.
                QString fullText = label->text();  // Format: "Username: ...\nMessage: ..."
                QString currentMsg = fullText.section("\nMessage: ", 1, 1);
                editLine->setText(currentMsg);

                // Create Save and Cancel buttons.
                QPushButton *saveButton = new QPushButton("Save", itemWidget);
                QPushButton *cancelButton = new QPushButton("Cancel", itemWidget);

                // Get the layout from the item widget.
                QHBoxLayout *layout = qobject_cast<QHBoxLayout*>(itemWidget->layout());
                if (layout) {
                    layout->addWidget(editLine);
                    layout->addWidget(saveButton);
                    layout->addWidget(cancelButton);
                    layout->activate();  // Force the layout to update immediately.
                }

                // Connect the Save button.
                int index = editButton->property("index").toInt();
                connect(saveButton, &QPushButton::clicked, this, [this, editLine, label, editButton, saveButton, cancelButton, index]() {
                    QString newMessage = editLine->text().trimmed();
                    emit editFeedbackRequested(index, newMessage);
                    // Clean up the edit widgets.
                    editLine->deleteLater();
                    saveButton->deleteLater();
                    cancelButton->deleteLater();
                    // Update the label text with the new message.
                    label->setText(QString("Username: %1\nMessage: %2")
                                       .arg(m_feedbackList.at(index).username)
                                       .arg(newMessage));
                    label->show();
                    editButton->show();
                });

                // Connect the Cancel button.
                connect(cancelButton, &QPushButton::clicked, this, [this, editLine, label, editButton, saveButton, cancelButton]() {
                    editLine->deleteLater();
                    saveButton->deleteLater();
                    cancelButton->deleteLater();
                    label->show();
                    editButton->show();
                });
            });
        }

        // Create a new QListWidgetItem and set the custom widget.
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(itemWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, itemWidget);
    }
}
