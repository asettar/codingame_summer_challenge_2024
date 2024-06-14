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
/*end*/