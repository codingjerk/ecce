#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include "core.hpp"

#define STATISTIC
#undef STATISTIC

#ifdef STATISTIC
    #include <iostream>
    #include <iomanip>
#endif

namespace Statistic {
    #ifdef STATISTIC 
        struct Data {
            UNummax alphaUpped;
            UNummax alphaPruned;
            UNummax betaPruned;
            UNummax goingToNextPhase;
            UNummax noLegalMoves;
            UNummax returnedAlpha;
            UNummax repeatPruned;
            UNummax negaScoutFails;

            UNummax quiesceAlphaUpped;
            UNummax quiesceAlphaPruned;
            UNummax quiesceBetaPruned;
            UNummax noQuiesceMoves;

            UNummax quiesceNodes;
            UNummax totalNodes;
        };

        static Data master;
    #endif

    inline void alphaUpped() {
        #ifdef STATISTIC
            ++master.alphaUpped;
        #endif
    }

    inline void alphaPruned() {
        #ifdef STATISTIC
            ++master.alphaPruned;
        #endif
    }

    inline void betaPruned() {
        #ifdef STATISTIC
            ++master.betaPruned;
        #endif
    }
    
    inline void goingToNextPhase() {
        #ifdef STATISTIC
            ++master.goingToNextPhase;
        #endif
    }

    inline void noLegalMoves() {
        #ifdef STATISTIC
            ++master.noLegalMoves;
        #endif
    }

    inline void returnedAlpha() {
        #ifdef STATISTIC
            ++master.returnedAlpha;
        #endif
    }

    inline void repeatPruned() {
        #ifdef STATISTIC
            ++master.repeatPruned;
        #endif
    }

    inline void negaScoutFailed() {
        #ifdef STATISTIC
            ++master.negaScoutFails;
        #endif
    }

    inline void quiesceAlphaUpped() {
        #ifdef STATISTIC
            ++master.quiesceAlphaUpped;
        #endif
    }

    inline void quiesceAlphaPruned() {
        #ifdef STATISTIC
            ++master.quiesceAlphaPruned;
        #endif
    }

    inline void quiesceBetaPruned() {
        #ifdef STATISTIC
            ++master.quiesceBetaPruned;
        #endif
    }

    inline void noQuiesceMoves() {
        #ifdef STATISTIC
            ++master.noQuiesceMoves;
        #endif
    }

    inline void increaseQuiesceNodes() {
        #ifdef STATISTIC
            ++master.quiesceNodes;
        #endif
    }

    inline void increaseNodes() {
        #ifdef STATISTIC
            ++master.totalNodes;
        #endif
    }

    inline void flush() {
        #ifdef STATISTIC
            master.alphaUpped         = 0;
            master.alphaPruned        = 0;
            master.betaPruned         = 0;
            master.goingToNextPhase   = 0;
            master.noLegalMoves       = 0;
            master.returnedAlpha      = 0;
            master.repeatPruned       = 0;
            master.negaScoutFails     = 0;

            master.quiesceAlphaUpped  = 0;
            master.quiesceAlphaPruned = 0;
            master.quiesceBetaPruned  = 0;
            master.noQuiesceMoves     = 0;

            master.quiesceNodes       = 0;
            master.totalNodes         = 0;
        #endif
    }

    inline void print() {
        #ifdef STATISTIC
            #define INFOPERCENT(PARAM, TOTAL) std::cout << "info string " << std::setw(20) << #PARAM ": " << std::setw(15) << master.PARAM << " (" << (double(master.PARAM * 100) / master.TOTAL) << "%)\n";
            #define INFO(PARAM) std::cout << "info string " << std::setw(20) << #PARAM ": " << std::setw(15) << master.PARAM << "\n";

            INFOPERCENT(alphaUpped, totalNodes);
            INFOPERCENT(alphaPruned, totalNodes);
            INFOPERCENT(betaPruned, totalNodes);
            INFOPERCENT(goingToNextPhase, totalNodes);
            INFOPERCENT(noLegalMoves, totalNodes);
            INFOPERCENT(returnedAlpha, totalNodes);
            INFOPERCENT(repeatPruned, totalNodes);
            INFOPERCENT(negaScoutFails, totalNodes);

            INFOPERCENT(quiesceAlphaUpped, quiesceNodes);
            INFOPERCENT(quiesceAlphaPruned, quiesceNodes);
            INFOPERCENT(quiesceBetaPruned, quiesceNodes);
            INFOPERCENT(noQuiesceMoves, quiesceNodes);

            INFO(quiesceNodes);
            INFO(totalNodes);
        #endif
    }
}

#endif /* STATISTIC_HPP */
