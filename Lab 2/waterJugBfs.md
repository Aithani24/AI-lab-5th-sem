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

bool waterJugBFS(int jug1Cap, int jug2Cap, int target) 
{
    queue<pair<int, int>> q;
    set<pair<int, int>> visited;

    q.push({0, 0});

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        cout << "Current state: (" << curr.first << ", " << curr.second << ")\n";

        if (curr.first == target || curr.second == target || curr.first + curr.second == target) 
        {
            cout << "Reached target: (" << curr.first << ", " << curr.second << ")\n";
            return true;
        }

        if (visited.count({curr.first, curr.second})) continue;
        visited.insert({curr.first, curr.second});

        vector<pair<int, int>> nextStates;

        nextStates.push_back({jug1Cap, curr.second});

        nextStates.push_back({curr.first, jug2Cap});

        nextStates.push_back({0, curr.second});

        nextStates.push_back({curr.first, 0});

        {
            int pour = min(curr.first, jug2Cap - curr.second);
            nextStates.push_back({curr.first - pour, curr.second + pour});
        }

        {
            int pour = min(curr.second, jug1Cap - curr.first);
            nextStates.push_back({curr.first + pour, curr.second - pour});
        }

        for (auto &s : nextStates) 
        {
            if (!visited.count({s.first, s.second})) 
            {
                q.push(s);
            }
        }
    }

    return false;
}

int main() {
    int jug1Cap, jug2Cap, target;
    cout << "Enter capacity of Jug 1: ";
    cin >> jug1Cap;
    cout << "Enter capacity of Jug 2: ";
    cin >> jug2Cap;
    cout << "Enter target amount: ";
    cin >> target;

    if (waterJugBFS(jug1Cap, jug2Cap, target))
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
