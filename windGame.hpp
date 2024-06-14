#pragma once
#include "gamesInfo.hpp"
/*start*/
struct windGame {
    string gpu;
    int unused;
    int posx[3], posy[3];
    int player_x, player_y;
    int player_idx;

    windGame() {}
    windGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) {
            cin >> posx[i]; cin.ignore();
            cin >> posy[i]; cin.ignore();
        }
        cin >> unused; cin.ignore();
        player_x = posx[game.player_idx];
        player_y = posy[game.player_idx];
        if (gpu == "GAME_OVER") game.windCnt++;
    }

    int getMovesLeft() {
        int sumLeft = 0;
        for(int i = 1; i < gpu.size(); i++)
            sumLeft += gpu[i] - '0';
        return sumLeft;
    }

    void    play(std::map<string, double> &movesCnt) {
        if (gpu == "GAME_OVER") return ;
        int sumLeft = getMovesLeft();
        if (sumLeft >= 15 || gpu.size() >= 6) return;
        cerr << "-----Wind---------------\n";
        cerr << gpu << endl;

        int power = gpu[0] - '0';
        int dx[4] = {1, 0, 0, -1};
        int dy[4] = {0, -1, 1, 0};
        int i = 0;
        cerr << "x:" << player_x << " y: " << player_y << " turnLeft:" << gpu.size()<< endl;
        map<int, vector<string>> best;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            int nx = posx[player_idx] + power * dx[i];
            int ny = posy[player_idx] + power * dy[i];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            i++;
            double  distance = sqrt(nx * nx + ny * ny); // we want it to be as little as possible
            double score = 400.0 / (distance * gpu.size() + 1);
            cerr << "   score: "<< s << ' ' << score << ' ' << distance << endl;
            movesCnt[s] += score;
        }
        cerr << endl;

    }
};
/*end*/
