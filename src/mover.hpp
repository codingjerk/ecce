#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"
#include "boards.hpp"

namespace Move {
    extern Boolspeed (*specialMakeWhite[6])(Move::Type, Board::Type&);
    extern Boolspeed (*specialMakeBlack[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeWhite[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeBlack[6])(Move::Type, Board::Type&);

    extern Boolspeed (*specialFastMakeWhite[6])(Move::Type, Board::Type&);
    extern Boolspeed (*specialFastMakeBlack[6])(Move::Type, Board::Type&);
    extern void (*specialFastUnmakeWhite[6])(Move::Type, Board::Type&);
    extern void (*specialFastUnmakeBlack[6])(Move::Type, Board::Type&);
    
    extern Boolspeed (*specialMakeCaptureWhite[6])(Move::Type, Board::Type&);
    extern Boolspeed (*specialMakeCaptureBlack[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeCaptureWhite[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeCaptureBlack[6])(Move::Type, Board::Type&);
    
    extern Boolspeed (*specialMakeSilentWhite[6])(Move::Type, Board::Type&);
    extern Boolspeed (*specialMakeSilentBlack[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeSilentWhite[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeSilentBlack[6])(Move::Type, Board::Type&);

    void initTables();

    // Universal
    template <Color::Type COLOR> 
    inline Boolspeed make(Type, Board::Type&);

    template <>
    inline Boolspeed make<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeWhite[specialIndex](move, board);    
    }

    template <>
    inline Boolspeed make<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeBlack[specialIndex](move, board);    
    }

    template <Color::Type COLOR>
    inline void unmake(Type, Board::Type&);

    template <>
    inline void unmake<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeWhite[specialIndex](move, board);
    }

    template <>
    inline void unmake<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeBlack[specialIndex](move, board);
    }
    
    // Captures
    template <Color::Type COLOR> 
    inline Boolspeed makeCapture(Type, Board::Type&);

    template <>
    inline Boolspeed makeCapture<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeCaptureWhite[specialIndex](move, board);    
    }

    template <>
    inline Boolspeed makeCapture<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeCaptureBlack[specialIndex](move, board);    
    }

    template <Color::Type COLOR>
    inline void unmakeCapture(Type, Board::Type&);

    template <>
    inline void unmakeCapture<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeCaptureWhite[specialIndex](move, board);
    }

    template <>
    inline void unmakeCapture<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeCaptureBlack[specialIndex](move, board);
    }
    
    // Silents
    template <Color::Type COLOR> 
    inline Boolspeed makeSilent(Type, Board::Type&);

    template <>
    inline Boolspeed makeSilent<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeSilentWhite[specialIndex](move, board);    
    }

    template <>
    inline Boolspeed makeSilent<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeSilentBlack[specialIndex](move, board);    
    }

    template <Color::Type COLOR>
    inline void unmakeSilent(Type, Board::Type&);

    template <>
    inline void unmakeSilent<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeSilentWhite[specialIndex](move, board);
    }

    template <>
    inline void unmakeSilent<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeSilentBlack[specialIndex](move, board);
    }

    // Null
    template <Color::Type COLOR>
    void makeNull(Board::Type&);
    void unmakeNull(Board::Type&);

    // Untemplated functions
    inline Boolspeed make(Type move, Board::Type& board) {
        if (board.turn == White) {
            return make<White>(move, board);
        } else {
            return make<Black>(move, board);
        }
    }

    inline void unmake(Type move, Board::Type& board) {
        if (board.turn == White) {
            unmake<White>(move, board);
        } else {
            unmake<Black>(move, board);
        }
    }

    // Fast
    template <Color::Type COLOR> 
    inline Boolspeed makeFast(Type, Board::Type&);

    template <>
    inline Boolspeed makeFast<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialFastMakeWhite[specialIndex](move, board);    
    }

    template <>
    inline Boolspeed makeFast<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialFastMakeBlack[specialIndex](move, board);    
    }

    template <Color::Type COLOR>
    inline void unmakeFast(Type, Board::Type&);

    template <>
    inline void unmakeFast<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialFastUnmakeWhite[specialIndex](move, board);
    }

    template <>
    inline void unmakeFast<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialFastUnmakeBlack[specialIndex](move, board);
    }
}

#endif /* MOVER_HPP */
