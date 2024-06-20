// file-Name: ./gamesInfo.hpp
// ########################
// ########################

#include <map>
#include <chrono>
#include <thread>
#include <string> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <sstream>
#include <cstring>
#include <set>
#include <random>

using namespace std;

struct hurdleGame;
struct windGame;
struct divingGame;

struct gamesInfo {
    std::string gpu;
    int divingPrevGpu = 0;
    int hurdleCnt = 0, windCnt = 0, diveCnt = 0;
    int player_idx;
    bool playingHurdle = 0;
    int turn = 0;
    map<std::string, double> movesCnt;
    int hurdleMedals[3];
    int windMedals[3];
    int rollerMedals[3];
    int divingMedals[3];
    int totalScore;
    vector<int> hurdledp;
    vector<string> permutations;
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
    map<char, double> gamesOrder;
    gamesInfo() {

    }
    void    setMedals(const string &scoreInfo) {
        std::vector<int> scores;
        std::stringstream ss(scoreInfo);
        int number;

        while (ss >> number)
            scores.push_back(number);
        totalScore = scores[0];
        for(int i = 1, k = 0; i < 4; i++, k++)
            hurdleMedals[k] = scores[i];
        for(int i = 4, k = 0; i < 7; i++, k++)
            windMedals[k] = scores[i];
        for(int i = 13, k = 0; i < 10; i++, k++)
            rollerMedals[k] = scores[i];
        for(int i = 10, k = 0; i < 13; i++, k++)
            divingMedals[k] = scores[i];
    }

    void rec(int i, string &s) {
        if (i == 3) {
            permutations.push_back(s);
            return;
        }
        for (char c : string("RLDU")) {
            s += c;
            rec(i + 1, s);
            s.pop_back();
        }
    }

    void generatePermutations() {
        string s;
        s.reserve(3);
        rec(0, s);
        random_device rd;
        mt19937 g(rd());
        shuffle(permutations.begin(), permutations.end(), g);
    }

};
// file-Name: ./header.hpp
// ########################
// ########################

// file-Name: ./hurdleGame.hpp
// ########################
// ########################
gamesInfo game;

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
   
    int getMyPos() {
        return pos[game.player_idx];
    }
    int getMinMoves(int pos, gamesInfo &game) {
        if (pos >= 29) return 0;
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

    int getMaxMoves(int idx) {
        if (gpu == "GAME_OVER") return 0;
        int ret = stun[idx];
        for(int i = pos[idx] + 1; i <= 29; i++) {
            ret++;
            if (gpu[i] == '#') ret += 2;
        }
        return ret;
    }
    
    void    play(gamesInfo& game) {
        if (gpu == "GAME_OVER") return ;
        if (guarentedWin(game)) return ;
        // cerr << "-----Hurdle-----------\n";
        // cerr << gpu << endl;
        // cerr << "   pos: " << player_pos << endl;

        if (stun[game.player_idx] > 0) return;
        // cerr << gpu[player_pos + 2] << " min moves :" << getMinMoves(player_pos, game) << ' ' << getMinMoves(player_pos + 2, game) << endl;

    
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
        
        int myMinNeed =  getMinMoves(pos[game.player_idx], game) + stun[game.player_idx];
        if (100 - game.turn < myMinNeed) return 1;
        int oppMinNeed = stun[(game.player_idx + 1) % 3] + getMinMoves(pos[(game.player_idx + 1) % 3], game);
        oppMinNeed = min(oppMinNeed, stun[(game.player_idx + 2) % 3] + getMinMoves(pos[(game.player_idx + 2) % 3], game));

        // cerr << "pos :" << pos[game.player_idx] << endl;
        // cerr << myMinNeed << ' ' << oppMinNeed << ' ' << getMaxMoves(game.player_idx) << endl;
        if (oppMinNeed >= getMaxMoves(game.player_idx)) return 1;
        int oppMaxNeed = getMaxMoves((game.player_idx + 1) % 3);
        oppMaxNeed = max(oppMaxNeed, getMaxMoves((game.player_idx + 2) % 3));
        if (oppMaxNeed < myMinNeed) return 1;
        // cerr << getMaxMoves(game.player_idx) << ' ' << oppMaxNeed << endl;

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
        return (pos[game.player_idx] >= 29 || gpu == "GAME_OVER");
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
        int myPos = pos[game.player_idx] - stun[game.player_idx];
        int opp1Pos = pos[(game.player_idx + 1) % 3] - stun[(game.player_idx + 1) % 3];
        int opp2Pos = pos[(game.player_idx + 2) % 3] - stun[(game.player_idx + 2) % 3];
        if (myPos >= max(opp1Pos, opp2Pos)) return 1;
        else if (myPos >= min(opp1Pos, opp2Pos)) return 2;
        else return 3;
    }
    double  getMyMedals() {
        for(int i = 0; i < 3; i++) {
            if (pos[i] >= 29) return medals[game.player_idx];
        }
        distributeMedals();
        return medals[game.player_idx];
    }
    double getOpp1Medals() {
        return medals[(game.player_idx + 1) % 3];
    }
    double getOpp2Medals() {
        return medals[(game.player_idx + 2) % 3];
    }
};

// file-Name: ./divingGame.hpp
// ########################
// ########################

struct divingGame {
    int points[3], combo[3];
    double medals[3];
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

        int myPoints = points[player_idx];
        int myCombo = combo[player_idx];
        bool ok = max(opp1, opp2) < points[player_idx];
        for(int i = 1; i <= gpu.size(); i++) {
            myCombo++;
            myPoints += myCombo;
        }
        ok |= (myPoints < min(points[(player_idx + 1) % 3], points[(player_idx + 2) % 3]));
        return (ok);
    }

    void    play(gamesInfo &game) {
        if (gpu == "GAME_OVER" || guarentedWin()) return ;
        // cerr << "------Diving----------\n";
        // cerr << gpu << ' ' << gpu.size() << endl;
        // cerr << "Score : " << score << endl;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        game.movesCnt[mp[gpu[0]]] += 4 + combo[player_idx] / 5;
    }

    void    nextTurn() {
        gpu.erase(gpu.begin());
    }
    void    simulate(int idx, char move) {
        if (gpu == "GAME_OVER") return ;
        // cerr << "diving: " << idx << ' ' << gpu << endl;
        if (move == gpu[0])
            combo[idx]++;
        else combo[idx] = 0;
        points[idx] += combo[idx];
    }

    bool    isTerminal() {
        return gpu.empty() || gpu == "GAME_OVER";
    }

    double getScore() {
        double score = double(points[game.player_idx]);
        return score;
    }

    int getMaximumReward() {
        if (gpu == "GAME_OVER") return 0;
        int comb = combo[game.player_idx], pts = 0;
        for(int i = 0; i < gpu.size(); i++) {
            comb++;
            pts += comb;
        }
        return pts;
    }

    int getCurPlace() {
        int myPoints = points[game.player_idx] + combo[game.player_idx];
        int opp1Points = points[(game.player_idx + 1) % 3] + combo[(game.player_idx + 1) % 3];
        int opp2Points = points[(game.player_idx + 2) % 3] + combo[(game.player_idx + 2) % 3];
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
        return medals[game.player_idx];
    }
    double getOpp1Medals() {
        return medals[(game.player_idx + 1) % 3];
    }
    double getOpp2Medals() {
        return medals[(game.player_idx + 2) % 3];
    }
};

// file-Name: ./windGame.hpp
// ########################
// ########################
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

    bool guarentedWin() {
        float winddp[15][41][41];
        // memset(winddp, -1, sizeof(winddp));
        for(int i = 0; i < 15; i++) {
            for(int j = 0; j < 41; j++) {
                for(int k = 0; k < 41; k++)  winddp[i][j][k] = -1;
            }
        }
        float myMaxDist = maximumDistance(0, posx[game.player_idx], posy[game.player_idx], winddp);
        float oppMaxDist = maximumDistance(0, posx[(game.player_idx + 1) % 3], posy[(game.player_idx + 1) % 3], winddp);
        oppMaxDist = max(oppMaxDist,  maximumDistance(0, posx[(game.player_idx + 2) % 3], posy[(game.player_idx + 2) % 3], winddp));
        // memset(winddp, -1, sizeof(winddp));

        for(int i = 0; i < 15; i++) {
            for(int j = 0; j < 41; j++) {
                for(int k = 0; k < 41; k++)  winddp[i][j][k] = -1;
            }
        }

        float oppMinDist = minimumDistance(0, posx[(game.player_idx + 1) % 3], posy[(game.player_idx + 1) % 3], winddp);
        oppMinDist = min(oppMinDist,  minimumDistance(0, posx[(game.player_idx + 2) % 3], posy[(game.player_idx + 2) % 3], winddp));
        int myMinDist =  minimumDistance(0, posx[game.player_idx], posy[game.player_idx], winddp);
        bool ok = (myMaxDist <= oppMinDist);
        ok |= (myMinDist > oppMaxDist);
    
        // cerr << myMaxDist << ' ' << oppMaxDist << endl;
        // cerr << myMinDist << ' ' << oppMinDist << endl;
        return ok;
    }
    double getDistance(int x, int y) {
        return sqrt(x * x + y * y);
    }
    float minimumDistance(int i, int x, int y, float winddp[15][41][41]) {
        if (i == gpu.size()) {
            return getDistance(x, y);
        }

        if (winddp[i][x + 20][y + 20] != -1)
            return winddp[i][x + 20][y + 20];
        float ans = 2e9;
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

    float  maximumDistance(int i, int x, int y, float winddp[15][41][41]) {
        if (i == gpu.size()) {
            return getDistance(x, y);
        }

        if (winddp[i][x + 20][y + 20] != -1)
            return winddp[i][x + 20][y + 20];
        float ans = -2e9;
        int k = 0;
        for(char c : "RUDL") {
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

    // void    play(gamesInfo &game) {
    //     if (gpu == "GAME_OVER") return ;
    //     // cerr << "-----Wind---------------\n";
    //     // cerr << gpu << endl;

    //     int i = 0 ;
    //     map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
    //     int winddp[15][41][41];
    //     memset(winddp, -1, sizeof(winddp));
    //     for (char c : "RUDL") {
    //         int nx = player_x + (gpu[0] - '0') * dx[i];
    //         int ny = player_y + (gpu[0] - '0') * dy[i];
    //         nx = min(nx, 20);
    //         nx = max(nx, -20);
    //         ny = min(ny, 20);
    //         ny = max(ny, -20);
    //         if (minimumDistance(0, player_x, player_y, winddp) == minimumDistance(1, nx, ny, winddp))
    //         game.movesCnt[mp[c]] += 5;
    //         i++;
    //     }
    // }
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

    bool    isTerminal() {
        return gpu.empty() || gpu == "GAME_OVER";
    }

    double  getScore() {
        if (gpu == "GAME_OVER") return 0;
        double dist = getDistance(posx[game.player_idx], posy[game.player_idx]);
        double score = 1 - (dist / getDistance(20, 20));
        return score;
    }

    int getCurPlace() {
        if (gpu.size() > 7) return 2;
        else if (gpu.size() > 3)return 1;
        else return 0;
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
        return medals[game.player_idx];
    }
    double getOpp1Medals() {
        return medals[(game.player_idx + 1) % 3];
    }
    double getOpp2Medals() {
        return medals[(game.player_idx + 2) % 3];
    }
};


class miniGame {
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
    bool hurdleEnd, windEnd, divingEnd;
    double  minimumHurdleMoves;
    double  maximumHurdleMoves;
    double  maximumDivingReward;
    int turnsCnt;
public:
    miniGame(hurdleGame *hurdle, windGame *wind, divingGame *diving, int turnsCnt) {
        this->hurdle = new hurdleGame(*hurdle);
        this->wind = new windGame(*wind);
        this->diving = new divingGame(*diving);
        this->hurdleEnd = 0;
        this->windEnd = 0;
        this->divingEnd = 0;
        this->minimumHurdleMoves = hurdle->getMinMoves(hurdle->pos[game.player_idx], game) + hurdle->stun[game.player_idx];
        this->maximumHurdleMoves = min(25, hurdle->getMaxMoves(game.player_idx));
        this->maximumDivingReward = diving->getMaximumReward();
        this->turnsCnt = turnsCnt;
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
        maximumDivingReward = other.maximumDivingReward;
        turnsCnt = other.turnsCnt;

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
                hurdle->simulate(game.player_idx, move);
            }
        }
        if (!windEnd) {
            if (wind->gpu == "GAME_OVER") windEnd = 1;
            else {
                wind->simulate(game.player_idx, move);
                wind->nextTurn();
            }
        }

        if (!divingEnd) {
            if (diving->gpu == "GAME_OVER") divingEnd = 1;
            else {
                diving->simulate(game.player_idx, move);
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
            totScore += curScore * game.gamesOrder['W'];
        }
        if (diving->gpu != "GAME_OVER") {
            curScore = diving->getScore() / maximumDivingReward;
            totScore += curScore * game.gamesOrder['D'];
            // cerr << score << ":: " << diving->getScore() << ' ' << maximumDivingReward << endl;
        }
        if (hurdle->gpu != "GAME_OVER") {
            if (minimumHurdleMoves == turnsCnt) curScore = 1;
            else curScore = (double)(maximumHurdleMoves - turnsCnt) / (maximumHurdleMoves - minimumHurdleMoves);
            // cerr << turnsCnt << ' ' << minimumHurdleMoves << ' ' << maximumHurdleMoves << endl;
            // cerr << "score: " <<  score << endl;
            totScore += curScore * game.gamesOrder['H'];
        }
        // cerr << score << endl;
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
        game.hurdle = new hurdleGame(game);
    else if (gameNumber == 1)
        game.wind = new windGame(game);
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else
        game.diving = new divingGame(game);
}

struct Node {
    miniGame state;
    Node* parent;
    std::vector<Node*> childs;
    int visitCount;
    double winScore;
    char move;
    string possibleActions;

    Node(const miniGame& state, Node* parent = nullptr, char move = ' ')
        : parent(parent), state(state), move(move), visitCount(0), winScore(0){
            possibleActions = "ULRD";
        }

    bool isFullyExpanded() const {
        return possibleActions.empty() && childs.size() > 0;
    }

    Node *select() {
        Node* bestNode = nullptr;
        double bestValue = -std::numeric_limits<double>::infinity();
        for (auto &child : childs) {
            double uctValue = child->winScore / child->visitCount + 1.41 * sqrt(log(visitCount) / child->visitCount);
            if (bestValue < uctValue) {
                bestValue = uctValue;
                bestNode = child;
            }
        }
        return bestNode;
    }

    Node *expand() {
        int index = rand() % possibleActions.size();
        char newMove = possibleActions[index];
        possibleActions.erase(possibleActions.begin() + index);
        miniGame newState = state; 
        newState.playMove(newMove);
        Node* newNode = new Node(newState, this, newMove);
        childs.push_back(newNode);
        return newNode;
    }
    double simulateMove() {
        miniGame tmp = state;
        int index = 0;
        string moves = "ULDR";
        while (!tmp.isTerminal()) {
            int index = rand() % moves.size();
            tmp.playMove(moves[index]);
        }
        double score = tmp.getMyScore();
        return score;
    }

    void    backpropagate(double result) {
        winScore += result;
        visitCount++;
        if (parent) parent->backpropagate(result);
    }
};

class MCTS {
public:
    MCTS() {}

    string    findNextMove(miniGame& gm, int iterations) {
        auto start = std::chrono::high_resolution_clock::now();
        Node* root = new Node(gm);
        for (int i = 0; i < iterations; i++) {
            // cerr << "searching, iteration num :" << i << endl;
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

            if ((elapsed > 47 && game.turn) || (elapsed > 995 && !game.turn)) {
                std::cerr << "Breaking after " << elapsed << " milliseconds." << std::endl;
                break;
            }
            Node* it = root;
            while(it->isFullyExpanded()) {
                it = it->select();
            }
            if (!it->state.isTerminal()) {
                // cerr << "hh\n";
                it = it->expand();
            }
            // cerr << "simulating " << it->state.isTerminal() << endl;
            double result = it->simulateMove();
            // cerr << "Probagating\n";
            it->backpropagate(result);
        }
        map<char, double> moves;
        double sum = 0;
        double bestVal = -std::numeric_limits<double>::infinity();
        char bestMove = 'R';
        for (auto &child : root->childs) {
            moves[child->move] += child->visitCount;
            sum += child->visitCount;
        }
        for(auto &[l, r] : moves) {
            if (r / sum > bestVal) {
                bestVal = r / sum;
                bestMove = l;
            }
        }
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D', "DOWN"}, {'U', "UP"}};
        return mp[bestMove];
    }
};

void    checkWins(hurdleGame *hurdle, windGame *wind, divingGame *diving){
    if (wind->gpu != "GAME_OVER" && (100 - game.turn < wind->gpu.size() || wind->guarentedWin())) wind->gpu = "GAME_OVER";
    if (diving->gpu != "GAME_OVER" && (100 - game.turn < diving->gpu.size() || diving->guarentedWin())) diving->gpu = "GAME_OVER";
    if (hurdle->gpu != "GAME_OVER" && hurdle->guarentedWin(game)) hurdle->gpu = "GAME_OVER";

}

void    prioritize(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        int i = 0;
        vector<vector<int>> order;
        if (wind->gpu != "GAME_OVER" && wind->gpu.size() <= 8) {
            int pos = wind->getCurPlace();
            int score = game.windMedals[0] * 3 + game.windMedals[1];
            // if (game.turn <= 60)
            order.push_back({pos, score, 'W'});
            // order.push_back({score, pos, 'W'});
        }
        if(hurdle->gpu != "GAME_OVER") {
            int pos = hurdle->getCurPlace();
            int score = game.hurdleMedals[0] * 3 + game.hurdleMedals[1];
            // if (game.turn <= 60)
                order.push_back({pos, score, 'H'});
            // order.push_back({score, pos, 'H'});
        }
        if (diving->gpu != "GAME_OVER") { 
            int pos = diving->getCurPlace();
            int score = game.divingMedals[0] * 3 + game.divingMedals[1];
            // if (game.turn <= 60)
                order.push_back({pos, score, 'D'});
            // order.push_back({score, pos, 'D'});
        }

        sort(order.begin(), order.end());
        vector<vector<int>> comp = order;
        for(vector<int> &v : comp) v.pop_back();
        if (order.size() == 1) game.gamesOrder[order[0][2]] = 1;
        else if (order.size() == 2) {
            if (comp[0] == comp[1]){
                game.gamesOrder[order[0][2]] = 0.5;
                game.gamesOrder[order[1][2]] = 0.5;
            }
            else {
                game.gamesOrder[order[0][2]] = 0.65;
                game.gamesOrder[order[1][2]] = 0.35;
            }
                
        }
        else if (order.size() == 3){
            if (comp[0] == comp[1] && comp[1] == comp[2]) {
                game.gamesOrder[order[0][2]] = 0.333333;
                game.gamesOrder[order[1][2]] = 0.333333;
                game.gamesOrder[order[2][2]] = 0.333333;
            }
            else if (comp[0] == comp[1]) {
                game.gamesOrder[order[0][2]] = 0.425;
                game.gamesOrder[order[1][2]] = 0.425;
                game.gamesOrder[order[2][2]] = 0.15;
            }
            else {
                game.gamesOrder[order[0][2]] = 0.6;
                game.gamesOrder[order[1][2]] = 0.25;
                game.gamesOrder[order[2][2]] = 0.15;
            }
        }
        // cerr << "Order\n";
        // for(auto &i : order) {
        //     for(int &j : i) {
        //         if (j == i.back()) cerr << (char)j << ' ';
        //         else cerr << j << ' ';
        //     }
        //     cerr << endl;
        // }
        // for(auto&[l, r] : game.gamesOrder) cerr << l << ' ' <<r << endl;

    }

int main()
{
    srand(time(0));
    cin >> game.player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();
    // game.generatePermutations();
    // game loop
    while (1) {
        game.movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string scoreInfo;
            getline(cin, scoreInfo);
            if (i == game.player_idx)
                game.setMedals(scoreInfo);
        }
        for (int i = 0; i < nb_games; i++) {
            setupGame(i);
        }
        // cerr << "\n---------------\n";
        MCTS mct;
        // game.hurdle->gpu = "GAME_OVER";
        // game.diving->gpu = "GAME_OVER";
        for(auto &[l, r] : game.gamesOrder) r = 0;
        checkWins(game.hurdle, game.wind, game.diving);
        prioritize(game.hurdle, game.wind, game.diving);
        int turnsCnt = 14;
        if (game.wind->gpu != "GAME_OVER") turnsCnt = min(turnsCnt, (int)game.wind->gpu.size());
        if (game.diving->gpu != "GAME_OVER") turnsCnt = min(turnsCnt, (int)game.diving->gpu.size());
        miniGame mg(game.hurdle, game.wind, game.diving, turnsCnt);
        // cerr << game.diving->gpu << ' ' << endl;
        // cerr << game.hurdle->gpu << ' ' << endl;
        // cerr << game.diving->gpu << ' ' << endl;
        if (game.turn == 0)
            cout << mct.findNextMove(mg, 10000) << endl;
        else cout << mct.findNextMove(mg, 6200) << endl;

        // double maxMoves = -2e9;
        // string ans;
        // for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
        //     if (game.movesCnt[s] > maxMoves) maxMoves = game.movesCnt[s], ans = s;
        //     cerr << s << ' ' << game.movesCnt[s] << endl;
        // }
        // cout << ans << endl;
        game.turn++;
        // return 0;
    }
    return 0;
}
