#include "pieces.hpp"

#include <limits.h>

#include "cstf.hpp"

using namespace Piece;

Type charToPieceTable[makeUNumspeed(1) << (sizeof(char) * CHAR_BIT)];
char pieceToCharTable[makeUNumspeed(1) << Piece::usedBits];

Type Piece::create(const Color::Type color, const Dignity dignity) {
    ASSERT(color == Black || color == White);
    ASSERT(dignityLower <= dignity && dignity <= dignityHighter);

    return color | dignity;
}

Type Piece::fromChar(const char c) {
    ASSERT(c == 'p' || c == 'n' || c == 'b' || c == 'r' || c == 'q' || c == 'k'
        || c == 'P' || c == 'N' || c == 'B' || c == 'R' || c == 'Q' || c == 'K');

    return charToPieceTable[c];
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

void Piece::initTables() {
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

    charToPieceTable['p'] = create(Black, Pawn);
    charToPieceTable['n'] = create(Black, Knight);
    charToPieceTable['b'] = create(Black, Bishop);
    charToPieceTable['r'] = create(Black, Rook);
    charToPieceTable['q'] = create(Black, Queen);
    charToPieceTable['k'] = create(Black, King);

    charToPieceTable['P'] = create(White, Pawn);
    charToPieceTable['N'] = create(White, Knight);
    charToPieceTable['B'] = create(White, Bishop);
    charToPieceTable['R'] = create(White, Rook);
    charToPieceTable['Q'] = create(White, Queen);
    charToPieceTable['K'] = create(White, King);
}
