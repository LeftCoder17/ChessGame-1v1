#ifndef PIECE_H
#define PIECE_H

#include <QDebug>
#include <iostream>
#include "managebits.h"

// Convert pieces information into numbers
const uint8_t none = 0;
const uint8_t king = 1;
const uint8_t pawn = 2;
const uint8_t knight = 3;
const uint8_t bishop = 4;
const uint8_t rook = 5;
const uint8_t queen = 6;
const uint8_t white = 8;
const uint8_t black = 16;
const uint8_t type_mask = 0b111; // firsts 3 bits
const uint8_t color_mask = 0b11000; // bits 4 and 5

// Convert directions into numbers
const int8_t right = 1;
const int8_t left = -1;
const int8_t up = -8;
const int8_t down = 8;
const uint8_t up_right = -7;
const uint8_t up_left = -9;
const uint8_t down_right = 9;
const uint8_t down_left = 7;

// Returns the pseudo legal moves of the pawn
uint64_t pawn_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares,
                                 uint64_t enpassant);

// Returns the pseudo legal moves of the knight
uint64_t knight_pseudo_legal_moves(uint8_t piece, int sq, uint64_t colorSquares);

// Returns the pseudo legal moves of the bishop
uint64_t bishop_pseudo_legal_moves(uint8_t piece, int sq,
                                   uint64_t colorSquares, uint64_t opponentSquares);

// Returns the pseudo legal moves of the rook
uint64_t rook_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares);

// Returns the pseudo legal moves of the queen
uint64_t queen_pseudo_legal_moves(uint8_t piece, int sq, 
                                  uint64_t colorSquares, uint64_t opponentSquares);

// Returns the pseudo legal moves of the king
uint64_t king_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares,
                                 bool canCastleKingside, bool canCastleQueenside,
                                 uint64_t lastLegalMoves[64]);

#endif
