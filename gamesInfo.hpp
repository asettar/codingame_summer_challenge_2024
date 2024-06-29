#include "header.hpp"
/*start*/
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
/*end*/