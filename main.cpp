#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
using namespace std;

int turns;

map<string, double> movesCnt;
int player_idx;
struct hurdleGame;

struct GamesInfo {
    int windPrevGpu = 0;
    int divingPrevGpu = 0;
    int hurdleCnt = 0, windCnt = 0, diveCnt = 0;
    hurdleGame hurdle;
    windGame wind;
    divingGame diving;
};

GamesInfo glob;

// hurdle = hurdle(game)
struct hurdleGame {
    string gpu;
    int pos[3], stun[3];
    int unused, player_pos;

    hurdleGame() {}

    hurdleGame(string &gpu) {
        gpu = gpu;
        for(int i = 0; i < 3; i++) cin >> pos[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> stun[i]; cin.ignore();
        cin >> unused; cin.ignore();
        player_pos = pos[player_idx];
        if (!player_pos) glob.hurdleCnt++;

    }
    // hurdleGame = 
    void    play() {
        if (glob.diveCnt < 3) return ;
        cerr << "-----Hurdle-----------\n";
        cerr << gpu << endl;
        cerr << "   pos: " << player_pos << endl;
        
        if (stun[player_idx] > 0) return;
        // player_pos -= 1;
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
            movesCnt[s] += score;
            cerr << "   Score :" << s << ' ' << score << endl;
            k++;
        }
        cerr << '\n';
    }

};

struct windGame {
    string gpu;
    int unused;
    int posx[3], posy[3];
    int player_x, player_y;
    
    windGame(string &gpu) {
        gpu = gpu;
        for(int i = 0; i < 3; i++) {
            cin >> posx[i]; cin.ignore();
            cin >> posy[i]; cin.ignore();
        }
        cin >> unused; cin.ignore();
        player_x = posx[player_idx];
        player_y = posy[player_idx];
        if (glob.windPrevGpu <= gpu.size()) glob.windCnt++;
        glob.windPrevGpu = gpu.size();
    }

    int getMovesLeft() {
        int sumLeft = 0;
        for(int i = 1; i < gpu.size(); i++)
            sumLeft += gpu[i] - '0';
        return sumLeft;
    }

    void    play() {
        int sumLeft = getMovesLeft();
        if (sumLeft >= 15 || gpu.size() >= 6) return;
        cerr << "-----Wind---------------\n";
        cerr << gpu << endl;

        int power = gpu[0] - '0';
        int dx[4] = {1, 0, 0, -1};
        int dy[4] = {0, -1, 1, 0};
        int i = 0;
        cerr << "x:" << posx[player_idx] << " y: " << posy[player_idx] << " turnLeft:" << gpu.size()<< endl;
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


struct divingGame {
    int points[3], combo[3];
    int unused;
    string gpu;
    divingGame(string &gpu) {
        gpu = gpu;
        for(int i = 0; i < 3; i++) cin >> points[i]; cin.ignore();
        for(int i = 0; i < 3; i++) cin >> combo[i]; cin.ignore();
        cin >> unused; cin.ignore();
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

    void    play() {
        if (glob.diveCnt > 3) return ;
        if (gpu.size() == 1) glob.diveCnt++;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D',  "DOWN"}, {'U', "UP"}};
        // if (combo[(player_idx + 1) % 3] + )
        double score = combo[player_idx] + 4;
        if (guarentedWin()) return ;
        cerr << "------Diving----------\n";
        cerr << gpu << ' ' << gpu.size() << ' ' << glob.diveCnt << endl;
        cerr << "Score : " << score << endl;
        movesCnt[mp[gpu[0]]] += score;
    }
};

void    playRollerSpeedGame(string &gpu) {
    int num;
    for(int i=0; i < 7; i++) cin>>num;cin.ignore();
}



void    playGame(int game) {
    string gpu; cin >> gpu; cin.ignore();
    
    if(gpu == "GAME_OVER") {
        int num;
        for(int i = 0; i < 7; i++)
            cin >> num; cin.ignore();
        return ;
    }
    if (game == 0) {
        glob.hurdle = hurdleGame(gpu);
        // hg.play();
    }
    else if (game == 1) {
        glob.wind = windGame(gpu);
        // wg.play();
    }
    else if (game == 3){
        glob.diving = divingGame(gpu);
        // dg.play();
    }
    else playRollerSpeedGame(gpu);
}

int main()
{
    cin >> player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();

    // game loop
    turns = 0;
    while (1) {
        movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string score_info;
            getline(cin, score_info);
        }
        turns++;
        for (int i = 0; i < nb_games; i++) {
            playGame(i);
        }   
        cerr << "\n---------------\n";
        double maxMoves = -2e9;
        string ans;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            if (movesCnt[s] > maxMoves) maxMoves = movesCnt[s], ans = s;
            cerr << s << ' ' << movesCnt[s] << endl;
        }
        cout << ans << endl;
    }
}



//  double rec(int hpos, int wturn, int px, int py, hurdleGame &hg, windGame &wg) {
//     double  sum = 0;
//     // if (hpos == hg.gpu.size()) sum += 
//     if (wturn == wg.gpu.size()) sum += sqrt(px * px + py * py);
//     if (wturn >= wg.gpu.size()) return sum;

//     int mv[4] = {3, 2, 2, 1};
//     int k = 0;
//     for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
//         int score = 0;
//         for(int j = hpos + 1; j <= hpos + mv[k]; j++) {
//             if (hg.gpu[j] == '#') {
//                 if (s == "UP" && j == hpos + 1) score += 0;
//                 else {
//                     score -= 3;
//                     break;
//                 }
//             }
//             score++;
//             if (j >= hg.gpu.size() - 1) {
//                 score += 15;
//             }
//         }
//         if (score > 0) score += hpos;
//         movesCnt[s] += score;
//         cerr << "   Score :" << s << ' ' << score << endl;
//         k++;
//     }
//     return sum;
// }

// void    getBestMoves(hurdleGame &hg, windGame &wg) {
//     int movesNeeded = rec(hg.player_pos, 0, wg.player_x, hg, wg);
// }

