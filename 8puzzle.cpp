#include <bits/stdc++.h>
using namespace std;

// Display the puzzle board
void showBoard(vector<vector<int>>& state) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            cout << state[r][c] << " ";
        }
        cout << "\n";
    }
    cout << "------" << endl;
}

// Check if current state is equal to target state
bool isTarget(vector<vector<int>>& curr, vector<vector<int>>& target) {
    return curr == target;
}

// Locate position of blank tile (0)
pair<int,int> locateBlank(vector<vector<int>>& state) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (state[r][c] == 0) return {r,c};
        }
    }
    return {-1,-1}; 
}

// DFS search
bool depthFirstSearch(vector<vector<int>> start, vector<vector<int>> target) {
    stack<vector<vector<int>>> stk;
    set<vector<vector<int>>> seen;
    stk.push(start);

    // possible moves: down, up, right, left
    vector<pair<int,int>> moves = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!stk.empty()) {
        auto node = stk.top();
        stk.pop();

        if (isTarget(node, target)) return true;
        if (seen.count(node)) continue;
        seen.insert(node);

        auto [br, bc] = locateBlank(node);

        for (auto mv : moves) {
            int nr = br + mv.first;
            int nc = bc + mv.second;
            if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
                auto newState = node;
                swap(newState[nr][nc], newState[br][bc]);
                if (!seen.count(newState)) stk.push(newState);
            }
        }
    }
    return false;
}

int main() {
    vector<vector<int>> startState = {{1,2,3},{4,5,6},{0,7,8}};
    vector<vector<int>> goalState  = {{1,2,3},{4,5,6},{7,8,0}};

    cout << "Initial Configuration:\n";
    showBoard(startState);

    if (depthFirstSearch(startState, goalState)) 
        cout << "Solution Found!\n";
    else 
        cout << "No Solution Exists!\n";

    return 0;
}
