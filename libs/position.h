#ifndef POSITION_H
#define POSITION_H

#include <string>
#include "piece.h"
#include "capturedpieces.h"

class Piece;

class Position
{
public:
    // Default Constructor
    Position() = default;

    // Constructor from FEN
    Position(std::string& fen);

    // Destructor
    ~Position() = default;

    // Returns the FEN of the Position
    std::string get_fen();

    // Uploads all the legal moves
    void compute_legal_moves();

    // Uploads all the possible pseudomoves
    void compute_pseudo_legal_moves();

    // Checks whether a move is legal or not
    bool is_move_legal(int initialSquare, int finalSquare);

    // Checks whether the king is in check or not
    bool king_in_check();

    // Move a piece in the position
    void move_piece(int initialSquare, int finalSquare);

public: // Members
    uint8_t m_squares[64] {};
    uint64_t m_whiteSquares {};  // There are 1s bits where there is a white piece
    uint64_t m_blackSquares {};  // There are 1s bits where there is a black piece
    bool m_whiteTurn {};
    bool m_whiteCanCastleKingside {};
    bool m_whiteCanCastleQueenside {};
    bool m_blackCanCastleKingside {};
    bool m_blackCanCastleQueenside {};
    uint64_t m_enpassant {};
    int m_halfMove {};
    int m_fullMove {};

    CapturedPieces m_whiteCaptured;
    CapturedPieces m_blackCaptured;

    uint64_t m_whiteKingSquare {};
    uint64_t m_blackKingSquare {};

    uint64_t m_pseudoLegalMoves[64] {}; // Containts a 64 bitboards where 1s are where the piece at that square can pseudo move
    uint64_t m_legalMoves[64] {}; // Containts a 64 bitboards where 1s are where the piece at that square can move
    uint64_t m_lastLegalMoves[64] {};

    bool m_staleMate {false};
    bool m_inCheck {false};
    bool m_mate {false};
};

#endif
