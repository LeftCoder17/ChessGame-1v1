#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "libs/chessboardwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleEndGame();

private:
    QWidget *m_centralWidget;
    QWidget *m_menuWidget;
    QPushButton *m_startButton;
    ChessBoardWidget *m_chessBoard;
    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_menuLayout;

private slots:
    void startGame();
};

#endif
