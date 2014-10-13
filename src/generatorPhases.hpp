#ifndef GENERATOR_PHASES_HPP
#define GENERATOR_PHASES_HPP

#include "silents.hpp"
#include "captures.hpp"

namespace Generator {
	typedef void(*Phase)(Move::Buffer&, const Board::Type&);
	// @TODO: Make and Unmake for every phase: typedef void(*Maker)(Move::Type, Board::Type&);
	//										   typedef void(*Unmaker)(Move::Type, Board::Type&);
	const UNumspeed phaseCount = 2;
    using Phases = Phase[phaseCount];

    template <Color::Type COLOR>
	inline Phase* phases() {
		static const Phases result = {
			Captures::phase<COLOR>,
			Silents::phase<COLOR>
		};

		return (Phase*)result;
    }

    #define forPhases(PHASE, PHASES) \
        UNumspeed G_M_I = 0; \
        auto PHASE = PHASES[G_M_I]; \
		for (; G_M_I < Generator::phaseCount; ++G_M_I, PHASE = PHASES[G_M_I])
}

#endif /* GENERATOR_PHASES_HPP */
