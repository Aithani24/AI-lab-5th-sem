# Tic-Tac-Toe AI with Minimax + Alpha-Beta Pruning (C++)

##Problem Statement
This project implements a Tic-Tac-Toe game where a human can play against an AI powered by the Minimax algorithm with Alpha-Beta Pruning.
-The board is a 3×3 grid.
-The AI plays optimally, ensuring it never loses.
-The player can choose whether the AI plays as X (first) or O (second).
-Alpha-Beta pruning optimizes the Minimax search by eliminating unnecessary branches, making the algorithm faster.

---

## Approach
1.**Board Representation**
  -The board is stored as a vector of size 10 (index 1–9 used).
  -Empty cells are represented by 2.
  -X is represented by 3, O is represented by 5.
  -Winning lines are detected using the product trick:
    3 × 3 × 3 = 27 → X wins
    5 × 5 × 5 = 125 → O wins

2.**Minimax with Alpha-Beta Pruning**
  -Recursively evaluates possible moves until win/loss/draw.
  -Alpha (α) = best value the maximizing player (AI) can guarantee so far.
  -Beta (β) = best value the minimizing player (Human) can guarantee so far.
  -If α ≥ β, further exploration of that branch is pruned (cut off).
  -Scoring:
    AI win = +10 - depth
    Human win = -10 + depth
    Draw = 0

3.**Game Loop**
  -If AI’s turn, it uses Minimax with Alpha-Beta pruning to choose the best move.
  -If Human’s turn, user is prompted to input a move (1–9).
  -After each move, the board is printed.
  -The game ends when either player wins or the board is full (draw).

---

## Implementation Details
-**displayBoard()** → Prints the current board.
-**getWinner()** → Checks if either X or O has won.
-**isBoardFull()** → Checks if the board is completely filled.
-**minimaxAlphaBeta()** → Recursive function with α–β pruning.
-**aiTurn()** → AI finds and plays the best move using Minimax + α–β pruning.
-**humanTurn()** → Accepts valid input from the human player.
-**main()**. → Handles setup, turn-taking, and game result display. 

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

int minimaxAlphaBeta(bool maximizing, int depth, int aiSymbol, int humanSymbol, int alpha, int beta) {
    int winner = getWinner();
    if (winner == aiSymbol) return 10 - depth;
    if (winner == humanSymbol) return depth - 10;
    if (isBoardFull()) return 0;

    if (maximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = aiSymbol;
                int score = minimaxAlphaBeta(false, depth + 1, aiSymbol, humanSymbol, alpha, beta);
                board[i] = 2;
                best = max(best, score);
                alpha = max(alpha, best);
                if (beta <= alpha) break; // prune
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = humanSymbol;
                int score = minimaxAlphaBeta(true, depth + 1, aiSymbol, humanSymbol, alpha, beta);
                board[i] = 2;
                best = min(best, score);
                beta = min(beta, best);
                if (beta <= alpha) break; // prune
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
            int score = minimaxAlphaBeta(false, 0, aiSymbol, humanSymbol,
                                         numeric_limits<int>::min(),
                                         numeric_limits<int>::max());
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
    cout << "Welcome to Tic-Tac-Toe (Minimax with Alpha-Beta Pruning)!\n";
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
-Without pruning: Minimax explores up to O(9!) ≈ 362,880 states.
-**With Alpha-Beta pruning:**
-Best case (perfect pruning) → O(b^(d/2)) ≈ O(√9!) ≈ manageable in real-time.
-Worst case (no pruning) → O(9!), same as plain Minimax.

## Space Complexity
- **O(d)** recursion depth.  
- Max depth = 9 (full board).  

---

## Use Cases
-Classic AI demonstration of Minimax + Alpha-Beta pruning.
-Educational tool for optimization in game-tree search.
-Shows how pruning can drastically reduce computation.  

---

## Limitations
-Still fixed to Tic-Tac-Toe (does not scale easily to larger boards).
-Pruning efficiency depends on move order (ordering best moves first improves performance).
-For Tic-Tac-Toe, speed difference is small (since state space is tiny), but impact is huge for larger games like Chess.
