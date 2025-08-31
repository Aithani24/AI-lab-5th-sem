# 8-Puzzle Solver using Greedy Best-First Search

## Problem Statement
The program solves the 8-puzzle problem using the **Greedy Best-First Search (GBFS)** algorithm with the **misplaced tiles heuristic**.  
It starts from an initial puzzle configuration and tries to reach the goal configuration by prioritizing states with fewer misplaced tiles.

---

## Approach
1. **Representation**:  
   - Puzzle is stored as a 3×3 matrix with `0` representing the empty space.
2. **Heuristic Function**:  
   - Counts the number of tiles that are not in their goal position (`misplaced tiles`).
3. **Greedy Best-First Search**:  
   - Maintains a priority queue sorted by heuristic value.
   - Expands the state with the smallest heuristic score.
4. **State Expansion**:  
   - Moves the empty space up, down, left, or right to generate new states.
5. **Cycle Prevention**:  
   - Uses a `visited` set to avoid revisiting the same state.

---

## Implementation Details
- **`print_board`**: Displays the current puzzle configuration in grid format.
- **`isGoal`**: Compares the current puzzle state with the goal state.
- **`findEmptyBox`**: Finds the position of the empty tile (`0`) in the puzzle.
- **`misplaced_tiles`**: Calculates the number of tiles out of place, ignoring the empty tile.
- **`boardToString`**: Converts the 2D board into a string for use in the visited set.
- **`greedyBestFirst`**:
  - Uses a **priority queue** to store states ordered by heuristic value.
  - Pops the best state, checks for goal, and generates valid moves.
  - Skips already visited states to prevent infinite loops.
- **`main`**:
  - Defines the initial and goal states.
  - Prints the starting state.
  - Calls `greedyBestFirst` to attempt solving.

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

pair<int,int> locateBlank(const vector<vector<int>>& grid) {
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (grid[r][c] == 0) return {r, c};
    return {-1,-1};
}

int h_misplaced(const vector<vector<int>>& grid, const vector<vector<int>>& target) {
    int cnt = 0;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (grid[r][c] != 0 && grid[r][c] != target[r][c]) cnt++;
    return cnt;
}

string encodeBoard(const vector<vector<int>>& grid) {
    string res;
    for (auto &row : grid)
        for (int val : row) res += to_string(val);
    return res;
}

bool bestFirstSearch(vector<vector<int>> start, vector<vector<int>> target) {
    auto cmp = [&](const vector<vector<int>>& a, const vector<vector<int>>& b) {
        return h_misplaced(a, target) > h_misplaced(b, target);
    };

    priority_queue<vector<vector<int>>, vector<vector<vector<int>>>, decltype(cmp)> pq(cmp);
    unordered_set<string> seen;
    pq.push(start);

    vector<pair<int,int>> moves = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!pq.empty()) {
        auto node = pq.top(); pq.pop();

        if (checkGoal(node, target)) {
            cout << "Solution Found!\n";
            showBoard(node);
            return true;
        }

        string key = encodeBoard(node);
        if (seen.count(key)) continue;
        seen.insert(key);

        auto [r, c] = locateBlank(node);

        for (auto [dr, dc] : moves) {
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
                auto next = node;
                swap(next[r][c], next[nr][nc]);
                string newKey = encodeBoard(next);
                if (!seen.count(newKey)) pq.push(next);
            }
        }
    }

    cout << "No Solution Possible!\n";
    return false;
}

int main() {
    vector<vector<int>> start = {{1,2,3}, {4,5,6}, {0,7,8}};
    vector<vector<int>> goal  = {{1,2,3}, {4,5,6}, {7,8,0}};

    cout << "Initial State:\n";
    showBoard(start);

    bestFirstSearch(start, goal);
    return 0;
}

```

---

## Time Complexity
- **Worst case**: \( O(b^d) \)  
  where:
  - \( b \) = branching factor (up to 4 for the 8-puzzle)  
  - \( d \) = depth of the shallowest goal node  
- GBFS may explore many nodes due to its lack of optimality guarantees.

## Space Complexity
- **O(n)** where n is the number of unique states stored in memory (visited set + priority queue).

---

## Use Cases
- Educational demonstration of heuristic search.
- Understanding strengths and weaknesses of GBFS.
- Introductory AI assignments for search algorithms.

---

## Limitations
- **Not Optimal**: May find a suboptimal path.
- **Not Complete**: May fail to find a solution even when one exists (depends on heuristic).
- **Performance**: Can still explore many unnecessary states for complex puzzles.
- **Heuristic Weakness**: Misplaced tiles heuristic doesn't consider distance, so it's less informed than Manhattan distance.
