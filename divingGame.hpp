#pragma once
#include "gamesInfo.hpp"
/*start*/

struct divingGame {
    int points[3], combo[3];
    int medals[3];
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
        return (max(opp1, opp2) < points[player_idx]);
    }

    void    play(gamesInfo &game) {
        if (gpu == "GAME_OVER" || guarentedWin()) return ;
        cerr << "------Diving----------\n";
        cerr << gpu << ' ' << gpu.size() << endl;
        // cerr << "Score : " << score << endl;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        game.movesCnt[mp[gpu[0]]] += 4 + combo[player_idx] / 5;
    }

    void    simulate(int idx, char move) {
        if (move == gpu[0])
            combo[idx]++;
        else combo[idx] = 0;
        points[idx] += combo[idx];
        gpu.erase(gpu.begin());
    }

    bool    isTerminal() {
        return gpu.empty();
    }

    void    distribueMedals() {
        vector<pair<double, int>> scores;
        for(int i = 0; i < 3; i++) {
            scores.push_back({points[(game.player_idx + i) % 3], (game.player_idx + i) % 3});
        }
        sort(scores.rbegin(), scores.rend());
        medals[scores[0].second] = 1;
        if (scores[1].first == scores[0].first) medals[scores[1].second] = 1;
        else medals[scores[1].second] = 0.5;
        if (scores[2].first == scores[0].first) medals[scores[2].second] = 1;
        else if (scores[2].first == scores[1].first)
            medals[scores[1].second] = 0.5;
        else medals[scores[1].second] = 0;
    }

    double  getMyMedals() {
        return medals[game.player_idx];
    }
    double getOpp1Medals() {
        return medals[(game.player_idx + 1) % 3];
    }
    double getOpp2Medals() {
        return medals[(game.player_idx + 2) % 3];
    }
};

/*end*/