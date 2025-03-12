#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allfeedbackwindow.h"
#include "negativefeedbackwindow.h"

#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Feedback Manager");

    // Connect buttons to their slots.
    connect(ui->acceptButton, &QPushButton::clicked, this, &MainWindow::onAcceptClicked);
    connect(ui->submitFeedbackButton, &QPushButton::clicked, this, &MainWindow::onSubmitFeedbackClicked);
    connect(ui->viewNegativeButton, &QPushButton::clicked, this, &MainWindow::onViewNegativeClicked);
    connect(ui->viewAllButton, &QPushButton::clicked, this, &MainWindow::onViewAllClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Updated slot: Accept the user's email and username.
// Email must include "@gmail" (case-insensitive).
// If credentials already exist, the new combination is accepted only if BOTH email and username are different.
void MainWindow::onAcceptClicked()
{
    QString emailInput = ui->emailLineEdit->text().trimmed();
    QString usernameInput = ui->usernameLineEdit->text().trimmed();

    if (emailInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter an email address first.");
        return;
    }
    if (!emailInput.contains("@gmail.com", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Warning", "Invalid Email address.");
        return;
    }
    if (usernameInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a username.");
        return;
    }

    // If no credentials are currently accepted, accept this new combination.
    if (currentEmail.isEmpty() && currentUsername.isEmpty()) {
        currentEmail = emailInput;
        currentUsername = usernameInput;
        QMessageBox::information(this, "Info", "Email and username accepted. You may now continue.");
        return;
    }

    // If the new combination exactly matches the current one, inform the user.
    if (currentEmail.compare(emailInput, Qt::CaseInsensitive) == 0 &&
        currentUsername.compare(usernameInput, Qt::CaseInsensitive) == 0) {
        QMessageBox::information(this, "Info", "Email and username accepted. You may now continue.");
        return;
    }

    // If only one of the fields is different, reject the change.
    bool emailChanged = (currentEmail.compare(emailInput, Qt::CaseInsensitive) != 0);
    bool usernameChanged = (currentUsername.compare(usernameInput, Qt::CaseInsensitive) != 0);
    if (emailChanged ^ usernameChanged) { // XOR: only one field changed
        QMessageBox::warning(this, "Warning", " Email or Username already used.");
        return;
    }

    // Both fields are different: accept the new combination.
    currentEmail = emailInput;
    currentUsername = usernameInput;
    QMessageBox::information(this, "Info", "Email and username accepted. You may now continue.");
}

// Slot: Submit feedback using the accepted email and username.
void MainWindow::onSubmitFeedbackClicked()
{
    if (currentEmail.isEmpty() || currentUsername.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please accept an email and username first (click ACCEPT AND CONTINUE).");
        return;
    }
    QString message = ui->feedbackLineEdit->text().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a feedback message.");
        return;
    }
    Feedback fb;
    fb.email = currentEmail;
    fb.username = currentUsername;
    fb.message = message;
    fb.negative = isNegative(message);
    m_feedbackList.append(fb);
    ui->feedbackLineEdit->clear();
    QMessageBox::information(this, "Success", "Feedback submitted successfully!");
}

// Slot: View negative feedback in NegativeFeedbackWindow.
void MainWindow::onViewNegativeClicked()
{
    QVector<QString> negatives;
    for (const Feedback &fb : m_feedbackList) {
        if (fb.negative) {
            QString combined = QString("Username: %1\nMessage: %2")
            .arg(fb.username)
                .arg(fb.message);
            negatives.append(combined);
        }
    }
    if (negatives.isEmpty()) {
        QMessageBox::information(this, "Negative Feedback", "No negative feedback found.");
    } else {
        NegativeFeedbackWindow dialog(negatives, this);
        dialog.exec();
    }
}

// Slot: View all feedback in AllFeedbackWindow with edit and delete functions.
void MainWindow::onViewAllClicked()
{
    if (m_feedbackList.isEmpty()) {
        QMessageBox::information(this, "All Feedback", "No feedback available.");
        return;
    }

    AllFeedbackWindow *dialog = new AllFeedbackWindow(m_feedbackList, currentEmail, this);

    // Connect delete signal: remove the item and refresh the dialog.
    connect(dialog, &AllFeedbackWindow::deleteFeedbackRequested, this, [this, dialog](int index) {
        if (index >= 0 && index < m_feedbackList.size()) {
            m_feedbackList.removeAt(index);
            dialog->setFeedbackList(m_feedbackList);
        }
    });

    // Connect edit signal: update the message and refresh the dialog.
    connect(dialog, &AllFeedbackWindow::editFeedbackRequested, this, [this, dialog](int index, const QString &newMessage) {
        if (index >= 0 && index < m_feedbackList.size()) {
            m_feedbackList[index].message = newMessage;
            dialog->setFeedbackList(m_feedbackList);
        }
    });

    dialog->exec();
    delete dialog;
}

// Helper: Check if a message contains any negative words.
bool MainWindow::isNegative(const QString &msg)
{
    static QStringList negativeWords = {
        // Original negative words (12)
        "bad", "terrible", "awful", "broken", "defective", "poor", "unacceptable",
        "slow", "laggy", "overheating", "disappointing", "unresponsive",

        // Cloth-related negative adjectives (32)
        "stained", "torn", "faded", "frayed", "worn-out", "shabby", "dirty", "mildewed",
        "ripped", "stretched", "bleached", "shrunk", "distorted", "cheap", "garish",
        "smeared", "soiled", "smudged", "ragged", "mottled", "threadbare", "dingy",
        "discolored", "tattered", "creased", "wrinkled", "pilled", "crumpled", "dusty",
        "scratchy", "rough", "flimsy",

        // Food-related negative adjectives (53)
        "spoiled", "stale", "rotten", "disgusting", "bland", "overcooked", "undercooked",
        "sour", "bitter", "salty", "greasy", "burnt", "soggy", "mushy", "rancid", "off",
        "unpalatable", "unappetizing", "smelly", "moldy", "icky", "gross", "watery",
        "insipid", "flavorless", "unseasoned", "chewy", "tough", "fibrous", "slimy",
        "oily", "over-salted", "under-salted", "tasteless", "charred", "pungent", "acrid",
        "stinky", "foul", "nasty", "vile", "offensive", "cloying", "unfresh", "sourish",
        "dull", "flat", "foul-tasting", "not good", "grimy", "damp", "unsavory", "off-putting",
        "leaden", "mediocre", "unsatisfactory", "horrid"
    };

    for (const QString &word : negativeWords) {
        if (msg.contains(word, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}
