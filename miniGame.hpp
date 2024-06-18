#pragma once 
#include "gamesInfo.hpp"

gamesInfo game;

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
        this->divingEnd = 0;
    }

    void    playMove(string &move) {
        if (!hurdleEnd)
            updateHurdle();
        if (!windEnd)
            updateWind();
        if (!divingEnd)
            updateDiving();
        checkGameEnds();
    }
    bool    isTerminal() {
        return (hurdleEnd && windEnd && divingEnd);
    }

private :
    void    updateHurdle(string move) {
        hurdle->simulate(game.player_idx, move[0]);
        hurdle->simulate((game.player_idx + 1) % 3, move[1]);
        hurdle->simulate((game.player_idx + 2) % 3, move[2]);
    }
    void    updateWind(string move) {
        wind->simulate(game.player_idx, move[0]);
        wind->simulate((game.player_idx + 1) % 3, move[1]);
        wind->simulate((game.player_idx + 2) % 3, move[2]);
    }
    void    updateDiving(string move) {
        diving->simulate(game.player_idx, move[0]);
        diving->simulate((game.player_idx + 1) % 3, move[1]);
        diving->simulate((game.player_idx + 2) % 3, move[2]);
    }
    void    checkGameEnds() {
        if (!hurdleEnd && hurdle->isTerminal()) {
            hurdleEnd = 1;
            hurdle->distributeMedals();
        }
        if (!windEnd && wind->isTerminal()) {
            windEnd = 1;
            wind->distributeMedals();
        }
        if (!divingEnd && diving->isTerminal()) {
            divingEnd = 1;
            diving->distributeMedals();
        }
    }
    
};