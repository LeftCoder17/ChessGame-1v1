#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(const QString &resultText, QWidget *parent = nullptr);

private slots:
    void onExitButtonClicked();

private:
    QLabel *m_resultLabel;
    QPushButton *m_exitButton;
};

#endif // RESULTDIALOG_H
