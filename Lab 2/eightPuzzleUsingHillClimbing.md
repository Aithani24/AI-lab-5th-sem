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

void print_board(const vector<vector<int>>& board) 
{
    for (auto &row : board) 
    {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
}

bool isGoal(const vector<vector<int>>& puzzle, const vector<vector<int>>& goal) 
{
    return puzzle == goal;
}

pair<int,int> findEmptyBox(const vector<vector<int>>& puzzle) 
{
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(puzzle[i][j] == 0)
                return {i,j};
    return {-1,-1};
}

int misplaced_tiles(const vector<vector<int>>& puzzle, const vector<vector<int>>& goal) 
{
    int count = 0;
    for(int i=0; i<3; i++) 
    {
        for(int j=0; j<3; j++) 
        {
            if (puzzle[i][j] != 0 && puzzle[i][j] != goal[i][j])
                count++;
        }
    }
    return count;
}

vector<vector<vector<int>>> get_neighbors(const vector<vector<int>>& board) 
{
    vector<vector<vector<int>>> neighbors;
    auto [x, y] = findEmptyBox(board);
    vector<pair<int,int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};

    for(auto [dx,dy] : dirs)
    {
        int nx = x + dx, ny = y + dy;
        if(nx>=0 && ny>=0 && nx<3 && ny<3){
            auto new_board = board;
            swap(new_board[x][y], new_board[nx][ny]);
            neighbors.push_back(new_board);
        }
    }
    return neighbors;
}

bool hillClimbing(vector<vector<int>> start, vector<vector<int>> goal) 
{
    vector<vector<int>> current = start;
    int current_h = misplaced_tiles(current, goal);

    cout << "Starting hill climbing...\n";
    print_board(current);
    cout << "Heuristic: " << current_h << "\n\n";

    while (true) 
    {
        if (isGoal(current, goal)) {

            cout << "Goal reached!\n";
            return true;
        }

        auto neighbors = get_neighbors(current);
        vector<vector<int>> best_neighbor = current;
        int best_h = current_h;

        for (auto &nb : neighbors) 
        {
            int h = misplaced_tiles(nb, goal);
            if (h < best_h) 
            {
                best_h = h;
                best_neighbor = nb;
            }
        }

        if (best_h >= current_h) 
        {
            cout << "No better neighbor found. Stopping.\n";
            return false;
        }

        current = best_neighbor;
        current_h = best_h;

        print_board(current);
        cout << "Heuristic: " << current_h << "\n\n";
    }
}

int main() 
{
    vector<vector<int>> puzzle = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    vector<vector<int>> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    hillClimbing(puzzle, goal);
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
