#include "piece.h"

uint64_t pawn_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares,
                                 uint64_t enpassant)
{
// 1. Initialize information
    uint64_t  pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute moves depending on the color
    if ((piece & color_mask) == white)
    {
        // 2.1. Advance pawn
        if (!get_nth_bit(opponentSquares, sq + up) &
            !get_nth_bit(colorSquares, sq + up))
        {
            set_nth_bit(pseudoLegalMoves, sq + up, 1);
            if ((sq2row_1 == 1) & !get_nth_bit(opponentSquares, sq + up*2) &
                !get_nth_bit(colorSquares, sq + up*2))
            {
                set_nth_bit(pseudoLegalMoves, sq + up*2, 1);
            }
        }

        // 2.2. Normal capture
        if (get_nth_bit(opponentSquares, sq + up_left) & (sq2col_a != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + up_left, 1);
        }
        if (get_nth_bit(opponentSquares, sq + up_right) & (sq2col_h != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + up_right, 1);
        }

        // 2.3. En passant captures
        if (get_nth_bit(enpassant, sq + left) & (sq2col_a != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + up_left, 1);
        }        
        if (get_nth_bit(enpassant, sq + right) & (sq2col_h != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + up_right, 1);
        }
    } else {
        // 2.4. Advance pawn
        if (!get_nth_bit(opponentSquares, sq + down)) {
            set_nth_bit(pseudoLegalMoves, sq + down, 1);
            if ((sq2row_8 == 1) & !get_nth_bit(opponentSquares, sq + down*2)) {
                set_nth_bit(pseudoLegalMoves, sq + down*2, 1);
            }
        }

        // 2.5. Normal capture
        if (get_nth_bit(opponentSquares, sq + down_left) & (sq2col_a != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + down_left, 1);
        }
        if (get_nth_bit(opponentSquares, sq + down_right) & (sq2col_h != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + down_right, 1);
        }

        // 2.6. En passant captures
        if (get_nth_bit(enpassant, sq + left) & (sq2col_a != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + down_left, 1);
        }        
        if (get_nth_bit(enpassant, sq + right) & (sq2col_h != 0)) {
            set_nth_bit(pseudoLegalMoves, sq + down_right, 1);
        }
    }
    return pseudoLegalMoves;
}

uint64_t knight_pseudo_legal_moves(uint8_t piece, int sq, uint64_t colorSquares)
{
// 1. Initialize information
    uint64_t pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute moves
    // 2.1. Up left
    if (!get_nth_bit(colorSquares, sq + up + left*2) & (sq2row_8 > 0) & (sq2col_a > 1))
    {
        set_nth_bit(pseudoLegalMoves, sq + up + left*2, 1);
    }
    if (!get_nth_bit(colorSquares, sq + up*2 + left) & (sq2row_8 > 1) & (sq2col_a > 0))
    {
        set_nth_bit(pseudoLegalMoves, sq + up*2 + left, 1);
    }

    // 2.2. Up right
    if (!get_nth_bit(colorSquares, sq + up*2 + right) & (sq2row_8 > 1) & (sq2col_h > 0))
    {
        set_nth_bit(pseudoLegalMoves, sq + up*2 + right, 1);
    }
    if (!get_nth_bit(colorSquares, sq + up + right*2) & (sq2row_8 > 0) & (sq2col_h > 1))
    {
        set_nth_bit(pseudoLegalMoves, sq + up + right*2, 1);
    }

    // 2.3. Down left
    if (!get_nth_bit(colorSquares, sq + down + left*2) & (sq2row_1 > 0) & (sq2col_a > 1))
    {
        set_nth_bit(pseudoLegalMoves, sq + down + left*2, 1);
    }
    if (!get_nth_bit(colorSquares, sq + down*2 + left) & (sq2row_1 > 1) & (sq2col_a > 0))
    {
        set_nth_bit(pseudoLegalMoves, sq + down*2 + left, 1);
    }

    // 2.4. Down right
    if (!get_nth_bit(colorSquares, sq + down*2 + right) & (sq2row_1 > 1) & (sq2col_h > 0))
    {
        set_nth_bit(pseudoLegalMoves, sq + down*2 + right, 1);
    }
    if (!get_nth_bit(colorSquares, sq + down + right*2) & (sq2row_1 > 0) & (sq2col_h > 1))
    {
        set_nth_bit(pseudoLegalMoves, sq + down + right*2, 1);
    }

    return pseudoLegalMoves;
}

uint64_t bishop_pseudo_legal_moves(uint8_t piece, int sq,
                                   uint64_t colorSquares, uint64_t opponentSquares)
{
// 1. Initialize information
    uint64_t pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute moves, until is a piece or out of bounds
    int actualIndex;
    int moves;

    // 2.1. Up left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a & moves < sq2row_8)
    {
        moves++;
        actualIndex += up_left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.2. Up right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h & moves < sq2row_8)
    {
        moves++;
        actualIndex += up_right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.3. Down left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a & moves < sq2row_1)
    {
        moves++;
        actualIndex += down_left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.4. Down right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h & moves < sq2row_1)
    {
        moves++;
        actualIndex += down_right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    return pseudoLegalMoves;
}

uint64_t rook_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares)
{
    // 1. Initialize information
    uint64_t pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute moves, until is a piece or out of bounds
    int actualIndex;
    int moves;

    // 2.1. Up
    actualIndex = sq;
    moves = 0;
    while (moves < sq2row_8)
    {
        moves++;
        actualIndex += up;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.2. Down
    actualIndex = sq;
    moves = 0;
    while (moves < sq2row_1)
    {
        moves++;
        actualIndex += down;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.3. Left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a)
    {
        moves++;
        actualIndex += left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.4. Right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h)
    {
        moves++;
        actualIndex += right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    return pseudoLegalMoves;
}

uint64_t queen_pseudo_legal_moves(uint8_t piece, int sq, 
                                  uint64_t colorSquares, uint64_t opponentSquares)
{
// 1. Initialize information
    uint64_t pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute moves, until is a piece or out of bounds
    int actualIndex;
    int moves;

    // 2.1. Up
    actualIndex = sq;
    moves = 0;
    while (moves < sq2row_8)
    {
        moves++;
        actualIndex += up;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.2. Down
    actualIndex = sq;
    moves = 0;
    while (moves < sq2row_1)
    {
        moves++;
        actualIndex += down;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.3. Left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a)
    {
        moves++;
        actualIndex += left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.4. Right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h)
    {
        moves++;
        actualIndex += right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.5. Up left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a & moves < sq2row_8)
    {
        moves++;
        actualIndex += up_left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.6. Up right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h & moves < sq2row_8)
    {
        moves++;
        actualIndex += up_right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.7. Down left
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_a & moves < sq2row_1)
    {
        moves++;
        actualIndex += down_left;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    // 2.8. Down right
    actualIndex = sq;
    moves = 0;
    while (moves < sq2col_h & moves < sq2row_1)
    {
        moves++;
        actualIndex += down_right;
        if (get_nth_bit(colorSquares, actualIndex))
        {
            break;
        }
        else if (get_nth_bit(opponentSquares, actualIndex))
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
            break;
        }
        else
        {
            set_nth_bit(pseudoLegalMoves, actualIndex, 1);
        }
    }

    return pseudoLegalMoves;
}

uint64_t king_pseudo_legal_moves(uint8_t piece, int sq,
                                 uint64_t colorSquares, uint64_t opponentSquares,
                                 bool canCastleKingside, bool canCastleQueenside,
                                 uint64_t lastLegalMoves[64])
{
// 1. Initialize information
    uint64_t pseudoLegalMoves = 0;
    uint8_t sq2col_a, sq2col_h, sq2row_1, sq2row_8;
    sq2col_a = sq % 8;
    sq2col_h = 7 - sq2col_a;
    sq2row_8 = sq / 8;
    sq2row_1 = 7 - sq2row_8;

    // 2. Compute normal moves
    // 2.1. Up
    if (sq2row_8 > 0)
    {
        if (!get_nth_bit(colorSquares, sq + up))
        {
            set_nth_bit(pseudoLegalMoves, sq + up, 1);
        }
    }

    // 2.2. Down
    if (sq2row_1 > 0)
    {
        if (!get_nth_bit(colorSquares, sq + down))
        {
            set_nth_bit(pseudoLegalMoves, sq + down, 1);
        }
    }

    // 2.3. Left
    if (sq2col_a > 0)
    {
        if (!get_nth_bit(colorSquares, sq + left))
        {
            set_nth_bit(pseudoLegalMoves, sq + left, 1);
        }
    }

    // 2.4. Right
    if (sq2col_h > 0)
    {
        if (!get_nth_bit(colorSquares, sq + right))
        {
            set_nth_bit(pseudoLegalMoves, sq + right, 1);
        }
    }

    // 2.5. Up left
    if (sq2row_8 > 0 & sq2col_a > 0)
    {
        if (!get_nth_bit(colorSquares, sq + up_left))
        {
            set_nth_bit(pseudoLegalMoves, sq + up_left, 1);
        }
    }

    // 2.6. Up right
    if (sq2row_8 > 0 & sq2col_h > 0)
    {
        if (!get_nth_bit(colorSquares, sq + up_right))
        {
            set_nth_bit(pseudoLegalMoves, sq + up_right, 1);
        }
    }

    // 2.7. Down left
    if (sq2row_1 > 0 & sq2col_a > 0)
    {
        if (!get_nth_bit(colorSquares, sq + down_left))
        {
            set_nth_bit(pseudoLegalMoves, sq + down_left, 1);
        }
    }

    // 2.8. Down right
    if (sq2row_1 > 0 & sq2col_h > 0)
    {
        if (!get_nth_bit(colorSquares, sq + down_right))
        {
            set_nth_bit(pseudoLegalMoves, sq + down_right, 1);
        }
    }

    // 2.9. Castling
    if (canCastleKingside &&
        !get_nth_bit(colorSquares, sq + right) &&
        !get_nth_bit(colorSquares, sq + right*2) &&
        !get_nth_bit(opponentSquares, sq + right) &&
        !get_nth_bit(opponentSquares, sq + right*2))
    {
        set_nth_bit(pseudoLegalMoves, sq + right*2, 1);
        // Check it is not in check or checks in the middle
        for (int k = 0; k < 64; k++)
        {
            if (get_nth_bit(lastLegalMoves[k], sq) ||
                get_nth_bit(lastLegalMoves[k], sq + right) ||
                get_nth_bit(lastLegalMoves[k], sq + right*2))
            {
                set_nth_bit(pseudoLegalMoves, sq + right*2, 0);
                break;
            }
        }
    }
    if (canCastleQueenside &&
        !get_nth_bit(colorSquares, sq + left) &&
        !get_nth_bit(colorSquares, sq + left*2) &&
        !get_nth_bit(colorSquares, sq + left*3) &&
        !get_nth_bit(opponentSquares, sq + left) &&
        !get_nth_bit(opponentSquares, sq + left*2) &&
        !get_nth_bit(opponentSquares, sq + left*3))
    {
        set_nth_bit(pseudoLegalMoves, sq + left*2, 1);
        // Check it is not in check or checks in the middle
        for (int k = 0; k < 64; k++)
        {
            if (get_nth_bit(lastLegalMoves[k], sq) ||
                get_nth_bit(lastLegalMoves[k], sq + left) ||
                get_nth_bit(lastLegalMoves[k], sq + left*2) ||
                get_nth_bit(lastLegalMoves[k], sq + left*3))
            {
                set_nth_bit(pseudoLegalMoves, sq + left*2, 0);
                break;
            }
        }
    }
    return pseudoLegalMoves;
}
