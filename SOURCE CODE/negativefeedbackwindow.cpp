#include "negativefeedbackwindow.h"
#include "ui_negativefeedbackwindow.h"
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

NegativeFeedbackWindow::NegativeFeedbackWindow(const QVector<Feedback> &negativeFeedbacks, bool isAdmin, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::NegativeFeedbackWindow),
    m_allFeedbacks(negativeFeedbacks),
    m_isAdmin(isAdmin)
{
    ui->setupUi(this);
    setWindowTitle("Negative Feedback");
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &NegativeFeedbackWindow::onSearchTextChanged);
    populateList(m_allFeedbacks);
}

NegativeFeedbackWindow::~NegativeFeedbackWindow()
{
    delete ui;
}

void NegativeFeedbackWindow::setFeedbackList(const QVector<Feedback> &feedbacks)
{
    m_allFeedbacks = feedbacks;
    populateList(m_allFeedbacks);
}

void NegativeFeedbackWindow::populateList(const QVector<Feedback> &feedbacks)
{
    ui->listWidget->clear();
    for (int i = 0; i < feedbacks.size(); ++i) {
        const Feedback &fb = feedbacks.at(i);
        QWidget *itemWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Build label text; append " (RESOLVED)" if applicable.
        QString labelText = QString("Username: %1\nMessage: %2")
                                .arg(fb.username)
                                .arg(fb.message);
        if (fb.resolved)
            labelText.append(" (RESOLVED)");
        QLabel *label = new QLabel(labelText);
        layout->addWidget(label);

        // If admin and feedback is not resolved, add a "Mark Resolved" button.
        if (m_isAdmin && !fb.resolved) {
            QPushButton *markResolvedButton = new QPushButton("Mark Resolved");
            layout->addWidget(markResolvedButton);
            markResolvedButton->setProperty("index", i);
            connect(markResolvedButton, &QPushButton::clicked, this, [this, markResolvedButton]() {
                int index = markResolvedButton->property("index").toInt();
                // Instead of updating m_allFeedbacks directly, emit a signal.
                emit markResolvedRequested(index);
                // Optionally update local copy here if desired.
                m_allFeedbacks[index].resolved = true;
                populateList(m_allFeedbacks);
            });
        }

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(itemWidget->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, itemWidget);
    }
}

void NegativeFeedbackWindow::onSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        populateList(m_allFeedbacks);
        return;
    }
    QVector<Feedback> filtered;
    for (const Feedback &fb : m_allFeedbacks) {
        QString combined = QString("Username: %1\nMessage: %2").arg(fb.username).arg(fb.message);
        if (combined.contains(text, Qt::CaseInsensitive))
            filtered.append(fb);
    }
    populateList(filtered);
}
