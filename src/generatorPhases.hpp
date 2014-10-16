#ifndef GENERATOR_PHASES_HPP
#define GENERATOR_PHASES_HPP

#include "silents.hpp"
#include "captures.hpp"

namespace Generator {
	typedef void(*Generate)(Move::Buffer&, const Board::Type&);
	typedef Boolspeed(*Maker)(Move::Type, Board::Type&);
	typedef void(*Unmaker)(Move::Type, Board::Type&);

    struct PhaseStruct {
        Generate generate;
        Maker make;
        Unmaker unmake;

        PhaseStruct(Generate generate, Maker make, Unmaker unmake):
            generate(generate), make(make), unmake(unmake)
        {}
    };

	const UNumspeed phaseCount = 2;
    using Phases = PhaseStruct[phaseCount];

    template <Color::Type COLOR>
	inline PhaseStruct* phases() {
		static const Phases result = {
            PhaseStruct(Captures::phase<COLOR>, Move::makeCapture<COLOR>, Move::unmakeCapture<COLOR>),
            PhaseStruct(Silents::phase<COLOR>,  Move::makeSilent<COLOR>,  Move::unmakeSilent<COLOR>)
		};

		return (PhaseStruct*)result;
    }

    #define forPhases(PHASE, PHASES) \
        UNumspeed G_M_I = 0; \
        auto PHASE = PHASES[G_M_I]; \
		for (; G_M_I < Generator::phaseCount; ++G_M_I, PHASE = PHASES[G_M_I])
}

#endif /* GENERATOR_PHASES_HPP */
