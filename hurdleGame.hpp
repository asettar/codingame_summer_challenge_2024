#pragma once
#include "gamesInfo.hpp"
/*start*/

using namespace std;

struct hurdleGame {
    string gpu;
    int pos[3], stun[3];
    int unused, player_pos;

    hurdleGame() {}

    hurdleGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> pos[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> stun[i]; cin.ignore();
        cin >> unused; cin.ignore();
        player_pos = pos[game.player_idx];
        if (gpu == "GAME_OVER") game.hurdleCnt++;

    }
    // hurdleGame = 
    void    play(gamesInfo& game) {
        if (gpu == "GAME_OVER") return ;
        if (game.diveCnt < 3) return ;
        cerr << "-----Hurdle-----------\n";
        cerr << gpu << endl;
        cerr << "   pos: " << player_pos << endl;
        
        if (stun[player_pos] > 0) return;
        int mv[4] = {3, 2, 2, 1};
        int k = 0;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            int score = 0;
            for(int j = player_pos + 1; j <= player_pos + mv[k]; j++) {
                if (gpu[j] == '#') {
                    if (s == "UP" && j == player_pos + 1) score += 0;
                    else {
                        score -= 3 * player_pos;
                        break;
                    }
                }
                if (j >= gpu.size() - 1) {
                    score += 15;
                    break;
                }
                score++;
            }
            if (score > 0) score += player_pos;
            game.movesCnt[s] += score;
            cerr << "   Score :" << s << ' ' << score << endl;
            k++;
        }
        cerr << '\n';
    }
};

/*end*/
