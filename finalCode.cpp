// file-Name: ./header.hpp
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

map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D', "DOWN"}, {'U', "UP"}};

struct hurdleGame;
struct windGame;
struct divingGame;

// file-Name: ./gamesInfo.hpp
// ########################
// ########################
struct gamesInfo {
    std::string gpu;
    int playerIdx;
    int turn = 0;
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
    float maxWindDist, minWindDist;
    float minHurdleNeed, maxHurdleNeed;
    float minDiving, maxDiving;
    
    gamesInfo() {}

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

gamesInfo game;
// file-Name: ./hurdleGame.hpp
// ########################
// ########################
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
// file-Name: ./divingGame.hpp
// ########################
// ########################
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
// file-Name: ./windGame.hpp
// ########################
// ########################
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
// file-Name: ./miniGame.hpp
// ########################
// ########################
class miniGame {
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
    bool hurdleEnd, windEnd, divingEnd;
    double  minimumHurdleMoves;
    double  maximumHurdleMoves;
    double  maxDivingPoints;
    int turnsCnt;
public:
    miniGame(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        this->hurdle = new hurdleGame(*hurdle);
        this->wind = new windGame(*wind);
        this->diving = new divingGame(*diving);
        this->hurdleEnd = 0;
        this->windEnd = 0;
        this->divingEnd = 0;
        this->minimumHurdleMoves = hurdle->getMinMoves(hurdle->pos[game.playerIdx]);
        this->maximumHurdleMoves = min(25, hurdle->getMaxMoves(game.playerIdx));
        this->maxDivingPoints = diving->getMaximumReward(game.playerIdx) + diving->points[game.playerIdx];
        this->turnsCnt = 0;
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
        maxDivingPoints = other.maxDivingPoints;
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
                hurdle->simulate(game.playerIdx, move);
            }
        }
        if (!windEnd) {
            if (wind->gpu == "GAME_OVER") windEnd = 1;
            else {
                wind->simulate(game.playerIdx, move);
                wind->nextTurn();
            }
        }

        if (!divingEnd) {
            if (diving->gpu == "GAME_OVER") divingEnd = 1;
            else {
                diving->simulate(game.playerIdx, move);
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
            totScore += curScore * game.gamesOrder['E'];
        }
        if (diving->gpu != "GAME_OVER") {
            curScore = 1.0 - (double)(game.maxDiving - diving->points[game.playerIdx]) / (double)(game.maxDiving - game.minDiving);
            if (curScore > 1) curScore = 1;
            if (curScore < 0) curScore = 0;
            totScore += curScore * game.gamesOrder['D'];
        }
        if (hurdle->gpu != "GAME_OVER") {
            if (minimumHurdleMoves == turnsCnt) curScore = 1;
            else curScore = (double)(game.maxHurdleNeed - turnsCnt) / (game.maxHurdleNeed - game.minHurdleNeed);
            if (curScore > 1) curScore = 1;
            if (curScore < 0) curScore = 0;
            totScore += curScore * game.gamesOrder['H'];
        }
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
        game.hurdle = new hurdleGame();
    else if (gameNumber == 1)
        game.wind = new windGame();
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else
        game.diving = new divingGame();
}
// file-Name: ./mct.cpp
// ########################
// ########################

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
                it = it->expand();
            }
            double result = it->simulateMove();
            it->backpropagate(result);
        }
        map<char, double> moves;
        double sum = 0;
        double bestVal = -1e9;
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
        return mp[bestMove];
    }
};
// file-Name: ./main.cpp
// ########################
// ########################
void    checkWinsLoses(hurdleGame *hurdle, windGame *wind, divingGame *diving){
    if (wind->gpu != "GAME_OVER" && (100 - game.turn < wind->gpu.size() || wind->guarentedWinLose())) wind->gpu = "GAME_OVER";
    if (diving->gpu != "GAME_OVER" && (100 - game.turn < diving->gpu.size() || diving->guarentedWinLose())) diving->gpu = "GAME_OVER";
    if (hurdle->gpu != "GAME_OVER" && (hurdle->stun[game.playerIdx] || hurdle->guarentedWinLose(game))) hurdle->gpu = "GAME_OVER";
}

void    prioritizeGames(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        vector<pair<double, char>> order;
        float sum = 0;
        if (wind->gpu != "GAME_OVER") {
            double medals = min((double)(game.windMedals[0] * 3 + game.windMedals[1]) / 12.0, 1.0);
            double toend = wind->gpu.size() / 16.0;
            order.push_back({toend * 0.3 + medals * 0.7 , 'E'});
            sum += 1.0 - order.back().first;
        }
        if(hurdle->gpu != "GAME_OVER" && !hurdle->stun[game.playerIdx]) {
            double pos = hurdle->getCurPlace() / 3.0;
            double toend = 1 - hurdle->getMyPos() / 31.0;
            double medals = min((double)(game.hurdleMedals[0] * 3 + game.hurdleMedals[1]) / 12.0, 1.0);
            order.push_back({pos * 0.3 + toend * 0.4 + medals * 0.3, 'H'});
            sum += 1.0 - order.back().first;

        }
        if (diving->gpu != "GAME_OVER") { 
            double pos = diving->getCurPlace() / 3.0;
            double medals = min((double)(game.divingMedals[0] * 3 + game.divingMedals[1]) / 12.0, 1.0);
            order.push_back({pos * 0.4  + medals * 0.6 , 'D'});
            sum += 1.0 - order.back().first;
        }

        sort(order.begin(), order.end());
        // cerr << order.size() << ' ' << sum << ' ' << game.gamesOrder.size() << endl;
        if (sum){
            for(auto &[l, r] : order)
                game.gamesOrder[r] = (1 - l) / sum;
        }
        
        // cerr << "Order\n";
        // for(auto&[l, r] : game.gamesOrder) cerr << l << ' ' << r << endl;

}


int main()
{
    srand(time(0));
    cin >> game.playerIdx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();
    // game.generatePermutations();
    // game loop
    while (1) {
        for (int i = 0; i < 3; i++) {
            string scoreInfo;
            getline(cin, scoreInfo);
            if (i == game.playerIdx)
                game.setMedals(scoreInfo);
        }
        for (int i = 0; i < nb_games; i++) {
            setupGame(i);
        }
        MCTS mct;
        game.gamesOrder.clear();
        checkWinsLoses(game.hurdle, game.wind, game.diving);
        prioritizeGames(game.hurdle, game.wind, game.diving);
        int cntOverGames = (game.hurdle->gpu == "GAME_OVER") + (game.wind->gpu == "GAME_OVER") + (game.diving->gpu == "GAME_OVER");
       
        if (cntOverGames == 2) {
            if (game.hurdle->gpu != "GAME_OVER")
                game.hurdle->play();
            else if (game.diving->gpu != "GAME_OVER")
                cout << mp[game.diving->gpu[0]] << endl;
            else game.wind->play();
        }
        else {
            miniGame mg(game.hurdle, game.wind, game.diving);
            if (game.turn == 0)
                    cout << mct.findNextMove(mg, 10000) << endl;
            else cout << mct.findNextMove(mg, 6000) << endl;
        }
        game.turn++;
    }
    return 0;
}
