# 8-Puzzle Solver using Hill Climbing

## Problem Statement
This program solves the 8-puzzle problem using the **Hill Climbing** search algorithm with the **misplaced tiles heuristic**.  
The algorithm starts from an initial configuration and repeatedly moves to a neighbor with a lower heuristic value, stopping when no improvement is possible.

---

## Approach
1. **Representation**:  
   - The puzzle is stored as a 3×3 matrix, with `0` representing the empty space.
2. **Heuristic Function**:  
   - Counts the number of tiles not in their correct position (ignores the empty tile).
3. **Hill Climbing Search**:  
   - At each step, evaluates all possible moves of the empty tile.
   - Selects the neighbor with the smallest heuristic value.
4. **Termination Conditions**:  
   - Goal state reached.
   - No neighbor has a lower heuristic value (local maximum/plateau).

---

## Implementation Details
- **`print_board`**: Prints the current puzzle board state.
- **`isGoal`**: Checks if the current state matches the goal state.
- **`findEmptyBox`**: Finds coordinates of the empty tile (`0`).
- **`misplaced_tiles`**: Calculates heuristic (number of misplaced tiles).
- **`get_neighbors`**: Generates all possible states by moving the empty tile in up to 4 directions.
- **`hillClimbing`**:
  - Keeps track of the current state and its heuristic.
  - Selects the neighbor with the lowest heuristic.
  - Stops if no improvement is possible.
- **`main`**:
  - Defines start and goal states.
  - Calls `hillClimbing` to solve the puzzle.

---


## Code (C++)

```cpp
#include <bits/stdc++.h>
using namespace std;

void showBoard(const vector<vector<int>>& grid) {
    for (auto &row : grid) {
        for (int val : row) cout << val << " ";
        cout << "\n";
    }
}

bool checkGoal(const vector<vector<int>>& state, const vector<vector<int>>& target) {
    return state == target;
}

pair<int,int> locateBlank(const vector<vector<int>>& state) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (state[r][c] == 0) return {r, c};
    return {-1,-1};
}

int heuristicMisplaced(const vector<vector<int>>& state, const vector<vector<int>>& target) {
    int cnt = 0;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (state[r][c] != 0 && state[r][c] != target[r][c]) cnt++;
    return cnt;
}

vector<vector<vector<int>>> expandNeighbors(const vector<vector<int>>& state) {
    vector<vector<vector<int>>> result;
    auto [br, bc] = locateBlank(state);
    vector<pair<int,int>> moves = {{1,0},{-1,0},{0,1},{0,-1}};

    for (auto [dr, dc] : moves) {
        int nr = br + dr, nc = bc + dc;
        if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
            auto next = state;
            swap(next[br][bc], next[nr][nc]);
            result.push_back(next);
        }
    }
    return result;
}

bool hillClimb(vector<vector<int>> start, vector<vector<int>> target) {
    auto current = start;
    int h_curr = heuristicMisplaced(current, target);

    cout << "Hill climbing started...\n";
    showBoard(current);
    cout << "Heuristic: " << h_curr << "\n\n";

    while (true) {
        if (checkGoal(current, target)) {
            cout << "Reached the goal!\n";
            return true;
        }

        auto nbrs = expandNeighbors(current);
        auto best = current;
        int h_best = h_curr;

        for (auto &nb : nbrs) {
            int h = heuristicMisplaced(nb, target);
            if (h < h_best) {
                h_best = h;
                best = nb;
            }
        }

        if (h_best >= h_curr) {
            cout << "No better neighbor, stopping.\n";
            return false;
        }

        current = best;
        h_curr = h_best;

        showBoard(current);
        cout << "Heuristic: " << h_curr << "\n\n";
    }
}

int main() {
    vector<vector<int>> start = {{1,2,3}, {4,5,6}, {0,7,8}};
    vector<vector<int>> goal  = {{1,2,3}, {4,5,6}, {7,8,0}};

    hillClimb(start, goal);
    return 0;
}


```
---
## Time Complexity
- **Worst case**: \( O(b \times d) \)  
  where:
  - \( b \) = branching factor (up to 4 moves)  
  - \( d \) = number of steps taken until termination.
- Typically fast but may terminate prematurely.

## Space Complexity
- **O(1)** additional space (only current state and neighbors stored at each step).

---

## Use Cases
- Demonstrating the limitations of local search algorithms.
- Educational purposes for AI search methods.
- Comparing performance with other heuristics (e.g., Manhattan distance).

---

## Limitations
- **Local Optima**: Can get stuck in a local minimum.
- **Plateaus**: May stop if all neighbors have equal heuristic values.
- **No Backtracking**: Once stuck, it does not explore alternative paths.
- **Not Optimal**: May find a solution that is not the shortest.
