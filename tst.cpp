// file-Name: ./gamesInfo.hpp
// ########################
// ########################
#include <map>
#include <string> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
using namespace std;

struct hurdleGame;
struct windGame;
struct divingGame;

struct gamesInfo {
    std::string gpu;
    int divingPrevGpu = 0;
    int hurdleCnt = 0, windCnt = 0, diveCnt = 0;
    int player_idx;
    std::map<std::string, double> movesCnt;
    
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;
};
// file-Name: ./header.hpp
// ########################
// ########################

// file-Name: ./divingGame.hpp
// ########################
// ########################

struct divingGame {
    int points[3], combo[3];
    int unused, player_idx;
    string gpu;

    divingGame() {}
    divingGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) cin >> points[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> combo[i]; cin.ignore();
        cin >> unused; cin.ignore();
        if (gpu == "GAME_OVER") game.diveCnt++;
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
        cerr << opp1 << ' ' << opp2 << endl;
        return (max(opp1, opp2) < points[player_idx]);
    }

    void    play(gamesInfo &game) {
        if (gpu == "GAME_OVER") return ;
        if (game.diveCnt >= 3) return ;
        if (gpu.size() == 1) game.diveCnt++;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        if (guarentedWin()) return ;
        double score = combo[player_idx] + 4;
        cerr << "------Diving----------\n";
        cerr << gpu << ' ' << gpu.size() << endl;
        cerr << "Score : " << score << endl;
        game.movesCnt[mp[gpu[0]]] += score;
    }
};
// file-Name: ./hurdleGame.hpp
// ########################
// ########################

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

// file-Name: ./windGame.hpp
// ########################
// ########################
struct windGame {
    string gpu;
    int unused;
    int posx[3], posy[3];
    int player_x, player_y;
    int player_idx;

    windGame() {}
    windGame(gamesInfo &game) {
        gpu = game.gpu;
        for(int i = 0; i < 3; i++) {
            cin >> posx[i]; cin.ignore();
            cin >> posy[i]; cin.ignore();
        }
        cin >> unused; cin.ignore();
        player_x = posx[game.player_idx];
        player_y = posy[game.player_idx];
        if (gpu == "GAME_OVER") game.windCnt++;
    }

    int getMovesLeft() {
        int sumLeft = 0;
        for(int i = 1; i < gpu.size(); i++)
            sumLeft += gpu[i] - '0';
        return sumLeft;
    }

    void    play(std::map<string, double> &movesCnt) {
        if (gpu == "GAME_OVER") return ;
        int sumLeft = getMovesLeft();
        if (sumLeft >= 15 || gpu.size() >= 6) return;
        cerr << "-----Wind---------------\n";
        cerr << gpu << endl;

        int power = gpu[0] - '0';
        int dx[4] = {1, 0, 0, -1};
        int dy[4] = {0, -1, 1, 0};
        int i = 0;
        cerr << "x:" << player_x << " y: " << player_y << " turnLeft:" << gpu.size()<< endl;
        map<int, vector<string>> best;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            int nx = posx[player_idx] + power * dx[i];
            int ny = posy[player_idx] + power * dy[i];
            nx = min(nx, 20);
            nx = max(nx, -20);
            ny = min(ny, 20);
            ny = max(ny, -20);
            i++;
            double  distance = sqrt(nx * nx + ny * ny); // we want it to be as little as possible
            double score = 400.0 / (distance * gpu.size() + 1);
            cerr << "   score: "<< s << ' ' << score << ' ' << distance << endl;
            movesCnt[s] += score;
        }
        cerr << endl;

    }
};
// file-Name: ./main.cpp
// ########################
// ########################
gamesInfo game;


void    playRollerSpeedGame(string &gpu) {
    int num;
    for(int i=0; i < 7; i++) cin>>num;cin.ignore();
}



void    playGame(int gameNumber) {
    cin >> game.gpu; cin.ignore();
    
    if (gameNumber == 0) {
        game.hurdle = new hurdleGame(game);
        game.hurdle->play(game);
    }
    else if (gameNumber == 1) {
        game.wind = new windGame(game);
        game.wind->play(game.movesCnt);
    }
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else {
        game.diving = new divingGame(game);
        game.diving->play(game);
    }
}

int main()
{
    cin >> game.player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();

    // game loop
    while (1) {
        game.movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string score_info;
            getline(cin, score_info);
        }
        for (int i = 0; i < nb_games; i++) {
            playGame(i);
        }   
        cerr << "\n---------------\n";
        double maxMoves = -2e9;
        string ans;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            if (game.movesCnt[s] > maxMoves) maxMoves = game.movesCnt[s], ans = s;
            cerr << s << ' ' << game.movesCnt[s] << endl;
        }
        cout << ans << endl;
    }
}

