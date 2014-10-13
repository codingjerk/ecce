#ifndef GENERATOR_PHASES_HPP
#define GENERATOR_PHASES_HPP

#include "silents.hpp"
#include "captures.hpp"

namespace Generator {
    typedef void (*Phase)(Move::Buffer&, const Board::Type&);
    using Phases = Phase[2];

    static const Phases whitePhases = {
        Captures::phase<White>,
        Silents::phase<White>
    };

    static const Phases blackPhases = {
        Captures::phase<Black>,
        Silents::phase<Black>
    };

    template <Color::Type COLOR>
    inline Phase* phases() {
        if (COLOR == White) {
            return (Phase*)whitePhases;
        } else {
            return (Phase*)blackPhases;
        }
    }

    #define forPhases(PHASE, PHASES) \
        UNumspeed G_M_I = 0; \
        auto PHASE = PHASES[G_M_I]; \
        for (; G_M_I < 2; ++G_M_I, PHASE = PHASES[G_M_I])
}

#endif /* GENERATOR_PHASES_HPP */
