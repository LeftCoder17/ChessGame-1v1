#include "chessboardwidget.h"
#include "piece.h"


ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent)
{
    m_highlightedSquares.resize(64, false);
    m_endGameButton = new QPushButton("End Game", this);
    connect(m_endGameButton, &QPushButton::clicked, this, &ChessBoardWidget::endGame);
}

ChessBoardWidget::~ChessBoardWidget()
{
    delete m_endGameButton;
}

void ChessBoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawBoard(painter);
    drawPieces(painter);
    drawCapturedPieces(painter);
    drawPlayerLabels(painter);
    drawTurnLabel(painter);
    drawEndGameButton();
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event)
{
    QSize centralWidgetSize = parentWidget()->size();
    int boardSize = qMin(centralWidgetSize.width(), centralWidgetSize.height()) * 0.8;
    int startX = centralWidgetSize.width() * 0.1;
    int startY = centralWidgetSize.height() * 0.1;
    int squareSize = boardSize / 8;

    int col = (event->pos().x() - startX) / squareSize;
    int row = (event->pos().y() - startY) / squareSize;

    // Ensure the click was within the bounds of the chessboard
    int selectedSquare;
    if (col >= 0 && col < 8 && row >= 0 && row < 8)
    {
        selectedSquare = row * 8 + col;
    }
    else
    {
        return;
    }

    Position* currentPosition = m_gameBoard.get_position();

    if (m_highlightedSquares[selectedSquare])
    {
        m_gameBoard.add_position_historial(currentPosition);
        Position* newPosition = new Position;
        *newPosition = *currentPosition;
        newPosition->move_piece(currentSelectedSquare, selectedSquare);
        newPosition->m_inCheck = false;

        uint8_t movingPiece = newPosition->m_squares[selectedSquare];
        if ((movingPiece & type_mask) == pawn)
        {
            bool isWhite = (movingPiece & color_mask) == white;
            int promotionRow = isWhite ? 0 : 7;

            if ((selectedSquare / 8) == promotionRow)
            {
                PromotionDialog dialog(isWhite, this);
                if (dialog.exec() == QDialog::Accepted)
                {
                    newPosition->m_squares[selectedSquare] = (isWhite ? white : black) | dialog.selectedPieceType();
                    switch (newPosition->m_squares[selectedSquare] & type_mask)
                    {
                        case knight: isWhite ? newPosition->m_whiteCaptured.promotionPoints += 2 : newPosition->m_blackCaptured.promotionPoints += 2; break;
                        case bishop: isWhite ? newPosition->m_whiteCaptured.promotionPoints += 2 : newPosition->m_blackCaptured.promotionPoints += 2; break;
                        case rook: isWhite ? newPosition->m_whiteCaptured.promotionPoints += 4 : newPosition->m_blackCaptured.promotionPoints += 4; break;
                        case queen: isWhite ? newPosition->m_whiteCaptured.promotionPoints += 8 : newPosition->m_blackCaptured.promotionPoints += 8; break;
                        default: break;
                    }
                }
            }
        }

        for (int sq = 0; sq < 64; sq++)
        {
            newPosition->m_lastLegalMoves[sq] = newPosition->m_legalMoves[sq];
        }
        newPosition->compute_legal_moves();
        m_gameBoard.set_position(newPosition);
        currentSelectedSquare = -1;
        m_highlightedSquares.fill(false);
        update();

        if (newPosition->m_mate)
        {
            QString resultText = newPosition->m_whiteTurn ? "Checkmate! Black wins!" : "Checkmate! White wins!";
            ResultDialog dialog(resultText, this);
            dialog.exec();
        }
        else if (newPosition->m_staleMate)
        {
            QString resultText = "Stalemate! It's a draw!";
            ResultDialog dialog(resultText, this);
            dialog.exec();
        }
    }
    else
    {
        m_highlightedSquares.fill(false);
        for (int sq = 0; sq < 64; sq++)
        {
            if (get_nth_bit(currentPosition->m_legalMoves[selectedSquare], sq))
            {
                m_highlightedSquares[sq] = true;
            }
        }

        currentSelectedSquare = selectedSquare;
        update();
    }
    return;
}

void ChessBoardWidget::drawBoard(QPainter &painter)
{
    QColor lightSquareColor(240, 217, 181);
    QColor darkSquareColor(181, 136, 99);

    // Calculate the size of the board based on the central widget's dimensions
    QSize centralWidgetSize = parentWidget()->size();
    int boardSize = qMin(centralWidgetSize.width(), centralWidgetSize.height()) * 0.8;
    int startX = centralWidgetSize.width() * 0.1;
    int startY = centralWidgetSize.height() * 0.1;

    int squareSize = boardSize / 8;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            QRect squareRect(startX + col * squareSize, startY + row * squareSize, squareSize, squareSize);
            if ((row + col) % 2) {
                painter.fillRect(squareRect, darkSquareColor);
            } else {
                painter.fillRect(squareRect, lightSquareColor);
            }
        }
    }
    return;
}


void ChessBoardWidget::drawPieces(QPainter &painter)
{
    // Calculate board dimensions and offsets
    QSize centralWidgetSize = parentWidget()->size();
    int boardSize = qMin(centralWidgetSize.width(), centralWidgetSize.height()) * 0.8;
    int startX = centralWidgetSize.width() * 0.1;
    int startY = centralWidgetSize.height() * 0.1;
    int squareSize = boardSize / 8;

    Position* currentPosition = m_gameBoard.get_position();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int sq = row * 8 + col;
            uint8_t piece = currentPosition->m_squares[sq];

            // Calculate coordinates for the square
            int x = startX + col * squareSize;
            int y = startY + row * squareSize;
            QRect squareRect(x, y, squareSize, squareSize);

            if (piece != none) {
                QString fileName;
                if ((piece & color_mask) == white) {
                    switch (piece & type_mask) {
                        case pawn: fileName = ":/libs/images/w_pawn.svg"; break;
                        case knight: fileName = ":/libs/images/w_knight.svg"; break;
                        case bishop: fileName = ":/libs/images/w_bishop.svg"; break;
                        case rook: fileName = ":/libs/images/w_rook.svg"; break;
                        case queen: fileName = ":/libs/images/w_queen.svg"; break;
                        case king: fileName = ":/libs/images/w_king.svg"; break;
                        default: continue;
                    }
                } else {
                    switch (piece & type_mask) {
                        case pawn: fileName = ":/libs/images/b_pawn.svg"; break;
                        case knight: fileName = ":/libs/images/b_knight.svg"; break;
                        case bishop: fileName = ":/libs/images/b_bishop.svg"; break;
                        case rook: fileName = ":/libs/images/b_rook.svg"; break;
                        case queen: fileName = ":/libs/images/b_queen.svg"; break;
                        case king: fileName = ":/libs/images/b_king.svg"; break;
                        default: continue;
                    }
                }

                if (!fileName.isEmpty()) {
                    QSvgRenderer renderer(fileName);
                    if ((piece == (black | knight)) || (piece == (black | king)) || ((piece & type_mask) == queen)) {
                        QRectF scaledRect(
                            x + squareSize * 0.05,
                            y + squareSize * 0.05, 
                            squareSize * 0.9, 
                            squareSize * 0.9
                        );
                        renderer.render(&painter, scaledRect);
                    }
                    else if (piece == (white | bishop))
                    {
                        QRectF scaledRect(
                            x - squareSize * 0.05,
                            y - squareSize * 0.05, 
                            squareSize * 1.1, 
                            squareSize * 1.1
                        );
                        renderer.render(&painter, scaledRect);
                    }
                    else

                    {
                        renderer.render(&painter, squareRect);
                    }
                }
            }

            if (m_highlightedSquares[sq]) {
                painter.fillRect(squareRect, QColor(255, 165, 0, 128)); // Orange with transparency
            }
        }
    }
    return;
}

void ChessBoardWidget::drawPlayerLabels(QPainter &painter)
{
    // 1. Calculate board dimensions and offsets
    QSize widgetSize = size();
    int boardSize = qMin(widgetSize.width(), widgetSize.height()) * 0.8;
    int startX = widgetSize.width() * 0.1;
    int startYPlayer1 = widgetSize.height() * 0.95;
    int startYPlayer2 = widgetSize.height() * 0.05;
    int labelWidth = widgetSize.width() * 0.9;  // Match the board width
    int labelHeight = widgetSize.height() * 0.04;

    // 2. Set up labels
    QRect player1Rect(startX, startYPlayer1, labelWidth, labelHeight);
    QRect player2Rect(startX, startYPlayer2, labelWidth, labelHeight);

    // 2.1. Set up label for Player 1
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(player1Rect);
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(player1Rect, Qt::AlignCenter, "Player 1");

    // 2.2. Set up label for Player 2
    painter.setBrush(Qt::black);
    painter.setPen(Qt::white);
    painter.drawRect(player2Rect);
    painter.drawText(player2Rect, Qt::AlignCenter, "Player 2");

    return;
}

void ChessBoardWidget::drawTurnLabel(QPainter &painter)
{
    // 1. Calculate board dimensions and offsets
    QSize widgetSize = size();
    int boardSize = qMin(widgetSize.width(), widgetSize.height()) * 0.8;
    int startX = widgetSize.width() * 0.01;
    int startY = widgetSize.height() * 0.4;
    int labelWidth = widgetSize.width() * 0.08;
    int labelHeight = widgetSize.width() * 0.2;

    // 2. Determine the current player's turn
    bool isWhiteTurn = m_gameBoard.get_position()->m_whiteTurn ? true : false;

    // 3. Set up turn label
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(isWhiteTurn ? Qt::white : Qt::black);
    painter.setPen(Qt::black);
    painter.drawRect(startX, startY, labelWidth, labelHeight);

    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(isWhiteTurn ? Qt::black : Qt::white);

    painter.translate(startX + labelWidth / 2, startY + labelHeight / 2);
    painter.rotate(-90);

    QRect textRect(-labelHeight / 2, -labelWidth / 2, labelHeight, labelWidth);
    painter.drawText(textRect, Qt::AlignCenter, isWhiteTurn ? "White's Turn" : "Black's Turn");

    painter.rotate(90);
    painter.translate(-startX - labelWidth / 2, -startY - labelHeight / 2);
}


void ChessBoardWidget::drawCapturedPieces(QPainter &painter)
{
    // 1. Define the piece values
    const std::map<int, int> pieceValues = {
        {pawn, 1}, {knight, 3}, {bishop, 3}, {rook, 5}, {queen, 9}
    };

    // 2. Calculate the size of the board based on the central widget's dimensions
    QSize centralWidgetSize = parentWidget()->size();
    int boardSize = qMin(centralWidgetSize.width(), centralWidgetSize.height()) * 0.8;
    int startX = centralWidgetSize.width() * 0.1;
    int startY = centralWidgetSize.height() * 0.1;
    int squareSize = boardSize / 8;

    int capturedAreaStartX = startX + boardSize + 20;
    int capturedAreaWidth = centralWidgetSize.width() - capturedAreaStartX - 20;
    int capturedAreaHeight = boardSize;
    int capturedPieceSize = squareSize / 2;

    int blackStartY = startY;
    int whiteStartY = startY + boardSize / 2 + 20;
    int pieceSpacing = 5;

    // 3. Draw boxes for captured pieces
    QRect blackBox(capturedAreaStartX, blackStartY, capturedAreaWidth, capturedAreaHeight / 2 - 20);
    QRect whiteBox(capturedAreaStartX, whiteStartY, capturedAreaWidth, capturedAreaHeight / 2 - 20);

    // Use softer colors for the boxes
    QColor darkerGray(160, 160, 160);
    QColor lightGray(230, 230, 230);

    painter.setBrush(darkerGray); // Soft gray for the black player's box
    painter.drawRect(blackBox);
    painter.setBrush(lightGray); // Light gray for the white player's box
    painter.drawRect(whiteBox);

    // 4. Draw captured pieces for white
    int currentX = capturedAreaStartX;
    int currentY = whiteStartY;
    for (int i = 0; i < m_gameBoard.get_position()->m_whiteCaptured.pawns; ++i)
    {
        QString filePath = ":/libs/images/b_pawn.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
        if (i == 4) 
        {
            currentX = capturedAreaStartX;
            currentY += capturedPieceSize + pieceSpacing;
        }
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_whiteCaptured.knights; ++i)
    {
        QString filePath = ":/libs/images/b_knight.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_whiteCaptured.bishops; ++i)
    {
        QString filePath = ":/libs/images/b_bishop.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_whiteCaptured.rooks; ++i)
    {
        QString filePath = ":/libs/images/b_rook.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_whiteCaptured.queens; ++i)
    {
        QString filePath = ":/libs/images/b_queen.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }

    // 5. Draw captured pieces for black
    currentX = capturedAreaStartX;
    currentY = blackStartY;
    for (int i = 0; i < m_gameBoard.get_position()->m_blackCaptured.pawns; ++i)
    {
        QString filePath = ":/libs/images/w_pawn.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
        if (i == 4) 
        {
            currentX = capturedAreaStartX;
            currentY += capturedPieceSize + pieceSpacing;
        }
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_blackCaptured.knights; ++i)
    {
        QString filePath = ":/libs/images/w_knight.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_blackCaptured.bishops; ++i)
    {
        QString filePath = ":/libs/images/w_bishop.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_blackCaptured.rooks; ++i)
    {
        QString filePath = ":/libs/images/w_rook.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }
    currentX = capturedAreaStartX;
    currentY += capturedPieceSize + pieceSpacing;
    for (int i = 0; i < m_gameBoard.get_position()->m_blackCaptured.queens; ++i)
    {
        QString filePath = ":/libs/images/w_queen.svg";
        QSvgRenderer renderer(filePath);
        renderer.render(&painter, QRect(currentX, currentY, capturedPieceSize, capturedPieceSize));
        currentX += capturedPieceSize + pieceSpacing;
    }

    // 6. Compute punctuation
    int blackScore = m_gameBoard.get_position()->m_blackCaptured.pawns * pieceValues.at(pawn) +
                     m_gameBoard.get_position()->m_blackCaptured.knights * pieceValues.at(knight) +
                     m_gameBoard.get_position()->m_blackCaptured.bishops * pieceValues.at(bishop) +
                     m_gameBoard.get_position()->m_blackCaptured.rooks * pieceValues.at(rook) +
                     m_gameBoard.get_position()->m_blackCaptured.queens * pieceValues.at(queen) +
                     m_gameBoard.get_position()->m_blackCaptured.promotionPoints;

    int whiteScore = m_gameBoard.get_position()->m_whiteCaptured.pawns * pieceValues.at(pawn) +
                     m_gameBoard.get_position()->m_whiteCaptured.knights * pieceValues.at(knight) +
                     m_gameBoard.get_position()->m_whiteCaptured.bishops * pieceValues.at(bishop) +
                     m_gameBoard.get_position()->m_whiteCaptured.rooks * pieceValues.at(rook) +
                     m_gameBoard.get_position()->m_whiteCaptured.queens * pieceValues.at(queen) +
                     m_gameBoard.get_position()->m_whiteCaptured.promotionPoints;

    int punctuation = whiteScore - blackScore;
    QString sign = punctuation >= 0 ? "+" : "-";

    // 7. Draw the punctuation label
    QRect punctuationRect(capturedAreaStartX, startY + boardSize / 2 - 10, capturedAreaWidth, 20);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(punctuationRect, Qt::AlignCenter, QString("Punctuation: %1 %2").arg(sign).arg(abs(punctuation)));
}


void ChessBoardWidget::drawEndGameButton()
{
    // 1. Calculate board dimensions and offsets
    QSize widgetSize = size();
    int boardSize = qMin(widgetSize.width(), widgetSize.height()) * 0.8;
    int startX = widgetSize.width() * 0.01;
    int startY = widgetSize.height() * 0.8;
    int buttonWidth = widgetSize.width() * 0.08;
    int buttonHeight = widgetSize.width() * 0.05;

    // 2. Set up button style
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #dc3545;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c82333;"
        "}";

    m_endGameButton->setStyleSheet(buttonStyle);
    m_endGameButton->setGeometry(startX, startY, buttonWidth, buttonHeight);
    m_endGameButton->setText("End Game");
    m_endGameButton->show();
}


void ChessBoardWidget::endGame()
{
    emit signalEndGame();
}
