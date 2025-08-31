# Tic-Tac-Toe AI using Heuristic Board Evaluation

## Problem Statement
Build a Tic-Tac-Toe game where a human plays against an AI. The AI uses a heuristic evaluation function to choose the best move on each turn, aiming to maximize its chances of winning or blocking the opponent.

## Approach
- The game board is represented as a vector with values representing empty, X, or O.
- The AI evaluates each empty square based on:
  - Whether it’s center, corner, or edge (positional advantage).
  - The potential to complete or block winning lines (rows, columns, diagonals).
- Scores for each square are calculated by summing weights based on these criteria.
- The AI picks the move with the highest evaluation score.
- The game loops with alternating turns until a winner is found or the board is full.

## Implementation Details
- Board uses integers: `2` for empty, `3` for X, and `5` for O.
- Winning lines are predefined sets of indices.
- `evaluateSquare()` assigns scores based on position and line potential.
- AI selects the best scoring square each turn (`chooseBestMoveEval()`).
- Human input is validated for legality.
- The game checks for winner after every move using multiplication logic.
- The board is printed after every move with current markings.

## Code (C++)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> board(10, 2); 
int turn = 1;
bool aiIsX;

vector<vector<int>> winLines = {
    {1,2,3}, {4,5,6}, {7,8,9}, 
    {1,4,7}, {2,5,8}, {3,6,9}, 
    {1,5,9}, {3,5,7}           
};

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

void Go(int n) 
{
    if (n >= 1 && n <= 9 && board[n] == 2) 
    {
        board[n] = (turn % 2 == 1) ? 3 : 5;
        turn++;
    }
}

int checkWinner() 
{
    for (auto &line : winLines) 
    {
        int prod = board[line[0]] * board[line[1]] * board[line[2]];
        if (prod == 27) return 3;   
        if (prod == 125) return 5; 
    }
    return 0; 
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
            Go(move);
            break;
        } 
        else 
        {
            cout << "Invalid move. Try again.\n";
        }
    }
}

int evaluateSquare(int pos, int aiSymbol, int humanSymbol) 
{
    double score = 0;

    if (pos == 5) score += 1; 
    else if (pos == 1 || pos == 3 || pos == 7 || pos == 9) score += 0.5;

    for (auto &line : winLines) {
        if (find(line.begin(), line.end(), pos) != line.end()) 
        {
            bool aiLine = true, humanLine = true;
            for (int sq : line) {
                if (sq == pos) continue;
                if (board[sq] == humanSymbol) aiLine = false;
                if (board[sq] == aiSymbol) humanLine = false;
            }
            if (aiLine) score += 3;      
            if (humanLine) score += 2;  
        }
    }
    return score * 100; // convert to int for easier compare
}

int chooseBestMoveEval(int aiSymbol, int humanSymbol) {
    int bestMove = 0, bestScore = -1;
    for (int i = 1; i <= 9; i++) {
        if (board[i] == 2) {
            int score = evaluateSquare(i, aiSymbol, humanSymbol);
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

void aiMove() {
    int aiSymbol = aiIsX ? 3 : 5;
    int humanSymbol = aiIsX ? 5 : 3;

    int move = chooseBestMoveEval(aiSymbol, humanSymbol);

    cout << "AI chooses square " << move << "\n";
    Go(move);
}

int main() 
{
    cout << "Welcome to Tic-Tac-Toe!\n";
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

## Time and Space Complexity

**Time Complexity:**  
- Each AI move evaluates up to 9 squares, checking 8 winning lines each.  
- Overall O(72) per AI turn (constant time).

**Space Complexity:**  
- O(1) extra space beyond fixed board and predefined winning lines.

## Use Cases
- Simple Tic-Tac-Toe AI for beginners.
- Demonstrates heuristic evaluation in games.
- Useful for understanding scoring strategies without deep search.
- Educational example of AI decision-making in turn-based games.

## Limitations
- No minimax or deep search implemented; only immediate state evaluated.
- AI can be outsmarted by strategic human players.
- No long-term prediction or planning.
- Draw detection only after board fills completely.
- Limited to standard 3x3 Tic-Tac-Toe.
