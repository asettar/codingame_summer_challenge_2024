#include "header.hpp"
/*start*/
struct windGame {
    string gpu;
    int unused;
    int posx[3], posy[3];
    int player_x, player_y;
    int playerIdx;
    int dx[4] = {1, 0, 0, -1};
    int dy[4] = {0, -1, 1, 0};
    double medals[3];

    windGame() {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) {
            cin >> posx[i]; cin.ignore();
            cin >> posy[i]; cin.ignore();
        }
        cin >> unused; cin.ignore();
        player_x = posx[game.playerIdx];
        player_y = posy[game.playerIdx];
        playerIdx = game.playerIdx;
    }

    int getMovesLeft() {
        int sumLeft = 0;
        for(int i = 1; i < gpu.size(); i++)
            sumLeft += gpu[i] - '0';
        return sumLeft;
    }

    bool guarentedWinLose() {
        float winddp[16][41][41];
        // memset(winddp, -1, sizeof(winddp));
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 41; j++) {
                for(int k = 0; k < 41; k++)  winddp[i][j][k] = -1;
            }
        }
        float myMaxDist = maximumDistance(0, posx[game.playerIdx], posy[game.playerIdx], winddp);
        float opp1MaxDist = maximumDistance(0, posx[(game.playerIdx + 1) % 3], posy[(game.playerIdx + 1) % 3], winddp);
        float opp2MaxDist =  maximumDistance(0, posx[(game.playerIdx + 2) % 3], posy[(game.playerIdx + 2) % 3], winddp);
        float oppMaxDist = max(opp2MaxDist, opp1MaxDist);
        // memset(winddp, -1, sizeof(winddp));

        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 41; j++) {
                for(int k = 0; k < 41; k++)  winddp[i][j][k] = -1;
            }
        }

        float opp1MinDist = minimumDistance(0, posx[(game.playerIdx + 1) % 3], posy[(game.playerIdx + 1) % 3], winddp);
        float opp2MinDist = minimumDistance(0, posx[(game.playerIdx + 2) % 3], posy[(game.playerIdx + 2) % 3], winddp);
        float oppMinDist = min(opp1MinDist, opp2MinDist);
        float   myMinDist =  minimumDistance(0, posx[game.playerIdx], posy[game.playerIdx], winddp);
        bool ok = (myMaxDist <= oppMinDist);
        ok |= (myMinDist > oppMaxDist);
    
        if (ok) return 1;
        bool can = 1;
        int k = 0;
         for(char c : string("RUDL")) {
            int nx = posx[game.playerIdx] + (gpu[0] - '0') * dx[k];
            int ny = posy[game.playerIdx] +  (gpu[0] - '0') * dy[k];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            can &= (abs(minimumDistance(1, nx, ny, winddp) - myMinDist) <= 1e-4);
            k++;
        }
        // ok |= can;
        game.maxWindDist = min(myMaxDist, oppMaxDist);
        game.minWindDist = max(oppMinDist, myMinDist);

        // guarentedSilver;
        if ((myMaxDist < opp1MinDist && myMinDist >= opp2MaxDist) || (myMaxDist < opp2MinDist && myMinDist >= opp1MaxDist)) ok = 1;

        return ok;
    }
    double getDistance(int x, int y) {
        return sqrt(x * x + y * y);
    }
    float minimumDistance(int i, int x, int y, float winddp[16][41][41]) {
        if (i == gpu.size()) {
            return getDistance(x, y);
        }

        if (winddp[i][x + 20][y + 20] != -1)
            return winddp[i][x + 20][y + 20];
        float ans = 2e9;
        int k = 0;
        for(char c : string("RUDL")) {
            int nx = x + (gpu[i] - '0') * dx[k];
            int ny = y +  (gpu[i] - '0') * dy[k];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            ans = min(ans, minimumDistance(i + 1, nx, ny, winddp));
            k++;
        }
        return winddp[i][x + 20][y + 20] = ans;
    }

    float  maximumDistance(int i, int x, int y, float winddp[16][41][41]) {
        if (i == gpu.size()) {
            return getDistance(x, y);
        }

        if (winddp[i][x + 20][y + 20] != -1)
            return winddp[i][x + 20][y + 20];
        float ans = -2e9;
        int k = 0;
        for(char c : string("RUDL")) {
            int nx = x + (gpu[i] - '0') * dx[k];
            int ny = y +  (gpu[i] - '0') * dy[k];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            ans = max(ans, maximumDistance(i + 1, nx, ny, winddp));
            k++;
        }
        return winddp[i][x + 20][y + 20] = ans;
    }

    void    nextTurn() {
        gpu.erase(gpu.begin());
    }
    void    simulate(int idx, char move) {
        // cerr << "Wind: " << idx << ' ' << gpu << endl;
        if (gpu == "GAME_OVER") return ;
        string moves = "RUDL";
        int k = moves.find(move);
        int nx = posx[idx] + (gpu[0] - '0') * dx[k];
        int ny = posy[idx] +  (gpu[0] - '0') * dy[k];
        nx = min(nx, 20);
        nx = max(nx, -20);
        ny = min(ny, 20);
        ny = max(ny, -20);

        posx[idx] = nx;
        posy[idx] = ny;
    }

    void    play() {
        if(gpu == "GAME_OVER") {
            cout << "RIGHT\n";
            return ;
        }
        float winddp[16][41][41];
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 41; j++) {
                for(int k = 0; k < 41; k++) 
                    winddp[i][j][k] = -1;
            }
        }
        float dis = 1e9;
        char bestAct = 'U';
        int i = 0;
        for (char c : {'R', 'U', 'D', 'L'}) {
            int nx = posx[game.playerIdx] + (gpu[0] - '0') * dx[i];
            int ny = posy[game.playerIdx] + (gpu[0] - '0') * dy[i];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            float cDis = minimumDistance(1, nx, ny, winddp);
            if (cDis < dis)
            {
                dis = cDis;
                bestAct = c;
            }
            i++;
        }
        cout << mp[bestAct] << endl;
    }

    bool    isTerminal() {
        return gpu.empty() || gpu == "GAME_OVER";
    }

    double  getScore() {
        if (gpu == "GAME_OVER") return 0;
        if (game.maxWindDist == game.minWindDist) return 1;
        float   dist = getDistance(posx[game.playerIdx], posy[game.playerIdx]);
        double  score = (game.maxWindDist - dist) / (game.maxWindDist - game.minWindDist);
        if (score > 1) score = 1;
        if (score < 0) score = 0;
        return score;
    }
    int getCurPlace() {
        if (gpu.size() > 10) return 3;
        else if (gpu.size() > 8) return 2;
        return 1;
    }
    void    distributeMedals() {
        vector<pair<double, int>> distances;
        for(int i = 0; i < 3; i++) {
            double curDist = getDistance(posx[i], posy[i]);
            distances.push_back({curDist, i});
        }
        sort(distances.begin(), distances.end());
        medals[distances[0].second] = 1;
        if (distances[1].first == distances[0].first) medals[distances[1].second] = 1;
        else medals[distances[1].second] = 0.5;
        if (distances[2].first == distances[0].first) medals[distances[2].second] = 1;
        else if (distances[2].first == distances[1].first)
            medals[distances[2].second] = 0.5;
        else medals[distances[2].second] = 0;
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