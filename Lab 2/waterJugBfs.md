# Water Jug Problem Solver (BFS Approach)

## Problem Statement
Given two jugs with known capacities and a target amount of water, determine if it is possible to measure exactly the target amount using the following operations:
- Fill a jug completely.
- Empty a jug.
- Pour water from one jug to the other until one is empty or the other is full.
---

## Approach
- Use **Breadth-First Search (BFS)** to explore all possible states in order of increasing steps.
- Represent each state as `(amountInJug1, amountInJug2)`.
- From each state, generate all possible next states:
  1. Fill either jug.
  2. Empty either jug.
  3. Pour water from one jug to the other.
- Keep track of visited states to avoid repeated processing.
- BFS ensures the shortest number of steps to reach the target.

---
## Implementation Details
- `waterJugBFS`: Implements BFS using a queue to explore states level-by-level.
- `visited`: A set to store visited states to prevent revisiting.
- `nextStates`: Generated from each state by applying all valid moves.
- Main function takes jug capacities and target amount, then runs BFS.

---
## Code

```
#include <bits/stdc++.h>
using namespace std;

bool solveWaterJug(int capacityA, int capacityB, int target) 
{
    queue<pair<int,int>> statesQueue;
    set<pair<int,int>> visitedStates;

    statesQueue.push({0, 0});

    while (!statesQueue.empty()) 
    {
        auto current = statesQueue.front();
        statesQueue.pop();

        cout << "Current state: (" << current.first << ", " << current.second << ")\n";

        if (current.first == target || current.second == target || current.first + current.second == target) 
        {
            cout << "Reached target: (" << current.first << ", " << current.second << ")\n";
            return true;
        }

        if (visitedStates.count(current)) continue;
        visitedStates.insert(current);

        vector<pair<int,int>> nextMoves;

        nextMoves.push_back({capacityA, current.second});     
        nextMoves.push_back({current.first, capacityB});     
        nextMoves.push_back({0, current.second});            
        nextMoves.push_back({current.first, 0});             

        int pourToB = min(current.first, capacityB - current.second);
        nextMoves.push_back({current.first - pourToB, current.second + pourToB});

        int pourToA = min(current.second, capacityA - current.first);
        nextMoves.push_back({current.first + pourToA, current.second - pourToA});

        for (auto &state : nextMoves) 
        {
            if (!visitedStates.count(state))
                statesQueue.push(state);
        }
    }

    return false;
}

int main() 
{
    int jugA, jugB, target;
    cout << "Enter capacity of Jug A: ";
    cin >> jugA;
    cout << "Enter capacity of Jug B: ";
    cin >> jugB;
    cout << "Enter target amount: ";
    cin >> target;

    if (solveWaterJug(jugA, jugB, target))
        cout << "Solution found!\n";
    else
        cout << "No solution possible!\n";

    return 0;
}


```
---

## Time Complexity
- Worst-case: **O(jug1Cap × jug2Cap)** — all possible states explored once.
- BFS ensures minimal steps to reach the target.

## Space Complexity
- **O(jug1Cap × jug2Cap)** for visited states and BFS queue.

---

## Use Cases
- Finding the shortest sequence of steps in the water jug problem.
- AI search algorithm demonstration.
- Teaching tool for BFS and state-space exploration.

---
## Limitations
- Requires both jug capacities and target to be integers.
- Not optimized for very large capacities (can consume high memory).
- BFS may still explore many unnecessary states if target is unreachable.
