#pragma once
#include "gamesInfo.hpp"
/*start*/

struct divingGame {
    int points[3], combo[3];
    int unused, player_idx;
    string gpu;

    divingGame() {}
    divingGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> points[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> combo[i]; cin.ignore();
        cin >> unused; cin.ignore();
        if (gpu == "GAME_OVER") game.diveCnt++;
        player_idx = game.player_idx;
    }

    bool guarentedWin() {
        int opp1 = points[(player_idx + 1) % 3];
        int opp2 = points[(player_idx + 2) % 3];
        int combo1 = combo[(player_idx + 1) % 3];
        int combo2 = combo[(player_idx + 2) % 3];
        for(int i = 1; i <= gpu.size(); i++) {
            combo1++, combo2++;
            opp1 += combo1;
            opp2 += combo2;
        }
        cerr << opp1 << ' ' << opp2 << endl;
        return (max(opp1, opp2) < points[player_idx]);
    }

    void    play(gamesInfo &game) {
        if (gpu == "GAME_OVER") return ;
        if (game.diveCnt >= 3) return ;
        if (gpu.size() == 1) game.diveCnt++;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        if (guarentedWin()) return ;
        double score = combo[player_idx] + 4;
        cerr << "------Diving----------\n";
        cerr << gpu << ' ' << gpu.size() << endl;
        cerr << "Score : " << score << endl;
        game.movesCnt[mp[gpu[0]]] += score;
    }
};

/*end*/