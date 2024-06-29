#ifndef CAPTUREDPIECES_H
#define CAPTUREDPIECES_H

struct CapturedPieces
{
    int pawns {};
    int knights {};
    int bishops {};
    int rooks {};
    int queens {};
    int promotionPoints {};

    // Constructor
    CapturedPieces();
};

#endif
