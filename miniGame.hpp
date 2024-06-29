#include "header.hpp"
/*start*/
class miniGame {
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
    bool hurdleEnd, windEnd, divingEnd;
    double  minimumHurdleMoves;
    double  maximumHurdleMoves;
    double  maxDivingPoints;
    int turnsCnt;
public:
    miniGame(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        this->hurdle = new hurdleGame(*hurdle);
        this->wind = new windGame(*wind);
        this->diving = new divingGame(*diving);
        this->hurdleEnd = 0;
        this->windEnd = 0;
        this->divingEnd = 0;
        this->minimumHurdleMoves = hurdle->getMinMoves(hurdle->pos[game.playerIdx]);
        this->maximumHurdleMoves = min(25, hurdle->getMaxMoves(game.playerIdx));
        this->maxDivingPoints = diving->getMaximumReward(game.playerIdx) + diving->points[game.playerIdx];
        this->turnsCnt = 0;
    }

     miniGame(const miniGame& other) {
        hurdle = new hurdleGame(*other.hurdle);
        wind = new windGame(*other.wind);
        diving = new divingGame(*other.diving);
        hurdleEnd = other.hurdleEnd;
        windEnd = other.windEnd;
        divingEnd = other.divingEnd;
        turnsCnt = other.turnsCnt;
        minimumHurdleMoves = other.minimumHurdleMoves;
        maximumHurdleMoves = other.maximumHurdleMoves;
        maxDivingPoints = other.maxDivingPoints;
    }
    void    dbg() {
        // cerr << hurdle->gpu << ' ' << wind->gpu << ' ' << diving->gpu << endl;
    }
    void    playMove(char move) {
        // cerr << move << ' ' << hurdleEnd << ' ' << windEnd << ' ' << divingEnd << endl;
        if (!hurdleEnd) {
            if (hurdle->gpu == "GAME_OVER" || turnsCnt == 25) hurdleEnd = 1;
            else {
                turnsCnt++;
                hurdle->simulate(game.playerIdx, move);
            }
        }
        if (!windEnd) {
            if (wind->gpu == "GAME_OVER") windEnd = 1;
            else {
                wind->simulate(game.playerIdx, move);
                wind->nextTurn();
            }
        }

        if (!divingEnd) {
            if (diving->gpu == "GAME_OVER") divingEnd = 1;
            else {
                diving->simulate(game.playerIdx, move);
                diving->nextTurn();
            }
        }
        checkGameEnds();
    }
    bool    isTerminal() {
        return ((hurdleEnd || hurdle->gpu == "GAME_OVER") && (windEnd || wind->gpu == "GAME_OVER")
             && (divingEnd || diving->gpu == "GAME_OVER"));
    }
    double  getMyScore() {
        double totScore = 0, curScore = 0;
        if (wind->gpu != "GAME_OVER") {
            curScore = wind->getScore();
            totScore += curScore * game.gamesOrder['E'];
        }
        if (diving->gpu != "GAME_OVER") {
            curScore = 1.0 - (double)(game.maxDiving - diving->points[game.playerIdx]) / (double)(game.maxDiving - game.minDiving);
            if (curScore > 1) curScore = 1;
            if (curScore < 0) curScore = 0;
            totScore += curScore * game.gamesOrder['D'];
        }
        if (hurdle->gpu != "GAME_OVER") {
            if (minimumHurdleMoves == turnsCnt) curScore = 1;
            else curScore = (double)(game.maxHurdleNeed - turnsCnt) / (game.maxHurdleNeed - game.minHurdleNeed);
            if (curScore > 1) curScore = 1;
            if (curScore < 0) curScore = 0;
            totScore += curScore * game.gamesOrder['H'];
        }
        return (totScore);
    }

private :
    void    checkGameEnds() {
        if (!hurdleEnd && hurdle->isTerminal())
            hurdleEnd = 1;
        if (!windEnd && wind->isTerminal())
            windEnd = 1;
        if (!divingEnd && diving->isTerminal())
            divingEnd = 1;
    }
};

void    setupGame(int gameNumber) {
    cin >> game.gpu; cin.ignore();
    
    if (gameNumber == 0)
        game.hurdle = new hurdleGame();
    else if (gameNumber == 1)
        game.wind = new windGame();
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else
        game.diving = new divingGame();
}
/*end*/