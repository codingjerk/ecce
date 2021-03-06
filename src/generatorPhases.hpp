#ifndef GENERATOR_PHASES_HPP
#define GENERATOR_PHASES_HPP

#include "silents.hpp"
#include "captures.hpp"
#include "recaptures.hpp"
#include "uselesses.hpp"
#include "hash.hpp"

namespace Generator {
	typedef void(*Generate)(Move::Buffer&, const Board::Type&);
	typedef Boolspeed(*Maker)(Move::Type, Board::Type&);
	typedef void(*Unmaker)(Move::Type, Board::Type&);

	enum class PhaseType {
		Hash,
		Recapture,
		Capture,
		Silent,
		Useless
	};

    struct Phase {
        Generate generate;
		Maker make;
		Unmaker unmake;
		PhaseType type;

		Phase(Generate generate, Maker make, Unmaker unmake, PhaseType type) : generate(generate), make(make), unmake(unmake), type(type) {}
    };

    const UNumspeed phaseCount = 5;
    using Phases = Phase[phaseCount];

    template <Color::Type COLOR>
    inline Phase* phases() {
        static const Phases result = {
            Phase(Hash::phase,              Move::make<COLOR>,        Move::unmake<COLOR>, PhaseType::Hash),
			Phase(Recaptures::phase<COLOR>, Move::makeCapture<COLOR>, Move::unmakeCapture<COLOR>, PhaseType::Recapture),
            Phase(Captures::phase<COLOR>,   Move::makeCapture<COLOR>, Move::unmakeCapture<COLOR>, PhaseType::Capture),
            Phase(Silents::phase<COLOR>,    Move::makeSilent<COLOR>,  Move::unmakeSilent<COLOR>, PhaseType::Silent),
            Phase(Uselesses::phase<COLOR>,  Move::make<COLOR>,        Move::unmake<COLOR>, PhaseType::Useless)
        };

        return (Phase*)result;
    }

    const UNumspeed quiescePhaseCount = 2;
    using QuiescePhases = Phase[quiescePhaseCount];

    template <Color::Type COLOR>
    inline Phase* quiescePhases() {
        static const QuiescePhases result = {
			Phase(Recaptures::phase<COLOR>, Move::makeCapture<COLOR>, Move::unmakeCapture<COLOR>, PhaseType::Recapture),
            Phase(Captures::phase<COLOR>,   Move::makeCapture<COLOR>, Move::unmakeCapture<COLOR>, PhaseType::Capture)
        };

        return (Phase*)result;
    }

    #define forPhases(PHASE, PHASES) \
        UNumspeed G_M_I = 0; \
        auto PHASE = PHASES[G_M_I]; \
		for (; G_M_I < Generator::phaseCount; ++G_M_I, PHASE = PHASES[G_M_I])

    #define forPerftPhases(PHASE, PHASES) \
        UNumspeed G_M_I = 1; \
        auto PHASE = PHASES[G_M_I]; \
		for (; G_M_I < Generator::phaseCount; ++G_M_I, PHASE = PHASES[G_M_I])

    #define forQuiescePhases(PHASE, PHASES) \
        UNumspeed G_M_I = 0; \
        auto PHASE = PHASES[G_M_I]; \
		for (; G_M_I < Generator::quiescePhaseCount; ++G_M_I, PHASE = PHASES[G_M_I])
}

#endif /* GENERATOR_PHASES_HPP */
