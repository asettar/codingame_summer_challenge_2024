#pragma once
#include "gamesInfo.hpp"
/*start*/

using namespace std;

struct hurdleGame {
    string gpu;
    int pos[3], stun[3];
    double  medals[3];
    int unused, player_pos; 
    hurdleGame() {}

    hurdleGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> pos[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> stun[i]; cin.ignore();
        cin >> unused; cin.ignore();
        player_pos = pos[game.player_idx];
        if (gpu == "GAME_OVER") game.hurdleCnt++;
        if (!player_pos) {
            game.hurdledp.resize(31);
            fill(game.hurdledp.begin(), game.hurdledp.end(), -1);
        }
    }

    int getMinMoves(int pos, gamesInfo &game) {
        if (pos >= gpu.size()) return 0;
        // left
        if (game.hurdledp[pos] != -1) return game.hurdledp[pos]; 

        int ans = 2e9;
        if (gpu[pos + 1] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 1, game));
        if (gpu[pos + 2] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 2, game));
        if (gpu[pos + 1] != '#' && gpu[pos + 2] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 2, game));
        if (gpu[pos + 1] != '#' && gpu[pos + 2] != '#' && gpu[pos + 3] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 3, game));
        return game.hurdledp[pos] = ans;
    }

    void    play(gamesInfo& game) {
        if (gpu == "GAME_OVER") return ;
        if (guarentedWin(game)) return ;
        cerr << "-----Hurdle-----------\n";
        cerr << gpu << endl;
        cerr << "   pos: " << player_pos << endl;

        if (stun[game.player_idx] > 0) return;
        cerr << gpu[player_pos + 2] << " min moves :" << getMinMoves(player_pos, game) << ' ' << getMinMoves(player_pos + 2, game) << endl;

    
        if (gpu[player_pos + 1] != '#' && getMinMoves(player_pos, game) == 1 + getMinMoves(player_pos + 1, game))
            game.movesCnt["LEFT"] += 5;
        if (gpu[player_pos + 2] != '#' && getMinMoves(player_pos, game) == 1 + getMinMoves(player_pos + 2, game))
            game.movesCnt["UP"] += 5;
        
        if (gpu[player_pos + 1] != '#' && gpu[player_pos + 2] != '#') {

            if (getMinMoves(player_pos, game) == 1 + getMinMoves(player_pos + 2, game))
                game.movesCnt["DOWN"] += 5;
            if (gpu[player_pos + 3] != '#' && getMinMoves(player_pos, game) == 1 + getMinMoves(player_pos + 3, game))
                game.movesCnt["RIGHT"] += 5;
        }
    }

    void    score(gamesInfo &game) {
        if (gpu == "GAME_OVER" || stun[game.player_idx] > 0 || guarentedWin(game)) return;
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
            if (score > 0)
                game.movesCnt[s] += score;
            k++;
        }
    }

    bool guarentedWin(gamesInfo &game) {
        int maxNeedForWin = stun[game.player_idx] + gpu.size() - player_pos;
        for(int i = player_pos + 1; i < gpu.size(); i++) {
            if (gpu[i] == '#') maxNeedForWin += 3;
        }
        
        int opps1Need = stun[(game.player_idx + 1) % 3] + getMinMoves(pos[(game.player_idx + 1) % 3], game);
        int opp2Need = stun[(game.player_idx + 2) % 3] + getMinMoves(pos[(game.player_idx + 2) % 3], game);
        return (maxNeedForWin <= opps1Need && maxNeedForWin <= opp2Need);
    }

    void    simulate(int idx, char move) {
        if (stun[idx]) {
            stun[idx]--;
            return ;
        }
        int mv[4] = {3, 2, 2, 1};
        string moves = "RUDL";
        int k = moves.find(move);
        int start = pos[idx] + 1;
        for(int i = start; i <= pos[idx] + mv[k]; i++) {
            if (i >= 29) break;
            if (gpu[i] == '#') {
                if (move != 'UP' || i != start) {
                    stun[idx] += 3;
                    break;
                }
            }
            pos[idx]++;
        }
    }

    bool isTermial() {
        for(int i = 0; i < 3; i++) {
            if (pos[i] >= 29) return true;
        }
    }
    void    distribueMedals() {
        vector<pair<int, int>> positions;
        for(int i = 0; i < 3; i++) {
            positions.push_back({pos[(game.player_idx + i) % 3], (game.player_idx + i) % 3});
        }
        sort(positions.rbegin(), positions.rend());
        medals[positions[0].second] = 1;
        if (positions[1].first >= 29) medals[positions[1].second] = 1;
        else medals[positions[1].second] = 0.5;
        if (positions[2].first >= 29) medals[positions[2].second] = 1;
        else if (positions[2].first == positions[1].first)
            medals[positions[1].second] = 0.5;
        else medals[positions[1].second] = 0;
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
