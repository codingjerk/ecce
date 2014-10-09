#include "pieces.hpp"

#include <limits.h>

#include "cstf.hpp"

using namespace Piece;

Type charToPieceTable[makeUNumspeed(1) << (sizeof(char) * CHAR_BIT)];
char pieceToCharTable[makeUNumspeed(1) << Piece::usedBits];

Type Piece::fromChar(const char c) {
    ASSERT(c == 'p' || c == 'n' || c == 'b' || c == 'r' || c == 'q' || c == 'k'
        || c == 'P' || c == 'N' || c == 'B' || c == 'R' || c == 'Q' || c == 'K');

    return charToPieceTable[(unsigned char)(c)];
}

Type Piece::fromString(const std::string str) {
    ASSERT(str.size() == 1);

    return fromChar(str[0]);
}

std::string Piece::show(const Type piece) {
    ASSERT((piece & Piece::typeMask) == piece);

    std::string result;
    result.reserve(1);
    result += pieceToCharTable[piece];

    return result;
}

#define UC(c) (unsigned char)(c)

void Piece::initTables() {
    pieceToCharTable[null]                  = '.';

    pieceToCharTable[create(Black, Pawn)]   = 'p';
    pieceToCharTable[create(Black, Knight)] = 'n';
    pieceToCharTable[create(Black, Bishop)] = 'b';
    pieceToCharTable[create(Black, Rook)]   = 'r';
    pieceToCharTable[create(Black, Queen)]  = 'q';
    pieceToCharTable[create(Black, King)]   = 'k';

    pieceToCharTable[create(White, Pawn)]   = 'P';
    pieceToCharTable[create(White, Knight)] = 'N';
    pieceToCharTable[create(White, Bishop)] = 'B';
    pieceToCharTable[create(White, Rook)]   = 'R';
    pieceToCharTable[create(White, Queen)]  = 'Q';
    pieceToCharTable[create(White, King)]   = 'K';

    charToPieceTable[UC('p')] = create(Black, Pawn);
    charToPieceTable[UC('n')] = create(Black, Knight);
    charToPieceTable[UC('b')] = create(Black, Bishop);
    charToPieceTable[UC('r')] = create(Black, Rook);
    charToPieceTable[UC('q')] = create(Black, Queen);
    charToPieceTable[UC('k')] = create(Black, King);

    charToPieceTable[UC('P')] = create(White, Pawn);
    charToPieceTable[UC('N')] = create(White, Knight);
    charToPieceTable[UC('B')] = create(White, Bishop);
    charToPieceTable[UC('R')] = create(White, Rook);
    charToPieceTable[UC('Q')] = create(White, Queen);
    charToPieceTable[UC('K')] = create(White, King);
}
