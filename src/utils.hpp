#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef OSLINUX
#include <sys/time.h>
#include <unistd.h>

inline unsigned long GetTickCount()
{
    struct timeval tv;
    if( gettimeofday(&tv, 0) != 0 )
        return 0;
 
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#define min(a,b) ((a)<(b))?(a):(b)
#define max(a,b) ((a)>(b))?(a):(b)
#endif

#ifndef OSLINUX
    #include <Windows.h>
#endif

template <class RESULT = UNumspeed>
inline RESULT toNum(std::string text) {
    std::stringstream ss(text);
    RESULT value; 
    ss >> value;
    return value;
}

inline bool isInputAvailable() {
#ifdef OSLINUX
    int val;
    fd_set set[1];
    struct timeval time_val[1];

    FD_ZERO(set);
    FD_SET(STDIN_FILENO,set);

    time_val->tv_sec = 0;
    time_val->tv_usec = 0;

    val = select(STDIN_FILENO+1,set,NULL,NULL,time_val);

    return val > 0;
#else
    static bool init = false, is_pipe;
    static HANDLE stdin_h;
    DWORD val, error;

    if (stdin->_cnt > 0) return true;

    if (!init) {
        init = true;
        stdin_h = GetStdHandle(STD_INPUT_HANDLE);
        is_pipe = !GetConsoleMode(stdin_h, &val); 

        if (!is_pipe) {
            SetConsoleMode(stdin_h, val & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(stdin_h); 
        }
    }

    if (is_pipe) {
        if (!PeekNamedPipe(stdin_h, NULL, 0, NULL, &val, NULL)) {
            return true;
        }
        return val > 0;
    } else {
        GetNumberOfConsoleInputEvents(stdin_h, &val);
        return val > 1;
    }

    return false;
#endif
}

namespace Move {
    inline Type fromString(const std::string text, Board::Type &board) {
        Move::Type simple = fromString(text);

        // @TODO: Create methods Move::from and Move::to
        Coord::Type from = (simple >> Coord::usedBits) & Coord::typeMask;
        Coord::Type to = simple & Coord::typeMask;

        // Captures
        simple |= board.squares[to] << captureOffset;

        // PawnDoubles
        if (board.squares[from] == Piece::create(White, Pawn) || board.squares[from] == Piece::create(Black, Pawn)) {
            if (from - to == 16 || to - from == 16) {
                simple |= (pawnDoubleFlag << specialOffset);
            }//v
            // v
            // Promotions
            else if (text.size() == 5) {
                Color::Type we = (board.squares[from]) & Color::typeMask;
                Piece::Type promoted = (Piece::fromChar(text[4]) & Dignity::typeMask) | we;
                simple |= (promoted << promotionOffset)
                    | (promotionFlag << specialOffset);
            }//v
            // v
            // Enpassants
            else if (to == Board::enpassant(board)) {
                simple |= (enpassantFlag << specialOffset);
            }
        }//v
        // v
        // Castles
        else if (board.squares[from] == Piece::create(White, King) || board.squares[from] == Piece::create(Black, King)) {
            if ((from == Coord::E1 && to == Coord::C1) || (from == Coord::E8 && to == Coord::C8)) {
                simple |= (castleLongFlag << specialOffset);
            }
            else if ((from == Coord::E1 && to == Coord::G1) || (from == Coord::E8 && to == Coord::G8)) {
                simple |= (castleShortFlag << specialOffset);
            }
        }

        return simple;
    }

    template <Color::Type COLOR>
    inline Type fromShortString(const std::string text, Board::Type &board) {
        auto cursor = text.rbegin();
        
        if (*cursor == '+') ++cursor;

        const Coord::Type unknown = 666;

        Coord::Type toDigit = unknown;
        if (cursor != text.rend() && isdigit(*cursor)) {
            toDigit = Coord::digit(Coord::fromChars('a', *cursor));
            ++cursor;
        }

        if (cursor != text.rend() && *cursor == 'x') {
            ++cursor;
        }

        Coord::Type toAlpha = unknown;
        if (cursor != text.rend() && isalpha(*cursor) && islower(*cursor)) {
            toAlpha = Coord::alpha(Coord::fromChars(*cursor, '1'));
            ++cursor;
        }

        if (cursor != text.rend() && *cursor == 'x') {
            ++cursor;
        }

        Coord::Type fromDigit = unknown;
        if (cursor != text.rend() && isdigit(*cursor)) {
            if (toDigit == unknown) {
                toDigit = Coord::digit(Coord::fromChars('a', *cursor));
            } else {
                fromDigit = Coord::digit(Coord::fromChars('a', *cursor));
            }
            ++cursor;
        }

        if (cursor != text.rend() && *cursor == 'x') {
            ++cursor;
        }

        Coord::Type fromAlpha = unknown;
        if (cursor != text.rend() && isalpha(*cursor) && islower(*cursor)) {
            if (toAlpha == unknown) {
                toAlpha = Coord::alpha(Coord::fromChars(*cursor, '1'));
            } else {
                fromAlpha = Coord::alpha(Coord::fromChars(*cursor, '1'));
            }
            ++cursor;
        }

        if (cursor != text.rend() && *cursor == 'x') {
            ++cursor;
        }

        Piece::Type piece = Piece::create(White, Pawn);
        if (cursor != text.rend() && isalpha(*cursor) && isupper(*cursor)) {
            piece = Piece::fromChar(*cursor);
            ++cursor;
        }

        if (COLOR == Black) {
            piece ^= Color::White;
        }

        if (cursor != text.rend() && *cursor == 'x') {
            ++cursor;
        }

        Move::Buffer buffer;
        Generator::phase<COLOR>(buffer, board);

        auto total = buffer[0];
        for (Move::Type i = 1; i <= total; ++i) {
            const auto move = buffer[i];

            const auto checkedTo = move & Coord::typeMask;
            const auto checkedToDigit = Coord::digit(checkedTo);
            const auto checkedToAlpha = Coord::alpha(checkedTo);

            const auto checkedFrom = (move >> Coord::usedBits) & Coord::typeMask;
            const auto checkedFromDigit = Coord::digit(checkedFrom);
            const auto checkedFromAlpha = Coord::alpha(checkedFrom);
            
            if (board.squares[checkedFrom] == piece) {
                if (toDigit == unknown || toDigit == checkedToDigit) {
                    if (toAlpha == unknown || toAlpha == checkedToAlpha) {
                        if (fromDigit == unknown || fromDigit == checkedFromDigit) {
                            if (fromAlpha == unknown || fromAlpha == checkedFromAlpha) {
                                std::string moveString = Coord::show(checkedFrom) + Coord::show(checkedTo);
                                return fromString(moveString, board);
                            }
                        }
                    }
                }
            }
        }

        return Move::create(Coord::A1, Coord::A1);
    }
}

#endif /* UTILS_HPP */
