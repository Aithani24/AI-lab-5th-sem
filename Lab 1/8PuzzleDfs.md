# 8-Puzzle Solver using DFS

## Problem Statement

Implement a program that solves the 8-puzzle problem using **Depth First Search (DFS)**.  
The puzzle consists of a 3x3 grid with tiles numbered from 1 to 8 and one empty space (0).  
The goal is to move the tiles to reach the goal state.

---

## Approach

- Use a **stack** for DFS traversal.
- At each state:
  - Locate the blank (0) tile.
  - Generate all valid moves (up, down, left, right).
  - Push each new state onto the stack if it hasn’t been visited.
- Continue until the goal state is reached or all states are exhausted.

---

## Implementation Details

- The puzzle state is stored as a `vector<vector<int>>`.
- We use a `set` to track visited states to avoid cycles.
- Each move swaps the blank tile with one of its neighbors.
- The `dfs()` function performs the core search logic.

---

## Code

```cpp
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

```
---

## Time and Space Complexity

### Time Complexity:
- **Worst-case:** O((9!)²)  
  - Each state can lead to up to 4 moves.
  - Total possible states = 9! = 362,880.
  - In DFS, due to recursion and lack of optimal pruning, worst-case exploration can be exponential.

### Space Complexity:
- O(9!) for the visited set.
- O(H) for recursion stack in the worst-case path depth H (up to 9 levels deep).

---

## Use Cases
- Useful for understanding how uninformed search algorithms work.
- Demonstrates limitations of DFS (e.g., may not find shortest path, may go deep unnecessarily).
- Forms the basis for solving AI problems with state-space search.
