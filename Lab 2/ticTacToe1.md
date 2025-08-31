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

void Go(int n) 
{
    if (board[n] == 2) 
    {
        board[n] = (turn % 2 == 1) ? 3 : 5;
        turn++;
    }
}

int Make2() 
{
    if (board[5] == 2) return 5;
    for (int i : {2, 4, 6, 8}) 
    {
        if (board[i] == 2) return i;
    }
    return 0;
}

int Posswin(int player) 
{
    vector<vector<int>> lines = 
    {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    int target = player * player * 2;

    for (auto &line : lines) 
    {
        int a = line[0], b = line[1], c = line[2];
        int prod = board[a] * board[b] * board[c];
        if (prod == target) 
        {
            if (board[a] == 2) return a;
            if (board[b] == 2) return b;
            if (board[c] == 2) return c;
        }
    }
    return 0;
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

void aiMove() 
{
    int move = 0;
    bool aiTurnOdd = ((turn % 2 == 1) == aiIsX);

    if (aiTurnOdd) 
    {
        switch (turn) 
        {
            case 1:
                move = 1;
                break;
            case 3:
                move = (board[9] == 2) ? 9 : 3;
                break;
            case 5:
                if ((move = Posswin(3))) break;
                if ((move = Posswin(5))) break;
                move = (board[7] == 2) ? 7 : 3;
                break;
            case 7:
            case 9:
                if ((move = Posswin(3))) break;
                if ((move = Posswin(5))) break;
                for (int i = 1; i <= 9; i++) 
                {
                    if (board[i] == 2) 
                    { 
                        move = i; 
                        break; 
                    }
                }
                break;
        }
    }
    else 
    {
        switch (turn) 
        {
            case 2:
                move = (board[5] == 2) ? 5 : 1;
                break;
            case 4:
                if ((move = Posswin(3))) break;
                move = Make2();
                break;
            case 6:
                if ((move = Posswin(5))) break;
                if ((move = Posswin(3))) break;
                move = Make2();
                break;
            case 8:
                if ((move = Posswin(5))) break;
                if ((move = Posswin(3))) break;
                for (int i = 1; i <= 9; i++) 
                {
                    if (board[i] == 2) 
                    { 
                        move = i; 
                        break; 
                    }
                }
                break;
        }
    }

    if (!move) 
    {
        if ((move = Posswin(aiIsX ? 3 : 5))) {}
        else if ((move = Posswin(aiIsX ? 5 : 3))) {}
        else 
        {
            for (int i = 1; i <= 9; i++) 
            {
                if (board[i] == 2) 
                { 
                    move = i; 
                    break; 
                }
            }
        }
    }

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
