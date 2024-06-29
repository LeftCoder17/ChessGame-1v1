#include <map>
#include <sstream>
#include <bitset>

#include "position.h"

Position::Position(std::string& fen)
{    
    std::stringstream ss(fen);
    std::string boardPart, turnPart, castlingPart, enpassantPart, halfMovePart, fullMovePart;
    ss >>  boardPart >> turnPart >> castlingPart >> enpassantPart >> halfMovePart >> fullMovePart;
    
    // 1. Set squares
    m_whiteSquares = 0;
    m_blackSquares = 0;

    int sq = 0;
    for (char c : boardPart)
    {
        if (c == '/') continue;
        else if (isdigit(c))
        {
            int emptySquares = c - '0';
            for (int i = 0; i < emptySquares; i++)
            {
                m_squares[sq] = none;
                sq++;
            }
        } else {
            bool isWhite = isupper(c);
            uint8_t color = isWhite ? white : black;
            switch (tolower(c))
            {
                case 'p': m_squares[sq] = pawn | color; break;
                case 'n': m_squares[sq] = knight | color; break;
                case 'b': m_squares[sq] = bishop | color; break;
                case 'r': m_squares[sq] = rook | color; break;
                case 'q': m_squares[sq] = queen | color; break;
                case 'k': 
                {
                    m_squares[sq] = king | color;
                    if (color == white) set_nth_bit(m_whiteKingSquare, sq, 1);
                    else set_nth_bit(m_blackKingSquare, sq, 1);
                    break;
                }
                default: break;
            }
            if (color == white) {
                set_nth_bit(m_whiteSquares, sq, 1);
            } else {
                set_nth_bit(m_blackSquares, sq, 1);
            }
            sq++;
        }
    }

    // 2. Set turn
    if (turnPart == "w") m_whiteTurn = true;
    else m_whiteTurn = false;

    // 3. Set castling rights
    m_whiteCanCastleKingside = false;
    m_whiteCanCastleQueenside = false;
    m_blackCanCastleKingside = false;
    m_blackCanCastleQueenside = false;

    for (char c : castlingPart)
    {
        if (c == 'K') m_whiteCanCastleKingside = true;
        else if (c == 'Q') m_whiteCanCastleQueenside = true;
        else if (c == 'k') m_blackCanCastleKingside = true;
        else if (c == 'q') m_blackCanCastleQueenside = true;
    }

    // 4. Set en passant
    if (enpassantPart == "-") {
        m_enpassant = 0;
    } else {
        uint64_t column = enpassantPart[0] - 'a';
        uint64_t row = 7 - (enpassantPart[1] - '1');
        set_nth_bit(m_enpassant, row * 8 + column, 1);
    }

    // 5. Add half-move clock and full-move number
    m_halfMove = std::stoi(halfMovePart);
    m_fullMove = std::stoi(fullMovePart);
}

std::string Position::get_fen()
{
    std::map<uint8_t, char> invfenSymbols;
    invfenSymbols[black | pawn] = 'p';
    invfenSymbols[black | knight] = 'n';
    invfenSymbols[black | bishop] = 'b';
    invfenSymbols[black | rook] = 'r';
    invfenSymbols[black | queen] = 'q';
    invfenSymbols[black | king] = 'k';
    invfenSymbols[white | pawn] = 'P';
    invfenSymbols[white | knight] = 'N';
    invfenSymbols[white | bishop] = 'B';
    invfenSymbols[white | rook] = 'R';
    invfenSymbols[white | queen] = 'Q';
    invfenSymbols[white | king] = 'K';

    std::string fen {};

    // 1. Add squares information
    int emptySquares {0};
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            uint8_t& piece = m_squares[row * 8 + column];
            if (piece == none) {
                emptySquares++;
            } else {
                if (emptySquares) {
                    fen += std::to_string(emptySquares);
                    emptySquares = 0;
                }
                fen += invfenSymbols[piece];
            }
        }
        if (emptySquares) {
            fen += std::to_string(emptySquares);
            emptySquares = 0;
        }
        if (row != 7) {
            fen += '/';
        }
    }

    // 2. Add turn information
    fen += m_whiteTurn == true ? " w " : " b ";

    // 3. Add castling rights information
    fen += m_whiteCanCastleKingside ? "K" : "";
    fen += m_whiteCanCastleQueenside ? "Q" : "";
    fen += m_blackCanCastleKingside ? "k" : "";
    fen += m_blackCanCastleQueenside ? "q" : "";
    // If no castling rights, use '-'
    if (fen.back() == ' ') {
        fen += "-";
    }
    fen += ' ';

    // 4. Add en passant target square (if any)
    if (m_enpassant == 0) {
        fen += '-';
    } else {
        int index = -1;
        uint64_t enpassant = m_enpassant;
        while (enpassant) {
            index++;
            enpassant >>= 1;
        }

        char column = 'a' + (index % 8);
        char row = '8' - (index / 8);
        fen += column;
        fen += row;
    }
    fen += ' ';

    // 5. Add half-move clock and full-move number
    fen += std::to_string(m_halfMove);
    fen += ' ';
    fen += std::to_string(m_fullMove);

    return fen;
}

void Position::compute_legal_moves() {
    // 1. Compute pseudo legal moves
    compute_pseudo_legal_moves();

    // 2. Computes legal moves
    for (int initialSquare = 0; initialSquare < 64; initialSquare++)
    {
        m_legalMoves[initialSquare] = 0;
        uint64_t pseudoMoves = m_pseudoLegalMoves[initialSquare];
        while (pseudoMoves)
        {
            int finalSquare = __builtin_ctzll(pseudoMoves);
            pseudoMoves &= pseudoMoves - 1; // Takes the LSB and sets it to 0

            if (is_move_legal(initialSquare, finalSquare))
            {
                set_nth_bit(m_legalMoves[initialSquare], finalSquare, 1);
            }
        }
    }

    // 3. Checks if Stalemate
    m_staleMate = true;
    for (uint64_t legalMoves : m_legalMoves)
    {
        if (legalMoves != 0)
        {
            m_staleMate = false;
            break;
        }
    }

    // 4. Check if Mate
    if (m_staleMate && m_inCheck) m_mate = true;

    return;
}

void Position::compute_pseudo_legal_moves()
{
    uint8_t colorTurn = m_whiteTurn ? white : black;
    uint64_t colorSquares = m_whiteTurn ? m_whiteSquares : m_blackSquares;
    uint64_t opponentSquares = m_whiteTurn ? m_blackSquares : m_whiteSquares;
    bool canCastleKingside = m_whiteTurn ? m_whiteCanCastleKingside : m_blackCanCastleKingside;
    bool canCastleQueenside = m_whiteTurn ? m_whiteCanCastleQueenside : m_blackCanCastleQueenside;

    for (int sq = 0; sq < 64; sq++)
    {
        uint8_t piece = m_squares[sq];
        if ((piece & color_mask) == colorTurn)
        {
            switch (piece & type_mask)
            {
                case pawn: m_pseudoLegalMoves[sq] = pawn_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares, m_enpassant); break;
                case knight: m_pseudoLegalMoves[sq] = knight_pseudo_legal_moves(piece, sq, colorSquares); break;
                case bishop: m_pseudoLegalMoves[sq] = bishop_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case rook: m_pseudoLegalMoves[sq] = rook_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case queen: m_pseudoLegalMoves[sq] = queen_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case king: m_pseudoLegalMoves[sq] = king_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares,
                                                                            canCastleKingside, canCastleQueenside, m_lastLegalMoves); break;
                default: break;
            }
        }
        else
        {
            m_pseudoLegalMoves[sq] = 0;
        }

    }

    return;
}

bool Position::is_move_legal(int initialSquare, int finalSquare)
{
    Position tempPosition = *this;
    tempPosition.move_piece(initialSquare, finalSquare);
    if (tempPosition.king_in_check())
    {
        m_inCheck = true;
        return false;
    }
    return true;
}

bool Position::king_in_check()
{
    uint64_t kingPosition = m_whiteTurn ? m_blackKingSquare : m_whiteKingSquare;
    uint8_t colorTurn = m_whiteTurn ? white : black;
    uint64_t colorSquares = m_whiteTurn ? m_whiteSquares : m_blackSquares;
    uint64_t opponentSquares = m_whiteTurn ? m_blackSquares : m_whiteSquares;
    bool canCastleKingside = m_whiteTurn ? m_whiteCanCastleKingside : m_blackCanCastleKingside;
    bool canCastleQueenside = m_whiteTurn ? m_whiteCanCastleQueenside : m_blackCanCastleQueenside;

    for (int sq = 0; sq < 64; sq++)
    {
        uint8_t piece = m_squares[sq];
        if ((piece & color_mask) == colorTurn)
        {
            uint64_t pseudoLegalMoves {0};
            switch (piece & type_mask)
            {
                case pawn: pseudoLegalMoves = pawn_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares, m_enpassant); break;
                case knight: pseudoLegalMoves = knight_pseudo_legal_moves(piece, sq, colorSquares); break;
                case bishop: pseudoLegalMoves = bishop_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case rook: pseudoLegalMoves = rook_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case queen: pseudoLegalMoves = queen_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares); break;
                case king: pseudoLegalMoves = king_pseudo_legal_moves(piece, sq, colorSquares, opponentSquares,
                                                                      canCastleKingside, canCastleQueenside, m_lastLegalMoves); break;
                default: break;
            }
            if (kingPosition & pseudoLegalMoves)
            {
                return true;
            }
        }
    }
    return false;
}

void Position::move_piece(int initialSquare, int finalSquare)
{
    // 1. Piece movement
    uint8_t piece = m_squares[initialSquare];
    if ((m_squares[finalSquare]!= none))
    {
        switch (m_squares[finalSquare] & type_mask)
        {
            case pawn: m_whiteTurn ? m_whiteCaptured.pawns++ : m_blackCaptured.pawns++; break;
            case knight: m_whiteTurn ? m_whiteCaptured.knights++ : m_blackCaptured.knights++; break;
            case bishop: m_whiteTurn ? m_whiteCaptured.bishops++ : m_blackCaptured.bishops++; break;
            case rook: m_whiteTurn ? m_whiteCaptured.rooks++ : m_blackCaptured.rooks++; break;
            case queen:  m_whiteTurn ? m_whiteCaptured.queens++ : m_blackCaptured.queens++; break;
            default: break;
            }
    }
    m_squares[finalSquare] = piece;
    m_squares[initialSquare] = none;

    if (m_whiteTurn)
    {
        set_nth_bit(m_whiteSquares, initialSquare, 0);
        set_nth_bit(m_whiteSquares, finalSquare, 1);
        set_nth_bit(m_blackSquares, finalSquare, 0);
    }
    else
    {
        set_nth_bit(m_blackSquares, initialSquare, 0);
        set_nth_bit(m_blackSquares, finalSquare, 1);
        set_nth_bit(m_whiteSquares, finalSquare, 0);
    }

    // 2. Change turn
    m_whiteTurn = !m_whiteTurn;

    // 3. Manage king move
    // 3.1. White castling
    if (((piece & type_mask) == king) &&
         (piece & color_mask) == white)
    {   
        bool whiteCastle = false;
        int initalRookIndex, finalRookIndex;
        if (finalSquare == initialSquare + right*2)
        {
            whiteCastle = true;
            initalRookIndex = 63;
            finalRookIndex = 61;

        }
        else if (finalSquare == initialSquare + left*2)
        {
            whiteCastle = true;
            initalRookIndex = 56;
            finalRookIndex = 59;
        }

        if (whiteCastle)
        {
            m_squares[finalRookIndex] = m_squares[initalRookIndex];
            m_squares[initalRookIndex] = none;
            set_nth_bit(m_whiteSquares, initalRookIndex, 0);
            set_nth_bit(m_whiteSquares, finalRookIndex, 1);
        }
        m_whiteCanCastleKingside = false;
        m_whiteCanCastleQueenside = false;
        m_whiteKingSquare = 0;
        set_nth_bit(m_whiteKingSquare, finalSquare, 1);
    }

    // 3.2. Black castling
    if (((piece & type_mask) == king) &&
         (piece & color_mask) == black)
    {   
        bool blackCastle = false;
        int initalRookIndex, finalRookIndex;
        if (finalSquare == initialSquare + right*2)
        {
            blackCastle = true;
            initalRookIndex = 7;
            finalRookIndex = 5;

        }
        else if (finalSquare == initialSquare + left*2)
        {
            blackCastle = true;
            initalRookIndex = 0;
            finalRookIndex = 3;
        }

        if (blackCastle)
        {
            m_squares[finalRookIndex] = m_squares[initalRookIndex];
            m_squares[initalRookIndex] = none;
            set_nth_bit(m_blackSquares, initalRookIndex, 0);
            set_nth_bit(m_blackSquares, finalRookIndex, 1);
        }
        m_blackCanCastleKingside = false;
        m_blackCanCastleQueenside = false;
        m_blackKingSquare = 0;
        set_nth_bit(m_blackKingSquare, finalSquare, 1);
    }

    // 3.3. Nullify castles if rook involved
    if ((initialSquare == 63) || (finalSquare == 63)) m_whiteCanCastleKingside = false;
    if ((initialSquare == 56) || (finalSquare == 56)) m_whiteCanCastleQueenside = false;
    if ((initialSquare == 7) || (finalSquare == 7)) m_blackCanCastleKingside = false;
    if ((initialSquare == 0) || (finalSquare == 0)) m_blackCanCastleQueenside = false;

    // 4. Manage enpassant
    if (((piece & type_mask) == pawn))
    {
        // 4.1. Double advance of pawn
        if ((finalSquare == initialSquare + up*2) || (finalSquare == initialSquare + down*2))
        {
            m_enpassant = 0;
            set_nth_bit(m_enpassant, finalSquare, 1);
        }
        else
        {
            // 4.2. Enpassant capture of white
            if (get_nth_bit(m_enpassant, finalSquare + down))
            {
                m_squares[finalSquare + down] = none;
                set_nth_bit(m_blackSquares, finalSquare + down, 0);
                m_whiteCaptured.pawns++;
            }
            // 4.3. Enpassant capture of black
            else if (get_nth_bit(m_enpassant, finalSquare + up))
            {
                m_squares[finalSquare + up] = none;
                set_nth_bit(m_whiteSquares, finalSquare + up, 0);
                m_blackCaptured.pawns++;
            }
            m_enpassant = 0;
        }
    }
    else
    {
        m_enpassant = 0;
    }

    // 5. Moves counter
    if (((piece & type_mask) == pawn))
    {
        m_halfMove = 0;
    }
    else
    {
        m_halfMove++;
    }

    if (m_whiteTurn) m_fullMove++;

    return;
}
