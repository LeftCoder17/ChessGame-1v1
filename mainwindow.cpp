#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QFont>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_chessBoard(nullptr)
{
    // Set window title
    setWindowTitle("CHESS GAME");

    // Resize the main window
    resize(1000, 800);

    // Central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_menuWidget = new QWidget(m_centralWidget);

    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_menuLayout = new QVBoxLayout(m_menuWidget);

    QLabel *titleLabel = new QLabel("Welcome to Chess Game", m_menuWidget);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *imageLabel = new QLabel(m_menuWidget);
    QPixmap image(":/libs/images/portada.jpg");
    imageLabel->setPixmap(image.scaled(600, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    m_startButton = new QPushButton("Start Game", m_menuWidget);
    m_startButton->setFixedSize(250, 60);
    m_startButton->setFont(QFont("Arial", 20, QFont::Bold));
    m_startButton->setStyleSheet(
        "QPushButton {"
        "background-color: #4CAF50;"
        "color: white;"
        "border: none;"
        "border-radius: 12px;"
        "padding: 12px 24px;"
        "text-align: center;"
        "text-decoration: none;"
        "font-size: 16px;"
        "margin: 4px 2px;"
        "cursor: pointer;"
        "}"
        "QPushButton:hover {"
        "background-color: #45a049;"
        "}"
    );

    QSpacerItem *spacer = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_menuLayout->addWidget(titleLabel);
    m_menuLayout->addWidget(imageLabel);
    m_menuLayout->addSpacerItem(spacer);
    m_menuLayout->addWidget(m_startButton, 0, Qt::AlignCenter);
    m_menuWidget->setLayout(m_menuLayout);

    m_mainLayout->addWidget(m_menuWidget);
    m_centralWidget->setLayout(m_mainLayout);

    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startGame);
}

MainWindow::~MainWindow()
{
    delete m_centralWidget;
    if (m_chessBoard) delete m_chessBoard;
}

void MainWindow::startGame()
{
    m_menuWidget->hide();
    m_chessBoard = new ChessBoardWidget(m_centralWidget);
    connect(m_chessBoard, &ChessBoardWidget::signalEndGame, this, &MainWindow::handleEndGame);
    m_chessBoard->setGeometry(0, 0, m_centralWidget->width(), m_centralWidget->height());
    m_mainLayout->addWidget(m_chessBoard);
    m_chessBoard->m_gameBoard.init_position();
    m_chessBoard->show();
}

void MainWindow::handleEndGame()
{
    m_chessBoard->hide();
    m_mainLayout->removeWidget(m_chessBoard);
    delete m_chessBoard->m_gameBoard.get_position();
    delete m_chessBoard;
    m_chessBoard = nullptr;
    m_menuWidget->show();
}