# 8-Puzzle Solver using BFS

## Problem Statement

Solve the classic **8-puzzle problem** using **Breadth First Search (BFS)**.  
The puzzle is represented as a 3x3 grid containing numbers `1-8` and a blank space (`0`).  
The objective is to transform a given start state into the goal state by sliding the tiles.

---

## Approach

- Use a **queue** to explore the state space in a level-wise manner (BFS).
- At every state:
  - Find the position of the empty tile.
  - Generate all valid states by moving the tile (up, down, left, right).
  - Enqueue each new unvisited state.
- Stop when the goal state is found.

---

## Implementation Details

- Each state is a `vector<vector<int>>` (3x3 matrix).
- `std::queue` stores the current state frontier.
- `std::set` is used to avoid revisiting states.
- `findEmptyBox()` locates the position of the empty tile (0).
- The directions are defined as `(dx, dy)` pairs for moving the blank tile.

---

## Code

```cpp
#include <bits/stdc++.h>
using namespace std;

// Print current configuration
void displayGrid(vector<vector<int>>& grid) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            cout << grid[r][c] << " ";
        }
        cout << "\n";
    }
    cout << "------" << endl;
}

// Compare state with target
bool checkGoal(vector<vector<int>>& state, vector<vector<int>>& target) {
    return state == target;
}

// Find blank (0) position
pair<int,int> blankTile(vector<vector<int>>& state) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (state[r][c] == 0) return {r, c};
        }
    }
    return {-1, -1};
}

// Breadth First Search
bool breadthFirst(vector<vector<int>> start, vector<vector<int>> target) {
    queue<vector<vector<int>>> q;
    set<vector<vector<int>>> seen;
    q.push(start);

    // possible shifts: down, up, right, left
    vector<pair<int,int>> moves = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (checkGoal(node, target)) return true;
        if (seen.count(node)) continue;
        seen.insert(node);

        auto [bx, by] = blankTile(node);

        for (auto mv : moves) {
            int nr = bx + mv.first;
            int nc = by + mv.second;
            if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
                auto nextState = node;
                swap(nextState[nr][nc], nextState[bx][by]);
                if (!seen.count(nextState)) q.push(nextState);
            }
        }
    }
    return false;
}

int main() {
    vector<vector<int>> start = {{1,2,3}, {4,5,6}, {0,7,8}};
    vector<vector<int>> goal  = {{1,2,3}, {4,5,6}, {7,8,0}};

    cout << "Initial State:\n";
    displayGrid(start);

    if (breadthFirst(start, goal)) 
        cout << "Solution Found!\n";
    else 
        cout << "No Solution Possible!\n";

    return 0;
}

```

---
## Time and Space Complexity

### Time Complexity:
- **Worst-case:** O(9!)  
  - Each state is unique (up to 9! total).
  - BFS explores each state only once.
  - It ensures shortest path but visits many states.

### Space Complexity:
- O(9!) for the visited set and queue.
  - Each state is stored in memory.

---

## Use Cases
- Finds the shortest path to the goal (unlike DFS).
- Good for solving problems with minimal steps required.
- Demonstrates uninformed search algorithm in AI.
