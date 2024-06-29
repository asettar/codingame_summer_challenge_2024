#include "header.hpp"
/*start*/
struct divingGame {
    int points[3], combo[3];
    double medals[3];
    int unused, playerIdx;
    string gpu;
    
    divingGame() {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> points[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> combo[i]; cin.ignore();
        cin >> unused; cin.ignore();
        playerIdx = game.playerIdx;
    }

    bool guarentedWinLose() {
        int opp1 = points[(playerIdx + 1) % 3];
        int opp2 = points[(playerIdx + 2) % 3];
        int combo1 = combo[(playerIdx + 1) % 3];
        int combo2 = combo[(playerIdx + 2) % 3];
        for(int i = 1; i <= gpu.size(); i++) {
            combo1++, combo2++;
            opp1 += combo1;
            opp2 += combo2;
        }

        int myPoints = points[playerIdx];
        int myCombo = combo[playerIdx];
        bool ok = max(opp1, opp2) <= points[playerIdx];
        for(int i = 1; i <= gpu.size(); i++) {
            myCombo++;
            myPoints += myCombo;
        }
        ok |= (myPoints < min(points[(playerIdx + 1) % 3], points[(playerIdx + 2) % 3]));
        int pts = points[game.playerIdx];
        ok |= ((myPoints < points[(game.playerIdx + 1) % 3] && pts >= opp2) || (myPoints < points[(game.playerIdx + 2) % 3] && pts >= opp1));
        game.minDiving = min(points[playerIdx], max(points[(playerIdx + 1) % 3], points[(playerIdx + 2) % 3]));
        game.maxDiving = min(myPoints, max(opp1, opp2));
        return (ok);
    }

    void    nextTurn() {
        gpu.erase(gpu.begin());
    }
    void    simulate(int idx, char move) {
        if (gpu == "GAME_OVER") return ;
        if (move == gpu[0])
            combo[idx]++;
        else combo[idx] = 0;
        points[idx] += combo[idx];
    }

    bool    isTerminal() {
        return gpu.empty() || gpu == "GAME_OVER";
    }

    double getScore() {
        double score = double(points[game.playerIdx]) - game.diving->points[game.playerIdx];
        return score;
    }

    int getMaximumReward(int idx) {
        if (gpu == "GAME_OVER") return 0;
        int comb = combo[idx], pts = 0;
        for(int i = 0; i < gpu.size(); i++) {
            comb++;
            pts += comb;
        }
        return pts;
    }

    int getMaximumNeed() {
        if (gpu == "GAME_OVER") return 0;
        int opp1 = getMaximumReward((game.playerIdx + 1) % 3) + points[(game.playerIdx + 1) % 3];
        int opp2 = getMaximumReward((game.playerIdx + 2) % 3) + points[(game.playerIdx + 2) % 3];
        return max(opp1, opp2) - points[game.playerIdx];
    }

    int getCurPlace() {
        int myPoints = points[game.playerIdx] + combo[game.playerIdx];
        int opp1Points = points[(game.playerIdx + 1) % 3] + combo[(game.playerIdx + 1) % 3];
        int opp2Points = points[(game.playerIdx + 2) % 3] + combo[(game.playerIdx + 2) % 3];
        if (myPoints >= max(opp1Points, opp2Points)) return 1;
        else if (myPoints >= min(opp1Points, opp2Points)) return 2;
        else return 3;
    }

    void    distributeMedals() {
        vector<pair<double, int>> scores;
        for(int i = 0; i < 3; i++) {
            scores.push_back({points[i], i});
        }
        sort(scores.rbegin(), scores.rend());
        medals[scores[0].second] = 1;
        if (scores[1].first == scores[0].first) medals[scores[1].second] = 1;
        else medals[scores[1].second] = 0.5;
        if (scores[2].first == scores[0].first) medals[scores[2].second] = 1;
        else if (scores[2].first == scores[1].first)
            medals[scores[2].second] = 0.5;
        else medals[scores[2].second] = 0;
    }

    double  getMyMedals() {
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