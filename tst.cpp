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

    int getMaxMoves() {
        if (gpu == "GAME_OVER") return 0;
        int ret = stun[game.player_idx];
        for(int i = pos[game.player_idx] + 1; i <= 29; i++) {
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
        int maxNeedForWin = stun[game.player_idx] + gpu.size() - player_pos;
        for(int i = player_pos + 1; i < gpu.size(); i++) {
            if (gpu[i] == '#') maxNeedForWin += 3;
        }
        
        int opps1Need = stun[(game.player_idx + 1) % 3] + getMinMoves(pos[(game.player_idx + 1) % 3], game);
        int opp2Need = stun[(game.player_idx + 2) % 3] + getMinMoves(pos[(game.player_idx + 2) % 3], game);
        return (maxNeedForWin <= opps1Need && maxNeedForWin <= opp2Need);
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
        return (max(opp1, opp2) < points[player_idx]);
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
        double score = double(points[game.player_idx]) / 80.0;
        return score;
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
        // cerr << "-----Wind---------------\n";
        // cerr << gpu << endl;

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
    int turnsCnt;
public:
    miniGame(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        this->hurdle = new hurdleGame(*hurdle);
        this->wind = new windGame(*wind);
        this->diving = new divingGame(*diving);
        this->hurdleEnd = 0;
        this->windEnd = 0;
        this->divingEnd = 0;
        this->minimumHurdleMoves = hurdle->getMinMoves(hurdle->pos[game.player_idx], game) + hurdle->stun[game.player_idx];
        this->maximumHurdleMoves = min(25, hurdle->getMaxMoves() + hurdle->stun[game.player_idx]);
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
        double score = 0, cnt = 0;
        if (wind->gpu != "GAME_OVER")
            score += wind->getScore(), cnt++;
        if (diving->gpu != "GAME_OVER")
            score += diving->getScore(), cnt++;
        if (hurdle->gpu != "GAME_OVER") {
            if (minimumHurdleMoves == turnsCnt) score += 1;
            else score += (double)(maximumHurdleMoves - turnsCnt) / (maximumHurdleMoves - minimumHurdleMoves);
            // cerr << turnsCnt << ' ' << minimumHurdleMoves << ' ' << maximumHurdleMoves << endl;
            // cerr << "score: " <<  score << endl;
            cnt++;
        }
        score /= cnt;
        // cerr << score << endl;
        return (score);
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

// file-Name: ./main.cpp
// ########################
// ########################
// gamesInfo game;

// void    checkcheckHurdleDiving() {
//     // firstTime
//     if (!game.diveCnt && !game.windCnt && !game.diving->player_idx) {
//         // start with the one that take minimum moves possibles
//         int windMoves = game.wind->gpu.size();

//     }
// }


// void    checkBestMoves() {
//     // if (!game.hurdleCnt && !game.diveCnt && !game.hurdle->player_pos) {
//     //     // first turn start either hurdle or diving base on minimum moves
//     //     int divingMoves = game.diving->gpu.size();
//     //     int hurdleMoves = game.hurdle->getMinMoves(0); 

//     //     if (divingMoves > hurdleMoves) {
//     //         game.playingHurdle = 1;
//     //         game.hurdle->play(game);
//     //     }
//     //     else game.diving->play(game);
//     // }
//     if (game.hurdle->gpu != "GAME_OVER" && game.hurdleMedals[0] == 0 && !game.hurdle->guarentedWin(game))
//         game.hurdle->play(game);
//     else if (game.diving->gpu != "GAME_OVER" && game.divingMedals[0] == 0 && !game.diving->guarentedWin())
//         game.diving->play(game);
//     else if (game.hurdleMedals[0] == 1 && (game.divingMedals[0] == 1 || (game.divingMedals[0] == 0 && game.diving->guarentedWin())))
//         game.hurdle->play(game);
//         // add 1 medal to hurdle 
//     else if ((game.hurdleMedals[0] == 2 || (game.hurdle->guarentedWin(game) && game.hurdleMedals[0] == 1)) 
//         && game.divingMedals[0] == 1 && !game.diving->guarentedWin()) {
//         // add 1 medal to diiving
//         game.diving->play(game);
//     }
//     else { 
//         // now you guarented 2 gold in hurdle and 2 gold in diving 
//         // strategie for know if windgpu is less than 10 play for wind
//         // else play either for the hurdle or diving

       
//         if (game.wind->gpu != "GAME_OVER" && game.wind->gpu.size() <= 9)
//             game.wind->play(game);
//         game.hurdle->score(game);
//         game.diving->play(game);
//     }
// }

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

            if ((elapsed > 50 && game.turn) || (elapsed > 995 && !game.turn)) {
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
        // game.wind->gpu = "GAME_OVER";
        miniGame mg(game.hurdle, game.wind, game.diving);
        if (game.turn == 0)
            cout << mct.findNextMove(mg, 10000) << endl;
        else cout << mct.findNextMove(mg, 6500) << endl;

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

// 0.33 + 0.25 + 0.42