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

void MainWindow::onAcceptClicked()
{
    QString emailInput = ui->emailLineEdit->text().trimmed();
    QString usernameInput = ui->usernameLineEdit->text().trimmed();

    if (emailInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter an email address first.");
        return;
    }
    if (!emailInput.contains("@gmail.com", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Warning", " Invalid Email address");
        return;
    }
    if (usernameInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a username.");
        return;
    }

    // If no credentials are set, accept this new combination.
    if (currentEmail.isEmpty() && currentUsername.isEmpty()) {
        currentEmail = emailInput;
        currentUsername = usernameInput;
        // Check if the accepted credentials are admin credentials.
        if (currentEmail.compare("lorenzo@gmail.com", Qt::CaseInsensitive) == 0 &&
            currentUsername.compare("lorenzo", Qt::CaseInsensitive) == 0) {
            QMessageBox::information(this, "Admin", "Signed in as an Admin");
        } else {
            QMessageBox::information(this, "Info", "Email and username accepted. You may now continue.");
        }
        return;
    }

    // If the new combination exactly matches the current one, inform the user.
    if (currentEmail.compare(emailInput, Qt::CaseInsensitive) == 0 &&
        currentUsername.compare(usernameInput, Qt::CaseInsensitive) == 0) {
        QMessageBox::information(this, "Info", "The credentials you entered are already accepted.");
        return;
    }

    // If only one field is changed, reject the change.
    bool emailChanged = (currentEmail.compare(emailInput, Qt::CaseInsensitive) != 0);
    bool usernameChanged = (currentUsername.compare(usernameInput, Qt::CaseInsensitive) != 0);
    if (emailChanged ^ usernameChanged) {
        QMessageBox::warning(this, "Warning", "email or username already used.");
        return;
    }

    // Both fields are different: accept the new combination.
    currentEmail = emailInput;
    currentUsername = usernameInput;
    // Check for admin credentials on new combination.
    if (currentEmail.compare("lorenzo@gmail.com", Qt::CaseInsensitive) == 0 &&
        currentUsername.compare("lorenzo", Qt::CaseInsensitive) == 0) {
        QMessageBox::information(this, "Admin", "Signed in as an admin");
    } else {
        QMessageBox::information(this, "Info", "Credentials accepted. You may now continue.");
    }
}

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
    fb.resolved = false; // New feedback is unresolved.
    m_feedbackList.append(fb);
    ui->feedbackLineEdit->clear();
    QMessageBox::information(this, "Success", "Feedback submitted successfully!");
}

void MainWindow::onViewNegativeClicked()
{
    QVector<Feedback> negatives;
    for (const Feedback &fb : m_feedbackList) {
        if (fb.negative) {
            negatives.append(fb);
        }
    }
    bool isAdmin = (currentEmail.compare("lorenzo@gmail.com", Qt::CaseInsensitive) == 0 &&
                    currentUsername.compare("lorenzo", Qt::CaseInsensitive) == 0);
    if (negatives.isEmpty()) {
        QMessageBox::information(this, "Negative Feedback", "No negative feedback found.");
    } else {
        NegativeFeedbackWindow dialog(negatives, isAdmin, this);
        dialog.exec();
    }
}

void MainWindow::onViewAllClicked()
{
    if (m_feedbackList.isEmpty()) {
        QMessageBox::information(this, "All Feedback", "No feedback available.");
        return;
    }

    // Determine if the current user is admin.
    bool isAdmin = (currentEmail.compare("lorenzo@gmail.com", Qt::CaseInsensitive) == 0 &&
                    currentUsername.compare("lorenzo", Qt::CaseInsensitive) == 0);

    // Launch AllFeedbackWindow and pass the admin flag.
    AllFeedbackWindow *dialog = new AllFeedbackWindow(m_feedbackList, currentEmail, isAdmin, this);

    // Connect delete signal.
    connect(dialog, &AllFeedbackWindow::deleteFeedbackRequested, this, [this, dialog](int index) {
        if (index >= 0 && index < m_feedbackList.size()) {
            m_feedbackList.removeAt(index);
            dialog->setFeedbackList(m_feedbackList);
        }
    });

    // Connect edit signal.
    connect(dialog, &AllFeedbackWindow::editFeedbackRequested, this, [this, dialog](int index, const QString &newMessage) {
        if (index >= 0 && index < m_feedbackList.size()) {
            m_feedbackList[index].message = newMessage;
            dialog->setFeedbackList(m_feedbackList);
        }
    });

    dialog->exec();
    delete dialog;
}

bool MainWindow::isNegative(const QString &msg)
{
    static QStringList negativeWords = {
        "not working", "not responding", "fails to start", "not functioning", "inoperable",
        "non-functional", "won't turn on", "not powering up", "power failure",

        // Hardware issues
        "malfunctioning", "not fine", "broken", "defective", "unreliable", "unstable",
        "broken down", "hardware failure", "electrical issue", "short-circuit",
        "defective components", "poor build quality", "bad", "not good",

        // Software issues
        "crashing", "freezing", "unstable software", "software crash", "firmware error",
        "driver issue", "error-prone", "buggy", "glitchy", "laggy", "slow response", "delay", "lag",

        // Overheating & power
        "overheating", "excessively hot", "battery not charging", "battery dying", "battery not lasting",

        // Connectivity and interface issues
        "unresponsive", "unresponsive interface", "control failure", "touchscreen unresponsive",
        "poor connectivity", "unstable connection", "no signal", "weak signal",

        // Quality & performance adjectives
        "poor performance", "substandard", "unsatisfactory", "inefficient", "underperforming",
        "mediocre", "flawed", "deficient", "insufficient", "inadequate", "problematic",

        // Additional product issue descriptors (more specific to products)
        "stained", "torn", "faded", "frayed", "worn-out", "shabby", "dirty", "mildewed",
        "ripped", "stretched", "bleached", "shrunk", "distorted", "cheap", "garish",
        "smeared", "soiled", "smudged", "ragged", "mottled", "threadbare", "dingy",
        "discolored", "tattered", "creased", "wrinkled", "pilled", "crumpled", "dusty",
        "scratchy", "rough", "flimsy",

        // Food or consumable-related (if applicable, for product issues in food items)
        "spoiled", "stale", "rotten", "disgusting", "bland", "overcooked", "undercooked",
        "sour", "bitter", "salty", "greasy", "burnt", "soggy", "mushy", "rancid", "off",
        "unpalatable", "unappetizing", "smelly", "moldy", "icky", "gross", "watery",
        "insipid", "flavorless", "unseasoned", "chewy", "tough", "fibrous", "slimy",
        "oily", "over-salted", "under-salted", "tasteless", "charred", "pungent", "acrid",
        "stinky", "foul", "nasty", "vile", "offensive", "cloying", "unfresh", "sourish",
        "dull", "flat", "foul-tasting", "pasty", "grimy", "damp", "unsavory", "off-putting",
        "leaden", "unsatisfactory", "horrid",

        // More generic product issues
        "not operating", "fails", "failure", "malfunction", "error", "crash", "freeze", "hangs",
        "stuck", "unsteady", "jerky", "unstable performance", "inconsistent"
    };


    for (const QString &word : negativeWords) {
        if (msg.contains(word, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}
