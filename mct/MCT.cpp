#include "header.hpp"
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