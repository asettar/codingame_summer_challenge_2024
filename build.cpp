#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <fstream>

using namespace std;

vector<string> files =
{
    /* header-files*/
    "./header.hpp",
    "./gamesInfo.hpp",
    "./hurdleGame.hpp",
    "./divingGame.hpp",
    "./windGame.hpp",
    "./miniGame.hpp",
    "./mct.cpp",

    /*main-file*/
    "./main.cpp",
};

int main()
{
    ofstream out("./prod/code.cpp");

    for (auto &file : files)
    {
        ifstream    fin(file);
        string        content;
        while (content != "/*start*/") {
            getline(fin, content);
        }
        out << "// file-Name: " << file << endl;
        out << "// ########################" << endl;
        out << "// ########################" << endl;
        while (getline(fin, content) && content != "/*end*/")
        {
            out << content << endl;
        }
    }
    out.close();
    return (0);
}
