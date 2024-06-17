#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <random>
#include <ctime>
using namespace std;

class TicTac {
    char currentPlayer;
    vector<string> board;
    int sz;

public:
    TicTac(int sz) : sz(sz), currentPlayer('x'), board(sz, string(sz, '.')) {}

    void printBoard() {
        for (auto &row : board) {
            for (char j : row) cout << j << ' ';
            cout << endl;
        }
    }

    void playMove(pair<int, int> move) {
        board[move.first][move.second] = currentPlayer;
        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    }

    void undoMove(int i, int j) {
        board[i][j] = '.';
        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    }

    bool isTerminal() {
        return checkWin('x') || checkWin('o') || checkDraw();
    }

    char getCurrentPlayer() {
        return currentPlayer;
    }

    vector<pair<int, int>> getPossibleMoves() {
        vector<pair<int, int>> possibleMoves;
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                if (board[i][j] == '.') {
                    possibleMoves.push_back({i, j});
                }
            }
        }
        return possibleMoves;
    }

    bool checkWin(char player) {
        // Check rows, columns, and diagonals
         map<int, int> row, col;
        int diag1 = 0, diag2 = 0;
        for(int i = 0; i < sz; i++) {
            for(int j = 0; j < sz; j++) {
                if (board[i][j] != player) continue;
                row[i]++;
                col[j]++;
                if (i == j) diag1++;
                if (i == sz - 1 - j) diag2++;
                if (row[i] == sz || col[j] == sz || diag1 == sz || diag2 == sz) return true;
            }
        }
        return false;
    }

    bool checkDraw() {
        for (string &s : board) {
            for (char c : s) if (c == '.') return false;
        }
        return true;
    }

    char getWinner() {
        if (checkWin('o')) return 'o';
        if (checkWin('x')) return 'x';
        return 'n';
    }
};

struct Node {
    TicTac state;
    Node *parent;
    int visitCount;
    double winScore;
    pair<int, int> move;
    vector<Node*> children;

    Node(const TicTac& state, Node* parent = nullptr, pair<int, int> mv = {-1, -1})
        : state(state), parent(parent), visitCount(0), winScore(0), move(mv) {}

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }

    bool isFullyExpanded() {
        return children.size() == state.getPossibleMoves().size();
    }

    Node* bestChild(double explorationParam) {
        Node* bestNode = nullptr;
        double bestValue = -std::numeric_limits<double>::infinity();
        for (auto child : children) {
            double uctValue = (child->winScore / (child->visitCount + 1)) +
                              explorationParam * std::sqrt(std::log(visitCount + 1) / (child->visitCount + 1));
            if (uctValue > bestValue) {
                bestValue = uctValue;
                bestNode = child;
            }
        }
        return bestNode;
    }
};

class MCTS {
    int iterations;
    std::default_random_engine generator;

public:
    MCTS(int iterations) : iterations(iterations), generator(time(0)) {}

    pair<int, int> findNextMove(TicTac& game) {
        Node* root = new Node(game);
        for (int i = 0; i < iterations; i++) {
            Node* promisingNode = selectPromisingNode(root);
            if (!promisingNode->state.isTerminal()) {
                expandNode(promisingNode);
                Node* nodeToExplore = promisingNode;
                if (!nodeToExplore->children.empty()) {
                    std::uniform_int_distribution<int> distribution(0, nodeToExplore->children.size() - 1);
                    nodeToExplore = nodeToExplore->children[distribution(generator)];
                }
                double result = simulateMove(nodeToExplore);
                backpropagation(nodeToExplore, result);
            }
        }
        Node* bestNode = root->bestChild(0);
        pair<int, int> bestMove = bestNode->move;
        delete root;
        return bestMove;
    }

private:
    Node* selectPromisingNode(Node* rootNode) {
        Node* node = rootNode;
        while (!node->children.empty()) {
            node = node->bestChild(1.41);
        }
        return node;
    }

    void expandNode(Node* node) {
        vector<pair<int, int>> moves = node->state.getPossibleMoves();
        for (pair<int, int>& move : moves) {
            TicTac newState = node->state;
            newState.playMove(move);
            Node* newNode = new Node(newState, node, move);
            node->children.push_back(newNode);
        }
    }

    double simulateMove(Node* node) {
        TicTac tmp = node->state;
        while (!tmp.isTerminal()) {
            vector<pair<int, int>> moves = tmp.getPossibleMoves();
            std::uniform_int_distribution<int> distribution(0, moves.size() - 1);
            tmp.playMove(moves[distribution(generator)]);
        }
        char winner = tmp.getWinner();
        if (winner == 'o') return 1;
        if (winner == 'n') return 0.5;
        return 0;
    }

    void backpropagation(Node* node, double result) {
        while (node != nullptr) {
            node->winScore += result;
            node->visitCount++;
            node = node->parent;
        }
    }
};

int main() {
    TicTac game(3);
    MCTS mcts(1000);
    while (!game.isTerminal()) {
        game.printBoard();
        if (game.getCurrentPlayer() == 'x') {
            cout << "Player X to Move : " << endl;
            pair<int, int> move;
            cin >> move.first >> move.second;
            game.playMove(move);
        } else {
            cout << "Player O to move: it's AI" << endl;
            pair<int, int> move = mcts.findNextMove(game);
            cout << move.first << ' ' << move.second << endl;
            game.playMove(move);
        }
    }
    game.printBoard();
    if (game.checkWin('o')) cout << "Player O won\n";
    else if (game.checkWin('x')) cout << "Player X won\n";
    else cout << "Draw\n";
}
