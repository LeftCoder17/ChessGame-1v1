#include "resultdialog.h"

#include <QFont>
#include <QPalette>

ResultDialog::ResultDialog(const QString &resultText, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Game Over");

    // 1. Create and style the result label
    m_resultLabel = new QLabel(resultText);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont("Arial", 18, QFont::Bold);
    m_resultLabel->setFont(labelFont);
    m_resultLabel->setStyleSheet("color: #333333");

    // 2. Create and style the exit button
    m_exitButton = new QPushButton("Exit");
    m_exitButton->setStyleSheet("background-color: #007ACC; color: white; padding: 10px 20px; border-radius: 5px;");
    connect(m_exitButton, &QPushButton::clicked, this, &ResultDialog::onExitButtonClicked);

    // 3. Create layout and add widgets
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_resultLabel);
    layout->addWidget(m_exitButton);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(50, 50, 50, 50);

    setLayout(layout);
    setFixedSize(400, 250);
}

void ResultDialog::onExitButtonClicked()
{
    accept();
}
