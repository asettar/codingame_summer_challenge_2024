#pragma once 
#include "gamesInfo.hpp"

class miniGame {
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
    bool hurdeEnd, windEnd, divingEnd;
public:
    miniGame(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        this->hurdle = new hurdleGame(*hurdle);
        this->wind = new windGame(*wind);
        this->diving = new divingGame(*diving);
        this->hurdeEnd = 0;
        this->windEnd = 0;
        this->dinvingEnd = 0;
    }
    void    playMove(string &move) {
        if (!hurdleEnd)
            updateHurdle();
        if (!windEnd)
            updateWind();
        if (!divingEnd)
            updateDiving
    }
    bool    isTerminal() {
        return (hurdleEnd && windEnd && divingEnd);
    }

private :
    void    updateHurdle(string move) {

    }
    void    updateWind(string move) {

    }
    void    updateDiving(string move) {
        
    }
};