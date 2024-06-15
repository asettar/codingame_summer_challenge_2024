#pragma once
/*start*/
#include <map>
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
    map<std::string, double> movesCnt;
    int hurdleMedals[3];
    int windMedals[3];
    int rollerMedals[3];
    int divingMedals[3];
    int totalScore;
    vector<int> hurdledp;
    vector<vector<string>> permutations;
    hurdleGame *hurdle;
    windGame *wind;
    divingGame *diving;

    gamesInfo() {
        permutations.resize(10);
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

    void    rec(int i, string &s) {
        if (i == 10) return ;
        if (i) permutations[i].push_back(s);
        for(char c : "RLDU") {
            s += c;
            rec(i + 1, s);
            s.pop_back();
        }
    }

    void    generatePermutations() {
        string s;
        rec(0, s);
    }
};
/*end*/