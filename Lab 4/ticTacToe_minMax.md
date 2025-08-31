# Tic-Tac-Toe with Minimax AI (C++)

## Problem Statement
This project implements a **Tic-Tac-Toe game** where a human can play against an **AI powered by the Minimax algorithm**.  
- The board is a 3×3 grid.  
- The AI plays optimally, ensuring it never loses.  
- The player can choose whether the AI plays as **X (first)** or **O (second)**.

---

## Approach
1. **Board Representation**:  
   - The board is stored as a vector of size 10 (index 1–9 used).  
   - Empty cells are represented by **2**.  
   - X is represented by **3**, O is represented by **5**.  
   - Winning lines are detected using the **product trick**:
     - 3 × 3 × 3 = 27 → X wins  
     - 5 × 5 × 5 = 125 → O wins  

2. **Minimax Algorithm**:  
   - Recursively evaluates all possible moves until the game reaches a win, loss, or draw.  
   - **Maximizing player** = AI, tries to maximize score.  
   - **Minimizing player** = Human, tries to minimize score.  
   - Scoring:  
     - AI win = `+10 - depth`  
     - Human win = `-10 + depth`  
     - Draw = 0  

3. **Game Loop**:  
   - If AI’s turn, it uses Minimax to choose the best move.  
   - If Human’s turn, user is prompted to input a move (1–9).  
   - After each move, the board is printed.  
   - The game ends when either player wins or the board is full (draw).  

---

## Implementation Details
- **`printBoard()`** → Prints the current board.  
- **`checkWinner()`** → Checks if either X or O has won.  
- **`isFull()`** → Checks if the board is completely filled.  
- **`minimax()`** → Recursive function that evaluates moves and assigns scores.  
- **`aiMove()`** → AI finds and plays the best move using Minimax.  
- **`humanMove()`** → Accepts valid input from the human player.  
- **`main()`** → Handles setup, turn-taking, and game result display.  

---
## Code

```cpp
#include <iostream>
#include <vector>
#include <limits>
using namespace std;

vector<int> board(10, 2);
int moveCount = 1;
bool aiPlaysX;

void displayBoard() {
    cout << "\n";
    for (int i = 1; i <= 9; ++i) {
        char symbol = (board[i] == 3) ? 'X' : (board[i] == 5) ? 'O' : ' ';
        cout << " " << symbol << " ";
        if (i % 3 == 0) cout << "\n";
        else cout << "|";
    }
    cout << "\n";
}

int getWinner() {
    vector<vector<int>> winPatterns = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    for (auto &line : winPatterns) {
        int product = board[line[0]] * board[line[1]] * board[line[2]];
        if (product == 27) return 3;   
        if (product == 125) return 5;  
    }
    return 0;
}

bool isBoardFull() {
    for (int i = 1; i <= 9; i++)
        if (board[i] == 2) return false;
    return true;
}

int minimax(bool maximizing, int depth, int aiSymbol, int humanSymbol) {
    int winner = getWinner();
    if (winner == aiSymbol) return 10 - depth;
    if (winner == humanSymbol) return depth - 10;
    if (isBoardFull()) return 0;

    if (maximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = aiSymbol;
                best = max(best, minimax(false, depth + 1, aiSymbol, humanSymbol));
                board[i] = 2;
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = humanSymbol;
                best = min(best, minimax(true, depth + 1, aiSymbol, humanSymbol));
                board[i] = 2;
            }
        }
        return best;
    }
}

void aiTurn() {
    int aiSymbol = aiPlaysX ? 3 : 5;
    int humanSymbol = aiPlaysX ? 5 : 3;

    int bestMove = 0, bestScore = numeric_limits<int>::min();

    for (int i = 1; i <= 9; i++) {
        if (board[i] == 2) {
            board[i] = aiSymbol;
            int score = minimax(false, 0, aiSymbol, humanSymbol);
            board[i] = 2;
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    cout << "AI chooses square " << bestMove << "\n";
    board[bestMove] = aiSymbol;
    moveCount++;
}

void humanTurn() {
    int playerMove;
    while (true) {
        cout << "Your move (1-9): ";
        cin >> playerMove;
        if (playerMove >= 1 && playerMove <= 9 && board[playerMove] == 2) {
            board[playerMove] = (moveCount % 2 == 1) ? 3 : 5;
            moveCount++;
            break;
        } else {
            cout << "Invalid move. Try again.\n";
        }
    }
}

int main() {
    cout << "Welcome to Tic-Tac-Toe (Minimax AI)!\n";
    char choice;
    cout << "Should AI play as X and start first? (y/n): ";
    cin >> choice;
    aiPlaysX = (choice == 'y' || choice == 'Y');

    displayBoard();

    while (moveCount <= 9) {
        if ((moveCount % 2 == 1) == aiPlaysX)
            aiTurn();
        else
            humanTurn();

        displayBoard();

        int winner = getWinner();
        if (winner == 3)
            { cout << (aiPlaysX ? "AI wins!\n" : "Human wins!\n"); return 0; }
        else if (winner == 5)
            { cout << (!aiPlaysX ? "AI wins!\n" : "Human wins!\n"); return 0; }
    }

    cout << "It's a draw!\n";
    return 0;
}


```
---

## Time Complexity
- The Tic-Tac-Toe board has at most 9! possible game states.  
- Minimax explores all possible moves: **O(b^d)** where:  
  - b = branching factor (≤ 9)  
  - d = depth of game tree (≤ 9)  
- In worst case: **O(9!) ≈ 362,880 states** (manageable).  

## Space Complexity
- **O(d)** = O(9), due to recursion depth.  
- Board state stored globally.  

---

## Use Cases
- Classic AI demonstration of **Minimax algorithm**.  
- Educational tool for **game theory and decision-making**.  
- Useful for teaching **recursion, backtracking, and adversarial search**.  

---

## Limitations
- **Fixed to Tic-Tac-Toe** (not scalable to larger boards like 4×4).  
- **No alpha-beta pruning** → explores all states, though pruning could optimize.  
- **Blocking vs winning is not weighted differently** (pure Minimax evaluation).  
