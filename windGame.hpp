#pragma once
#include "gamesInfo.hpp"
/*start*/
struct windGame {
    string gpu;
    int unused;
    int posx[3], posy[3];
    int player_x, player_y;
    int player_idx;
    int dx[4] = {1, 0, 0, -1};
    int dy[4] = {0, -1, 1, 0};
    double medals[3];

    windGame() {}
    windGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) {
            cin >> posx[i]; cin.ignore();
            cin >> posy[i]; cin.ignore();
        }
        cin >> unused; cin.ignore();
        if (gpu == "GAME_OVER") game.windCnt++;
        player_x = posx[game.player_idx];
        player_y = posy[game.player_idx];
        player_idx = game.player_idx;
    }

    int getMovesLeft() {
        int sumLeft = 0;
        for(int i = 1; i < gpu.size(); i++)
            sumLeft += gpu[i] - '0';
        return sumLeft;
    }

    double getDistance(int x, int y) {
        return sqrt(x * x + y * y);
    }
    int minimumDistance(int i, int x, int y, int winddp[15][41][41]) {
        if (i == gpu.size()) {
            return getDistance(x, y);
        }

        if (winddp[i][x + 20][y + 20] != -1)
            return winddp[i][x + 20][y + 20];
        int ans = 2e9;
        int k = 0;
        for(char c : "RUDL") {
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

    void    play(gamesInfo &game) {
        if (gpu == "GAME_OVER") return ;
        cerr << "-----Wind---------------\n";
        cerr << gpu << endl;

        int i = 0 ;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        int winddp[15][41][41];
        memset(winddp, -1, sizeof(winddp));
        for (char c : "RUDL") {
            int nx = player_x + (gpu[0] - '0') * dx[i];
            int ny = player_y + (gpu[0] - '0') * dy[i];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            if (minimumDistance(0, player_x, player_y, winddp) == minimumDistance(1, nx, ny, winddp))
            game.movesCnt[mp[c]] += 5;
            i++;
        }
    }

    void    simulate(int idx, char move) {

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
        gpu.erase(gpu.begin());
    }

    bool    isTerminal() {
        return gpu.empty();
    }

    void    distributeMedals() {
        vector<pair<double, int>> distances;
        for(int i = 0; i < 3; i++) {
            double curDist = getDistance(posx[(game.player_idx + i) % 3], posy[(game.player_idx + i) % 3]);
            distances.push_back({curDist, (game.player_idx + i) % 3});
        }
        sort(distances.begin(), distances.end());
        medals[distances[0].second] = 1;
        if (distances[1].first == distances[0].first) medals[distances[1].second] = 1;
        else medals[distances[1].second] = 0.5;
        if (distances[2].first == distances[0].first) medals[distances[2].second] = 1;
        else if (distances[2].first == distances[1].first)
            medals[distances[1].second] = 0.5;
        else medals[distances[1].second] = 0;
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
