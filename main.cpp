#include "header.hpp"
/*start*/
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
    string move;
    int moveIndex;

    Node(const miniGame& state, Node* parent = nullptr, string move = "", int moveIndex = 0)
        : state(state), parent(parent), move(move), moveIndex(moveIndex), visitCount(0), winScore(0){}

    bool isFullyExpanded() const {
        return moveIndex >= 64 && childs.size() > 0;
    }

    Node *select() {
        Node* bestNode = nullptr;
        double bestValue = -std::numeric_limits<double>::infinity();
        map<char, pair<double, Node*>> groups;
        for (auto child : childs) {
            double uctValue = (child->winScore / (child->visitCount + 1)) + 1.41 * std::sqrt(std::log(visitCount + 1) / (child->visitCount + 1));
            if (!groups.count(child->move[0]))
                groups[child->move[0]] = {uctValue, child};
            else {
                if (uctValue < groups[child->move[0]].first)
                    groups[child->move[0]] = {uctValue, child};
            }
        }
        for(auto &[l, r] : groups) {
            if (bestValue < r.first) {
                bestValue = r.first;
                bestNode = r.second;
            }
        }
        return bestNode;
    }

    Node *expand() {
        string newMove = game.permutations[moveIndex];
        moveIndex++;
        miniGame newState = state; 
        newState.playMove(newMove);
        Node* newNode = new Node(newState, this, move);
        childs.push_back(newNode);
        return newNode;
    }
    double simulateMove() {
        miniGame tmp = state;
        int index = 0;
        while (!tmp.isTerminal()) {
            string &randomMove = game.permutations[index];
            tmp.playMove(randomMove);
            index++;
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

    string    findNextMove(miniGame& game, int iterations) {
        Node* root = new Node(game);
        for (int i = 0; i < iterations; i++) {
            Node* it = root;
            while(it->isFullyExpanded()) 
                it = it->select();
            if (!it->state.isTerminal())
                it = it->expand();
            double result = it->simulateMove();
            it->backpropagate(result);
        }
        Node* bestNode = root->select();
        string bestMove = bestNode->move;
        delete root;
        map<char, string> mp = {{'L', "LEFT"}, {'R', "RIGHT"}, {'D', "DOWN"}, {'U', "UP"}};
        return mp[bestMove[0]];
    }
};

int main()
{
    cin >> game.player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();
    game.generatePermutations();
    // game loop
    MCTS mct;
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
        // checkBestMoves();
        cerr << "\n---------------\n";
        double maxMoves = -2e9;
        // string ans;
        // for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
        //     if (game.movesCnt[s] > maxMoves) maxMoves = game.movesCnt[s], ans = s;
        //     cerr << s << ' ' << game.movesCnt[s] << endl;
        // }
        // cout << ans << endl;

        miniGame mg(game.hurdle, game.wind, game.diving);
        cout << mct.findNextMove(mg, 300) << endl;
    }
    return 0;
}
