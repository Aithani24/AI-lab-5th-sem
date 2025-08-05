#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

typedef vector<vector<int>> Matrix;
typedef vector<int> Coord;

void displayBoard(Matrix board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool isSameBoard(Matrix a, Matrix b) {
    for (int r = 0; r < a.size(); r++) {
        for (int c = 0; c < a[0].size(); c++) {
            if (a[r][c] != b[r][c])
                return false;
        }
    }
    return true;
}

Matrix swapWithZero(Matrix board, Coord zeroPos, Coord swapPos) {
    int temp = board[swapPos[0]][swapPos[1]];
    board[swapPos[0]][swapPos[1]] = 0;
    board[zeroPos[0]][zeroPos[1]] = temp;
    return board;
}

Coord locateZero(Matrix board) {
    Coord pos = {-1, -1};
    for (int r = 0; r < board.size(); r++) {
        for (int c = 0; c < board[0].size(); c++) {
            if (board[r][c] == 0) {
                pos[0] = r;
                pos[1] = c;
                break;
            }
        }
    }
    return pos;
}

vector<Matrix> getNextStates(Matrix board) {
    Coord zero = locateZero(board);
    vector<Matrix> kids;

    int row = zero[0], col = zero[1];

    if (row > 0) {
        kids.push_back(swapWithZero(board, zero, {row - 1, col}));
    }

    if (row < board.size() - 1) {
        kids.push_back(swapWithZero(board, zero, {row + 1, col}));
    }

    if (col > 0) {
        kids.push_back(swapWithZero(board, zero, {row, col - 1}));
    }

    if (col < board[0].size() - 1) {
        kids.push_back(swapWithZero(board, zero, {row, col + 1}));
    }

    return kids;
}

bool alreadySeen(Matrix board, vector<Matrix>& visited) {
    for (auto& b : visited) {
        if (isSameBoard(b, board)) return true;
    }
    return false;
}

void runDFS(Matrix start, Matrix goal) {
    stack<Matrix> nodes;
    vector<Matrix> visited;

    nodes.push(start);

    while (!nodes.empty()) {
        Matrix current = nodes.top();
        nodes.pop();

        if (isSameBoard(current, goal)) {
            cout << "Solution found!!\n";
            displayBoard(current);
            return;
        }

        visited.push_back(current);

        vector<Matrix> children = getNextStates(current);
        for (auto& child : children) {
            if (!alreadySeen(child, visited)) {
                nodes.push(child);
            }
        }
    }

    cout << "No possible Solution found\n";
}

void runBFS(Matrix start, Matrix goal) {
    queue<Matrix> nodeQueue;
}

int main() {
    Matrix goalState = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    Matrix startState = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };

    runDFS(startState, goalState);

    return 0;
}
