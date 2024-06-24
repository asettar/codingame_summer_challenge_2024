#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// totalScore = score of all games multiplied
int main()
{
    int player_idx;
    cin >> player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();

    // game loop
    while (1) {
        for (int i = 0; i < 3; i++) {
            string score_info;
            getline(cin, score_info);
        }

        map<string, int> movesCnt;
        for (int i = 0; i < nb_games; i++) {
            string gpu; 
            int reg_0; //position of p1
            int reg_1; //position of p1
            int reg_2; //position of p1
            int reg_3; //remaining stuntime for p1
            int reg_4; //remaining stuntime for p1
            int reg_5; //remaining stuntime for p1
            int reg_6;
            cin >> gpu ;
            vector<int> pos(3), stun(3);
            for(int &j : pos) cin >> j;
            for(int &j : stun) cin >> j;
            cin >> reg_6; cin.ignore();
            reg_0 = pos[player_idx];
            if (gpu == "GAME_OVER") continue;
            if (stun[player_idx] > 0) continue;
            cerr << gpu << ' ' << reg_0 << endl;

            int mv[3] = {3, 2, 1};
            int k = 0;
            for(string s : {"RIGHT", "UP", "LEFT"}) {
                for(int j = reg_0 + 1; j <= reg_0 + mv[k]; j++) {
                    if (gpu[j] == '#') {
                        if (s == "UP" && j == reg_0 + 1) movesCnt[s] += 0;
                        else {
                            movesCnt[s] -= 3;
                            break;
                        }
                    }
                    movesCnt[s]++;
                    if (j >= gpu.size() - 1) {
                        movesCnt[s] += 10;
                        break;
                    }
                }
                k++;
            }

            // movesCnt["LEFT"]++;
            // if (pos[player_idx + 1] == '#') movesCnt["LEFT"] -= 3;
            // else if (player_idx + 1 >= gpu.size() - 1) movesCnt["LEFT"] += 10;
            // cerr << i << ' ' << gpu << ' ' << pos[player_idx]  << endl;
            // if (reg_0 + 1 < gpu.size()) {
            //     movesCnt["UP"] += 2;
            //     if (reg_0 + 2 >= gpu.size() - 1) movesCnt["UP"] += 10;
            //     if (reg_0 + 2 < gpu.size() && gpu[reg_0 + 2] == '#') movesCnt["UP"] -= 3;
            // }
            // else {
            //     if (reg_0 + 3 < gpu.size() && gpu[reg_0 + 3] != '#') movesCnt["RIGHT"] += 3;
            //     if (reg_0 + 3 >= gpu.size() - 1) movesCnt["RIGHT"] += 10;
            // }
        }   
        int maxMoves = -2e9;
        string ans;
        for(string s : {"RIGHT", "UP", "LEFT"}) {
            if (movesCnt[s] > maxMoves) maxMoves = movesCnt[s], ans = s;
            cerr << s << ' ' << movesCnt[s] << endl;
        }
        cout << ans << endl;
    }
}


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// totalScore = score of all games multiplied

map<string, int> movesCnt;
int player_idx;

void    playHurdleGame(string &gpu) {
    int reg_0; //position of p1
    int reg_1; //position of p1
    int reg_2; //position of p1
    int reg_3; //remaining stuntime for p1
    int reg_4; //remaining stuntime for p1
    int reg_5; //remaining stuntime for p1
    int reg_6;
    vector<int> pos(3), stun(3);
    for(int &j : pos) cin >> j; cin.ignore();
    for(int &j : stun) cin >> j; cin.ignore();
    cin >> reg_6; cin.ignore();
    reg_0 = pos[player_idx];
    if (stun[player_idx] > 0) return;
    cerr << gpu << ' ' << reg_0 << endl;
    cerr << "pos " << reg_0 << endl;
    int mv[4] = {3, 2, 2, 1};
    int k = 0;
    for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
        for(int j = reg_0 + 1; j <= reg_0 + mv[k]; j++) {
            if (gpu[j] == '#') {
                if (s == "UP" && j == reg_0 + 1) movesCnt[s] += 0;
                else {
                    movesCnt[s] -= 3;
                    break;
                }
            }
            movesCnt[s]++;
            if (j >= gpu.size() - 1) {
                movesCnt[s] += 10;
                break;
            }
        }
        k++;
    }
}
void    playWindGame(string &gpu) {
    vector<int> posx(3), posy(3);
    for(int i = 0; i < 3; i++) {
        cin >> posx[i]; cin.ignore();
        cin >> posy[i]; cin.ignore();
    }
    int unused; cin >> unused; cin.ignore();

    int power = gpu[0] - '0';
    int dx[4] = {1, 0, 0, -1};
    int dy[4] = {0, -1, 1, 0};     //down y up,  up y down
    int i = 0;
    cerr << gpu << endl;
    cerr << "x : "<< posx[player_idx] << "|| y : " << posy[player_idx] << endl;
    for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
        int nx = posx[player_idx] + power * dx[i] + power * dy[i];
        int ny = posy[player_idx] + power * dy[i] + power * dy[i];
        nx = min(nx, 20);
        nx = max(nx, -20);
        ny = min(ny, 20);
        ny = max(ny, -20);
        i++;
        int distance = sqrt(nx * nx + ny * ny); // we want it to be as liitle as possible
        movesCnt[s] -= distance;
    }
}

void    playRollerSpeedGame(string &gpu) {
    int num;
    for(int i=0; i<7;i++) cin>>num;cin.ignore();
}
void     playDivingGame(string &gpu) {
    int num;
    for(int i =0;i<7;i++) cin >> num;cin.ignore();
}

void    playGame() {
    string gpu; cin >> gpu; cin.ignore();
    if (gpu[0] == '.') playHurdleGame(gpu);
    else if (isdigit(gpu[0])) playWindGame(gpu);
    else if (gpu.size() == 4) playRollerSpeedGame(gpu);
    else if (gpu[0] != 'G') playDivingGame(gpu);
    else{
        int num;
        for(int i=0; i<7; i++) cin>>num;cin.ignore();
    }
}

int main()
{
    cin >> player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();

    // game loop
    while (1) {
        movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string score_info;
            getline(cin, score_info);
        }

        for (int i = 0; i < nb_games; i++) {
            playGame();
        }   
        int maxMoves = -2e9;
        string ans;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            if (movesCnt[s] > maxMoves) maxMoves = movesCnt[s], ans = s;
            cerr << s << ' ' << movesCnt[s] << endl;
        }
        cout << ans << endl;
    }
}