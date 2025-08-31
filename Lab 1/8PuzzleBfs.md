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
# include<bits/stdc++.h>
using namespace std;

void print_board(vector<vector<int>>& board) 
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool isGoal(vector<vector<int>>& puzzle, vector<vector<int>>& goal)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(puzzle[i][j] != goal[i][j]) return false;
        }
    }

    return true;
}

void findEmptyBox(vector<vector<int>>& puzzle, int& x, int& y)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(puzzle[i][j] == 0)
            {
                x = i;
                y = j;
            }
        }
    }
}

bool bfs(vector<vector<int>>& puzzle, vector<vector<int>>& goal)
{
    queue<vector<vector<int>>> q;
    set<vector<vector<int>>> visited;
    q.push(puzzle);

    while(!q.empty())
    {
        vector<vector<int>> curr_puzzle = q.front();
        q.pop();

        if(isGoal(curr_puzzle, goal)) return true;
        if(visited.count(curr_puzzle)) continue;
        visited.insert(curr_puzzle);

        int x = -1, y = -1;
        findEmptyBox(curr_puzzle, x, y);

        vector<pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for(auto dir: dirs)
        {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if(nx >= 0 && ny >= 0 && nx < 3 && ny < 3)
            {
                vector<vector<int>> new_puzzle = curr_puzzle;
                swap(new_puzzle[nx][ny], new_puzzle[x][y]);
                if(!visited.count(new_puzzle)) q.push(new_puzzle);
            }
        }
    } 
    
    return false;
}

int main() {
    vector<vector<int>> puzzle = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    vector<vector<int>> goal = {{1,2, 3}, {4, 5, 6}, {7, 8, 0}};
    cout << "Start state:" << endl;
    print_board(puzzle);

    if (bfs(puzzle, goal)) cout << "Solution found!" << endl;
    else cout << "No solution found!" << endl;

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
