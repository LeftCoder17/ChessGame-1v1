#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "position.h"
#include <vector>

class ChessBoard
{
public:
    // Constructor for ChessBoard
    ChessBoard() = default;

    // Destructor for
    ~ChessBoard();

    // Initialize the starndard initial position
    void init_position();

    // Add the position to the historial
    void add_position_historial(Position* addedPosition);

    // Setters
    void set_position(Position *newPosition);
    // Getters
    Position* get_position();

private: // Members
    Position* m_actualPosition {nullptr};
    std::vector<Position*> m_historicalPositions {};

};

#endif