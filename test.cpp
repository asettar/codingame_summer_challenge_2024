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
    void    backpropagate(double result) {
        winScore += result;
        visitCount++;
        if (parent) parent->backpropagate(result);
    }

    double simulateMove() {
        miniGame tmp = state;
        int index = 0;
        string moves = "ULDR";
        while (!tmp.isTerminal()) {
            int index = rand() % moves.size();
            tmp.playMove(moves[index]);
        }
        double score = tmp.getMyScore();  /* score between 0 and 1 */
        return score;
    }
};

class MCTS {
public:
    MCTS() {}

    string    findNextMove(miniGame& gm, int iterations) {
        auto start = std::chrono::high_resolution_clock::now();
        Node* root = new Node(gm);
        for (int i = 0; i < iterations; i++) {
     
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
        double bestVal = -1e9;
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


main() { 
    cout << mct.findNextmove() << endl;
}

2  poshur : 4
'R'
4 : stun : 2
