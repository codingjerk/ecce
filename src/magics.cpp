#include "magics.hpp"

#include "generatorTables.hpp"

using namespace Magic;

UNumspeed Magic::bishopOffsets[64];

Bitboard::Type *Magic::bishopData = nullptr;

const UNumspeed Magic::bishopMaskBits[64] = {
    6,  5,  5,  5,  5,  5,  5,  6,
    5,  5,  5,  5,  5,  5,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  5,  5,  5,  5,  5,  5,
    6,  5,  5,  5,  5,  5,  5,  6
};

const UNumspeed Magic::bishopMaskShifts[64] = {
    64 - 6,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 6,
    64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 7,  64 - 7,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 9,  64 - 9,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 9,  64 - 9,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 7,  64 - 7,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,
    64 - 6,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 6
};

const UNum64 Magic::bishopMagics[makeUNumspeed(1) << Coord::usedBits] = {
    0x0048610528020080, 0x00c4100212410004, 0x0004180181002010, 0x0004040188108502, 
    0x0012021008003040, 0x0002900420228000, 0x0080808410c00100, 0x000600410c500622, 
    0x00c0056084140184, 0x0080608816830050, 0x00a010050200b0c0, 0x0000510400800181, 
    0x0000431040064009, 0x0000008820890a06, 0x0050028488184008, 0x00214a0104068200, 
    0x004090100c080081, 0x000a002014012604, 0x0020402409002200, 0x008400c240128100, 
    0x0001000820084200, 0x0024c02201101144, 0x002401008088a800, 0x0003001045009000, 
    0x0084200040981549, 0x0001188120080100, 0x0048050048044300, 0x0008080000820012, 
    0x0001001181004003, 0x0090038000445000, 0x0010820800a21000, 0x0044010108210110, 
    0x0090241008204e30, 0x000c04204004c305, 0x0080804303300400, 0x00a0020080080080, 
    0x0000408020220200, 0x0000c08200010100, 0x0010008102022104, 0x0008148118008140, 
    0x0008080414809028, 0x0005031010004318, 0x0000603048001008, 0x0008012018000100, 
    0x0000202028802901, 0x004011004b049180, 0x0022240b42081400, 0x00c4840c00400020, 
    0x0084009219204000, 0x000080c802104000, 0x0002602201100282, 0x0002040821880020, 
    0x0002014008320080, 0x0002082078208004, 0x0009094800840082, 0x0020080200b1a010, 
    0x0003440407051000, 0x000000220e100440, 0x00480220a4041204, 0x00c1800011084800, 
    0x000008021020a200, 0x0000414128092100, 0x0000042002024200, 0x0002081204004200
};

UNumspeed Magic::rookOffsets[64];

Bitboard::Type *Magic::rookData = nullptr;

const UNum64 Magic::rookMagics[64] = {
    0x00800011400080a6, 0x004000100120004e, 0x0080100008600082, 0x0080080016500080, 
    0x0080040008000280, 0x0080020005040080, 0x0080108046000100, 0x0080010000204080, 
    0x0010800424400082, 0x00004002c8201000, 0x000c802000100080, 0x00810010002100b8, 
    0x00ca808014000800, 0x0002002884900200, 0x0042002148041200, 0x00010000c200a100, 
    0x00008580004002a0, 0x0020004001403008, 0x0000820020411600, 0x0002120021401a00, 
    0x0024808044010800, 0x0022008100040080, 0x00004400094a8810, 0x0000020002814c21, 
    0x0011400280082080, 0x004a050e002080c0, 0x00101103002002c0, 0x0025020900201000, 
    0x0001001100042800, 0x0002008080022400, 0x000830440021081a, 0x0080004200010084, 
    0x00008000c9002104, 0x0090400081002900, 0x0080220082004010, 0x0001100101000820, 
    0x0000080011001500, 0x0010020080800400, 0x0034010224009048, 0x0002208412000841, 
    0x000040008020800c, 0x001000c460094000, 0x0020006101330040, 0x0000a30010010028, 
    0x0004080004008080, 0x0024000201004040, 0x0000300802440041, 0x00120400c08a0011, 
    0x0080006085004100, 0x0028600040100040, 0x00a0082110018080, 0x0010184200221200, 
    0x0040080005001100, 0x0004200440104801, 0x0080800900220080, 0x000a01140081c200, 
    0x0080044180110021, 0x0008804001001225, 0x00a00c4020010011, 0x00001000a0050009, 
    0x0011001800021025, 0x00c9000400620811, 0x0032009001080224, 0x001400810044086a
};

const UNumspeed Magic::rookMaskBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const UNumspeed Magic::rookMaskShifts[64] = {
    64 - 12, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 12,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 12, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 12
};

Bitboard::Type turnBits(Bitboard::Type mask, UNumspeed bitsToTurn) {
    Bitboard::Type result = 0;

    while (mask != 0 && bitsToTurn != 0) {
        UNumspeed first = Bitboard::bitScan(mask);
        mask ^= Bitboard::fromIndex(first);

        UNumspeed digit = UNumspeed(bitsToTurn & 1);
        bitsToTurn >>= 1;
        result |= digit * Bitboard::fromIndex(first);
    }

    return result;
}

Bitboard::Type up(const Bitboard::Type b) { return (b & ~(Bitboard::upLine)) << 8; }
Bitboard::Type down(const Bitboard::Type b) { return (b & ~(Bitboard::downLine)) >> 8; }
Bitboard::Type left(const Bitboard::Type b) { return (b & ~(Bitboard::leftLine)) >> 1; }
Bitboard::Type right(const Bitboard::Type b) { return (b & ~(Bitboard::rightLine)) << 1; }

Bitboard::Type upRight(const Bitboard::Type b) { return (b & ~(Bitboard::rightLine|Bitboard::upLine)) << 9; }
Bitboard::Type upLeft(const Bitboard::Type b) { return (b & ~(Bitboard::leftLine|Bitboard::upLine)) << 7; }
Bitboard::Type downRight(const Bitboard::Type b) { return (b & ~(Bitboard::rightLine|Bitboard::downLine)) >> 7; }
Bitboard::Type downLeft(const Bitboard::Type b) { return (b & ~(Bitboard::leftLine|Bitboard::downLine)) >> 9; }

Bitboard::Type calculateBishopAttacks(UNumspeed from, const Bitboard::Type nonEmpty)
{
    Bitboard::Type result = 0;
    Bitboard::Type x = 0;

    x = upRight(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = upRight(x);               
    }

    x = upLeft(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = upLeft(x);               
    }

    x = downRight(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = downRight(x);               
    }

    x = downLeft(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = downLeft(x);               
    }

    return result;
}

Bitboard::Type calculateRookAttacks(UNumspeed from, const Bitboard::Type nonEmpty)
{
    Bitboard::Type result = 0;
    Bitboard::Type x = 0;

    x = up(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = up(x);               
    }

    x = down(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = down(x);               
    }

    x = right(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = right(x);               
    }

    x = left(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = left(x);               
    }

    return result;
}

void Magic::initTables() {
    UNumspeed offset = makeUNumspeed(0);
    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);
        bishopOffsets[from] = offset;
        offset += (1 << bishopMaskBits[from]);
    }
    bishopData = new Bitboard::Type[offset];

    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);

        Bitboard::Type mask = Tables::bishopMasks[from];
        UNumspeed bits = bishopMaskBits[from];
        for (UNumspeed bitsToTurn = makeUNumspeed(0); bitsToTurn < (makeUNumspeed(1) << bits); ++bitsToTurn) {
            Bitboard::Type nonEmpty = turnBits(mask, bitsToTurn);
            Bitboard::Type data = calculateBishopAttacks(from, nonEmpty);
            UNumspeed index = bishopOffsets[from] + UNumspeed((nonEmpty * bishopMagics[from]) >> (makeUNumspeed(64)-bits));
            bishopData[index] = data;
        }
    }

    offset = makeUNumspeed(0);
    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);
        rookOffsets[from] = offset;
        offset += (1 << rookMaskBits[from]);
    }
    rookData = new Bitboard::Type[offset];

    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);

        Bitboard::Type mask = Tables::rookMasks[from];
        UNumspeed bits = rookMaskBits[from];
        for (UNumspeed bitsToTurn = makeUNumspeed(0); bitsToTurn < (makeUNumspeed(1) << bits); ++bitsToTurn) {
            Bitboard::Type nonEmpty = turnBits(mask, bitsToTurn);
            Bitboard::Type data = calculateRookAttacks(from, nonEmpty);
            UNumspeed index = rookOffsets[from] + UNumspeed((nonEmpty * rookMagics[from]) >> (makeUNumspeed(64) - bits));
            rookData[index] = data;
        }
    }
}
