# Tic-Tac-Toe Rule Based Implementation

## Problem Statement

Implement a **Tic-Tac-Toe** game where a human plays against an AI.  
The AI uses a heuristic-based strategy to decide moves and can play as either **X** (first player) or **O** (second player).  
The board is a 3x3 grid with positions numbered 1 to 9.

---

## Approach

- Represent the board as a vector of integers (size 10, index 1-9 used).
- Each cell holds:
  - `2` for empty
  - `3` for X
  - `5` for O  
  (Using prime numbers allows quick win detection via multiplication.)
- Track turns and alternate moves between human and AI.
- AI decision logic:
  - Attempt to win if possible.
  - Block opponent’s immediate winning moves.
  - Take center or corners as strategic moves.
  - Follow a heuristic plan depending on the turn number.
- The game ends when a player wins or all squares are filled (draw).

---

## Implementation Details

- `printBoard()` displays the current board state.
- `Go(int n)` places a move on the board if the cell is empty.
- `Posswin(int player)` checks if `player` can win on the next move.
- `checkWinner()` evaluates if there is a winner by checking all winning lines.
- `Make2()` helps the AI select strategic positions such as center or edges.
- `humanMove()` prompts and validates user input.
- `aiMove()` applies the AI heuristic logic to select moves.
- The main loop alternates turns, printing the board and checking for a winner after each move.

---

## Code (C++)

```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<int> grid(10, 2);  
int moveCount = 1;
bool aiPlaysX;

void showBoard() {
    cout << "\n";
    for (int i = 1; i <= 9; i++) {
        char c = (grid[i] == 3 ? 'X' : (grid[i] == 5 ? 'O' : ' '));
        cout << " " << c << " ";
        if (i % 3 == 0) cout << "\n";
        else cout << "|";
    }
    cout << "\n";
}

void putMark(int pos) {
    if (grid[pos] == 2) {
        grid[pos] = (moveCount % 2 ? 3 : 5);
        moveCount++;
    }
}

int freeCornerOrCenter() {
    if (grid[5] == 2) return 5;
    for (int k : {2, 4, 6, 8}) 
        if (grid[k] == 2) return k;
    return 0;
}

int winningMove(int player) {
    vector<vector<int>> combos = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    int target = player * player * 2;
    for (auto &line : combos) {
        int a = line[0], b = line[1], c = line[2];
        int p = grid[a] * grid[b] * grid[c];
        if (p == target) {
            if (grid[a] == 2) return a;
            if (grid[b] == 2) return b;
            if (grid[c] == 2) return c;
        }
    }
    return 0;
}

int getWinner() {
    vector<vector<int>> combos = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    for (auto &line : combos) {
        int res = grid[line[0]] * grid[line[1]] * grid[line[2]];
        if (res == 27) return 3;
        if (res == 125) return 5;
    }
    return 0;
}

void playerTurn() {
    int pos;
    while (true) {
        cout << "Your move (1-9): ";
        cin >> pos;
        if (pos >= 1 && pos <= 9 && grid[pos] == 2) {
            putMark(pos);
            break;
        }
        cout << "Invalid choice. Try again.\n";
    }
}

void computerTurn() {
    int move = 0;
    bool aiTurn = ((moveCount % 2 == 1) == aiPlaysX);

    if (aiTurn) {
        switch (moveCount) {
            case 1: move = 1; break;
            case 3: move = (grid[9] == 2) ? 9 : 3; break;
            case 5:
                if ((move = winningMove(3))) break;
                if ((move = winningMove(5))) break;
                move = (grid[7] == 2 ? 7 : 3);
                break;
            default:
                if ((move = winningMove(3))) break;
                if ((move = winningMove(5))) break;
                for (int i = 1; i <= 9; i++) if (grid[i] == 2) { move = i; break; }
                break;
        }
    } else {
        switch (moveCount) {
            case 2: move = (grid[5] == 2 ? 5 : 1); break;
            case 4: 
                if ((move = winningMove(3))) break;
                move = freeCornerOrCenter();
                break;
            case 6:
                if ((move = winningMove(5))) break;
                if ((move = winningMove(3))) break;
                move = freeCornerOrCenter();
                break;
            default:
                if ((move = winningMove(5))) break;
                if ((move = winningMove(3))) break;
                for (int i = 1; i <= 9; i++) if (grid[i] == 2) { move = i; break; }
                break;
        }
    }

    if (!move) {
        if ((move = winningMove(aiPlaysX ? 3 : 5))) {}
        else if ((move = winningMove(aiPlaysX ? 5 : 3))) {}
        else for (int i = 1; i <= 9; i++) if (grid[i] == 2) { move = i; break; }
    }

    cout << "AI chooses " << move << "\n";
    putMark(move);
}

int main() {
    cout << "Welcome to Tic-Tac-Toe!\n";
    char ans;
    cout << "Should AI play as X and go first? (y/n): ";
    cin >> ans;
    aiPlaysX = (ans == 'y' || ans == 'Y');

    showBoard();

    while (moveCount <= 9) {
        if ((moveCount % 2 == 1) == aiPlaysX) computerTurn();
        else playerTurn();

        showBoard();
        int win = getWinner();

        if (win == 3) {
            cout << (aiPlaysX ? "AI wins!\n" : "You win!\n");
            return 0;
        } 
        if (win == 5) {
            cout << (!aiPlaysX ? "AI wins!\n" : "You win!\n");
            return 0;
        }
    }

    cout << "Game ends in a draw!\n";
    return 0;
}

```
---

## Time and Space Complexity

- **Time Complexity:** O(1) per move — constant checks of 8 winning lines and up to 9 board positions.
- **Space Complexity:** O(1) — fixed-size board and auxiliary variables only.

---

## Use Cases

- Play Tic-Tac-Toe against a simple AI with basic strategic thinking.
- Demonstrates prime number technique for efficient win detection.
- Serves as a base for learning game AI heuristics.
- Can be extended to more advanced AI algorithms (minimax, alpha-beta pruning).

---

## Limitations

- Uses simple heuristics, not guaranteed optimal.
- Limited lookahead, no deep strategy.
- Fixed move rules can be predictable.
- No learning or adaptation.
- Detects draws only when board is full
