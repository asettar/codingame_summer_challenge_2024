#include "header.hpp"
/*start*/
struct hurdleGame {
    string gpu;
    int pos[3], stun[3];
    double  medals[3];
    int unused, player_pos; 

    hurdleGame() {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> pos[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> stun[i]; cin.ignore();
        cin >> unused; cin.ignore();
        player_pos = pos[game.playerIdx];
        if (!player_pos) {
            game.hurdledp = vector<int>(30, -1);
        }
    }
   
    int getMyPos() {
        return pos[game.playerIdx];
    }
    int getMinMoves(int pos) {
        if (pos >= 29) return 0;
        if (game.hurdledp[pos] != -1) return game.hurdledp[pos]; 

        int ans = 2e9;
        if (gpu[pos + 1] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 1));
        if (gpu[pos + 2] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 2));
        if (gpu[pos + 1] != '#' && gpu[pos + 2] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 2));
        if (gpu[pos + 1] != '#' && gpu[pos + 2] != '#' && gpu[pos + 3] != '#')
            ans = min(ans, 1 + getMinMoves(pos + 3));
        return game.hurdledp[pos] = ans;
    }

    int getMaxMoves(int idx) {
        if (gpu == "GAME_OVER") return 0;
        int ret = stun[idx];
        for(int i = pos[idx] + 1; i <= 29; i++) {
            ret++;
            if (gpu[i] == '#') ret += 2;
        }
        return ret;
    }
    
    void    play() {
        string ans = "RIGHT";
        if (gpu[player_pos + 1] != '#' && getMinMoves(player_pos) == 1 + getMinMoves(player_pos + 1))
            ans = "LEFT";
        else if (gpu[player_pos + 2] != '#' && getMinMoves(player_pos) == 1 + getMinMoves(player_pos + 2))
            ans = "UP";
        else if (gpu[player_pos + 1] != '#' && gpu[player_pos + 2] != '#') {
            if (getMinMoves(player_pos) == 1 + getMinMoves(player_pos + 2))
                ans = "DOWN";
            else ans = "RIGHT";
        }
        cout << ans << endl;
    }

    bool guarentedWinLose(gamesInfo &game) {
        
        int myMinNeed =  getMinMoves(pos[game.playerIdx]) + stun[game.playerIdx];
        if (100 - game.turn < myMinNeed) return 1;
        int myMaxNeed = getMaxMoves(game.playerIdx);
        int opp1MinNeed = stun[(game.playerIdx + 1) % 3] + getMinMoves(pos[(game.playerIdx + 1) % 3]);
        int opp2MinNeed = stun[(game.playerIdx + 2) % 3] + getMinMoves(pos[(game.playerIdx + 2) % 3]);
        int oppMinNeed = min(opp2MinNeed, opp1MinNeed);
        if (oppMinNeed >= myMaxNeed) return 1;
        int opp1MaxNeed = getMaxMoves((game.playerIdx + 1) % 3);
        int opp2MaxNeed = getMaxMoves((game.playerIdx + 2) % 3);
        int oppMaxNeed = max(opp1MaxNeed, opp2MaxNeed);
        if (oppMaxNeed < myMinNeed) return 1;

        if ((myMaxNeed < opp1MinNeed && myMinNeed >= opp2MaxNeed) || (myMaxNeed < opp2MinNeed && myMinNeed >= opp1MaxNeed)) return 1;
        game.minHurdleNeed = max(oppMinNeed, myMinNeed);
        game.maxHurdleNeed = min(oppMaxNeed, myMaxNeed);
        return 0;
    }

    void    dbg() {
        cerr << "dbg\n";
        for(int i = 0; i < 3; i++) {
            cerr <<"player idx: " << i << ' ' << stun[i] << ' ' << pos[i] << endl;
        }
    }
    void    simulate(int idx, char move) {
        if (gpu == "GAME_OVER") return ;
        if (stun[idx]) {
            stun[idx]--;
            return ;
        }
        // cerr << "Hurdle :" << idx << ' ' << stun[idx] << ' ' << pos[idx] << ' ' << gpu << endl;
        int mv[4] = {3, 2, 2, 1};
        string moves = "RUDL";
        int k = moves.find(move);
        int start = pos[idx];
        for(int i = start + 1; i <= start + mv[k]; i++) {
            pos[idx]++;
            if (i >= 29)
                break;
            if (gpu[i] == '#') {
                if (move != 'U' || i != start + 1) {
                    stun[idx] = 2;
                    break;
                }
            }
        }
    }

    bool isTerminal() {
        return (pos[game.playerIdx] >= 29 || gpu == "GAME_OVER");
    }
    void    distributeMedals() {
        vector<pair<int, int>> positions;
        for(int i = 0; i < 3; i++) {
            positions.push_back({pos[i], i});
        }
        sort(positions.rbegin(), positions.rend());
        medals[positions[0].second] = 1;
        if (positions[1].first == positions[0].first) medals[positions[1].second] = 1;
        else medals[positions[1].second] = 0.5;
        if (positions[2].first == positions[0].first) medals[positions[2].second] = 1;
        else if (positions[2].first == positions[1].first)
            medals[positions[2].second] = 0.5;
        else medals[positions[2].second] = 0;
    }

    int getCurPlace() {
        int myPos = pos[game.playerIdx] - stun[game.playerIdx];
        int opp1Pos = pos[(game.playerIdx + 1) % 3] - stun[(game.playerIdx + 1) % 3];
        int opp2Pos = pos[(game.playerIdx + 2) % 3] - stun[(game.playerIdx + 2) % 3];
        if (myPos >= max(opp1Pos, opp2Pos)) return 1;
        else if (myPos >= min(opp1Pos, opp2Pos)) return 2;
        else return 3;
    }
    double  getMyMedals() {
        for(int i = 0; i < 3; i++) {
            if (pos[i] >= 29) return medals[game.playerIdx];
        }
        distributeMedals();
        return medals[game.playerIdx];
    }
    double getOpp1Medals() {
        return medals[(game.playerIdx + 1) % 3];
    }
    double getOpp2Medals() {
        return medals[(game.playerIdx + 2) % 3];
    }
};
/*end*/
