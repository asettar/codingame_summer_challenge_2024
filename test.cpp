#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <climits>
#include <iomanip>
#include <random>
using namespace std;
#define int long long
#define double long double
#define vi vector <int>
#define vb vector <bool>
#define vs vector <string>
#define vvi vector <vi>
#define vvb vector <vb>
#define vvs vector <vs>
#define all(x) x.begin(), x.end()
#define	rall(x) x.rbegin(), x.rend()
#define endl '\n'
const int mod = 1e9+7;
const int maxn = 1e5 + 10;

vector<string> perm;



void rec(int i, string &s) {
    if (i == 4) {
        cerr << i << ' ' << s << endl;
        perm.push_back(s);
        return;
    }
    for (char c : string("RLDU")) {
        s += c;    // Set the character at position i
        rec(i + 1, s);
        s.pop_back();
    }
}

void generatePermutations() {
    string s;
    s.reserve(4);
    rec(0, s);
}


int32_t	main()
{
    generatePermutations();
    random_device rd;
    mt19937 g(rd());
    shuffle(perm.begin(), perm.end(), g);
    for(string s : perm) cout <<s  << endl;
}
