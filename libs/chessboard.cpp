#include "chessboard.h"
#include "piece.h"
#include <iostream>


ChessBoard::~ChessBoard()
{
    for (Position* position : m_historicalPositions)
    {
        delete position;
    }
}

void ChessBoard::init_position()
{
    std::string initialFen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    m_actualPosition = new Position(initialFen);
    for (int sq = 0; sq < 64; sq++)
    {
        m_actualPosition->m_lastLegalMoves[sq] = 0;
    }
    m_actualPosition->compute_legal_moves();

    return;
}

void ChessBoard::add_position_historial(Position* addedPosition)
{
    m_historicalPositions.push_back(addedPosition);
    return;
}

void ChessBoard::set_position(Position* newPosition)
{
    m_actualPosition = newPosition;
}

Position* ChessBoard::get_position() {
    return m_actualPosition;
}
