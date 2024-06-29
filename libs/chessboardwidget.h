#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QSvgRenderer>
#include <QPushButton>
#include <QLabel>

#include "chessboard.h"
#include "promotiondialog.h"
#include "resultdialog.h"

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit ChessBoardWidget(QWidget *parent = nullptr);

    // Destructor
    ~ChessBoardWidget();

protected:
    // It will be called whenever the widget needs to be repaired
    void paintEvent(QPaintEvent *event) override;

    // Called when press mouse
    void mousePressEvent(QMouseEvent *event) override;

private:
    // Draws the board
    void drawBoard(QPainter &painter);

    // Draws the pieces
    void drawPieces(QPainter &painter);

    // Draws the captures pieces
    void drawCapturedPieces(QPainter &painter);

    // Draws the Player Labels
    void drawPlayerLabels(QPainter &painter);

    // Draws the turn labels
    void drawTurnLabel(QPainter &painter);

    // Puts the end game button
    void drawEndGameButton();


public:  // Public members
    ChessBoard m_gameBoard {};
    int currentSelectedSquare {-1};
    QVector<bool> m_highlightedSquares {};
    QPushButton *m_endGameButton {nullptr};

signals:
    void signalEndGame();

private slots:
    void endGame();
};




#endif