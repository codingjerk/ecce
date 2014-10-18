#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include "core.hpp"

#define STATISTIC
// #undef STATISTIC

#ifdef STATISTIC
    #include <iostream>
    #include <iomanip>
#endif

namespace Statistic {
    struct Data {
        UNummax alphaUpped;
        UNummax alphaPruned;
        UNummax betaPruned;
        UNummax goingToNextPhase;
        UNummax noLegalMoves;
        UNummax returnedAlpha;
        UNummax totalNodes;
    };

    Data master;

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

    inline void increaseNodes() {
        #ifdef STATISTIC
            ++master.totalNodes;
        #endif
    }

    inline void flush(Data& data) {
        #ifdef STATISTIC
            data.alphaUpped       = 0;
            data.alphaPruned      = 0;
            data.betaPruned       = 0;
            data.goingToNextPhase = 0;
            data.noLegalMoves     = 0;
            data.returnedAlpha    = 0;
            data.totalNodes       = 0;
        #endif
    }

    inline void print(Data& data) {
        #ifdef STATISTIC
            #define INFOPERCENT(PARAM) std::cout << "info string " << std::setw(20) << #PARAM ": " << std::setw(15) << data.PARAM << " (" << (double(data.alphaUpped * 100) / data.totalNodes) << "%)\n";
            #define INFO(PARAM) std::cout << "info string " << std::setw(20) << #PARAM ": " << std::setw(15) << data.PARAM << "\n";

            INFOPERCENT(alphaUpped);
            INFOPERCENT(alphaPruned);
            INFOPERCENT(betaPruned);
            INFOPERCENT(goingToNextPhase);
            INFOPERCENT(noLegalMoves);
            INFOPERCENT(returnedAlpha);
            INFO(totalNodes);
        #endif
    }
}

#endif /* STATISTIC_HPP */
