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

string boardToString(const vector<vector<int>>& board) 
{
    string s;
    for(auto &row : board)
        for(int x : row) s += to_string(x);
    return s;
}

bool greedyBestFirst(vector<vector<int>> start, vector<vector<int>> goal) 
{
    auto cmp = [&](const vector<vector<int>>& a, const vector<vector<int>>& b) 
    {
        return misplaced_tiles(a, goal) > misplaced_tiles(b, goal);
    };

    priority_queue<vector<vector<int>>, vector<vector<vector<int>>>, decltype(cmp)> pq(cmp);
    set<string> visited;

    pq.push(start);

    vector<pair<int,int>> directions = {{1,0},{-1,0},{0,1},{0,-1}};

    while(!pq.empty())
    {
        auto current = pq.top(); pq.pop();

        if(isGoal(current, goal))
        {
            cout << "Solution found!\n";
            print_board(current);
            return true;
        }

        string key = boardToString(current);
        if(visited.count(key)) continue;
        visited.insert(key);

        auto [x,y] = findEmptyBox(current);

        for(auto [dx,dy] : directions)
        {
            int nx = x + dx, ny = y + dy;
            if(nx>=0 && ny>=0 && nx<3 && ny<3)
            {
                auto new_board = current;
                swap(new_board[x][y], new_board[nx][ny]);
                string newKey = boardToString(new_board);
                if(visited.count(newKey)) continue;
                pq.push(new_board);
            }
        }
    }

    cout << "No solution found!\n";
    return false;
}

int main() 
{
    vector<vector<int>> puzzle = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    vector<vector<int>> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    cout << "Start state:\n";
    print_board(puzzle);

    greedyBestFirst(puzzle, goal);

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
