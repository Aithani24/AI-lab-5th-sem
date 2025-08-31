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
int turn = 1;
bool aiIsX;

void printBoard() 
{
    cout << "\n";
    for (int i = 1; i <= 9; ++i) 
    {
        char mark = (board[i] == 3) ? 'X' : (board[i] == 5) ? 'O' : ' ';
        cout << " " << mark << " ";
        if (i % 3 == 0) cout << "\n";
        else cout << "|";
    }
    cout << "\n";
}

int checkWinner() 
{
    vector<vector<int>> lines = 
    {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    for (auto &line : lines) 
    {
        int prod = board[line[0]] * board[line[1]] * board[line[2]];
        if (prod == 27) return 3;   
        if (prod == 125) return 5;  
    }
    return 0;
}

bool isFull() 
{
    for (int i = 1; i <= 9; i++) 
    {
        if (board[i] == 2) return false;
    }
    return true;
}

int minimax(bool isMaximizing, int depth, int aiSymbol, int humanSymbol) 
{
    int winner = checkWinner();
    if (winner == aiSymbol) return 10 - depth;
    if (winner == humanSymbol) return depth - 10;
    if (isFull()) return 0;

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 1; i <= 9; i++) 
        {
            if (board[i] == 2) 
            {
                board[i] = aiSymbol;
                int score = minimax(false, depth + 1, aiSymbol, humanSymbol);
                board[i] = 2;
                best = max(best, score);
            }
        }
        return best;
    } 
    
    else 
    {
        int best = numeric_limits<int>::max();
        for (int i = 1; i <= 9; i++) 
        {
            if (board[i] == 2) 
            {
                board[i] = humanSymbol;
                int score = minimax(true, depth + 1, aiSymbol, humanSymbol);
                board[i] = 2;
                best = min(best, score);
            }
        }
        return best;
    }
}

void aiMove() {
    int aiSymbol = aiIsX ? 3 : 5;
    int humanSymbol = aiIsX ? 5 : 3;
    int bestScore = numeric_limits<int>::min();
    int move = 0;

    for (int i = 1; i <= 9; i++) 
    {
        if (board[i] == 2) 
        {
            board[i] = aiSymbol;
            int score = minimax(false, 0, aiSymbol, humanSymbol);
            board[i] = 2;
            if (score > bestScore) 
            {
                bestScore = score;
                move = i;
            }
        }
    }

    cout << "AI chooses square " << move << "\n";
    board[move] = aiSymbol;
    turn++;
}

void humanMove() 
{
    int move;
    while (true) 
    {
        cout << "Your move (1-9): ";
        cin >> move;
        if (move >= 1 && move <= 9 && board[move] == 2) 
        {
            board[move] = (turn % 2 == 1) ? 3 : 5;
            turn++;
            break;
        } 
        
        else 
        {
            cout << "Invalid move. Try again.\n";
        }
    }
}

int main() 
{
    cout << "Welcome to Tic-Tac-Toe with Minimax!\n";
    char choice;
    cout << "Should AI play as X and start first? (y/n): ";
    cin >> choice;
    aiIsX = (choice == 'y' || choice == 'Y');

    printBoard();

    while (turn <= 9) 
    {
        if ((turn % 2 == 1) == aiIsX) 
        {
            aiMove();
        } 
        
        else 
        {
            humanMove();
        }

        printBoard();

        int winner = checkWinner();
        if (winner == 3) 
        {
            cout << ((aiIsX) ? "AI wins!\n" : "Human wins!\n");
            return 0;
        }

        else if (winner == 5) 
        {
            cout << ((!aiIsX) ? "AI wins!\n" : "Human wins!\n");
            return 0;
        }
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
