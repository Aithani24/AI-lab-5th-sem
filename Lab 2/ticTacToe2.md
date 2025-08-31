# Tic-Tac-Toe with Magic Square Logic AI

## Problem Statement

Implement a **Tic-Tac-Toe** game where a human player competes against an AI opponent that leverages the mathematical properties of a **magic square** to make strategic decisions.  
The board is a 3x3 grid numbered 1 to 9, with each position mapped to a unique number in a magic square.  
The AI uses sums of 15 (a key property of the magic square) to detect winning moves and block the opponent effectively.  
The AI can play either as **X** (going first) or **O** (going second), based on user choice.

---

## Approach

- Represent the Tic-Tac-Toe board as a vector of size 10 (indexing positions 1 to 9 for convenience).  
- Each board position can hold:
  - `2` for empty,
  - `3` for X,
  - `5` for O.
- Use a predefined **magic square mapping** array to translate board positions to magic numbers:
    - Position: 1 2 3 4 5 6 7 8 9
    - Magic #: 8, 3, 4, 1, 5, 9, 6, 7, 2


- Track player moves by storing their chosen magic numbers.
- Detect possible winning or blocking moves by checking pairs of moves whose sums with a missing third number equal 15.
- On each AI turn:
- Try to win if possible.
- Block the opponent's winning move.
- Otherwise, choose the first available empty position.
- Determine the winner by checking if any combination of three magic numbers sums exactly to 15.

---

## Implementation Details

- **Board Representation:**  
A `vector<int>` of size 10, where indices 1 through 9 correspond to the board cells. Each cell holds 2 (empty), 3 (X), or 5 (O).

- **Magic Square Mapping:**  
`magicMap[10] = {0, 8, 3, 4, 1, 5, 9, 6, 7, 2};`  
Maps board indices to their magic square numbers.

- **Move Processing:**  
- `Go(int n)`: Marks the move at position `n` if empty and increments turn count.
- `getPlayerMoves(int player)`: Returns a vector of magic numbers for all positions occupied by the given player.
- `possWinMagic(int player)`: Checks if the player can win in the next move by looking for pairs summing to 15 minus the needed number; returns winning move position or 0 if none.
- `checkWinner()`: Checks if either player has a winning triple summing to 15.

- **Human Input:**  
Prompts for move until a valid, empty position is chosen.

- **AI Logic:**  
Uses magic square sums to find winning/blocking moves; otherwise, picks the first available spot.

- **Game Loop:**  
Alternates between AI and human moves, printing the board and checking for winners or draw.

---

## Code (C++)

```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<int> board(10, 2); 
int turn = 1;
bool aiIsX;

int magicMap[10] = {0, 8, 3, 4, 1, 5, 9, 6, 7, 2};

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

vector<int> getPlayerMoves(int player) 
{
    vector<int> moves;
    for (int i = 1; i <= 9; i++) 
    {
        if (board[i] == player) 
        {
            moves.push_back(magicMap[i]);
        }
    }
    return moves;
}

int possWinMagic(int player) 
{
    vector<int> moves = getPlayerMoves(player);
    int n = moves.size();
    if (n < 2) return 0;

    for (int i = 0; i < n; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            int needed = 15 - (moves[i] + moves[j]);
            if (needed < 1 || needed > 9) continue;
            for (int pos = 1; pos <= 9; pos++) 
            {
                if (magicMap[pos] == needed && board[pos] == 2) 
                {
                    return pos;
                }
            }
        }
    }
    return 0;
}

int checkWinner() 
{
    vector<int> xMoves = getPlayerMoves(3);
    vector<int> oMoves = getPlayerMoves(5);

    auto hasWon = [](const vector<int>& moves) 
    {
        int n = moves.size();
        if (n < 3) return false;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                for (int k = j + 1; k < n; k++)
                    if (moves[i] + moves[j] + moves[k] == 15)
                        return true;
        return false;
    };

    if (hasWon(xMoves)) return 3;
    if (hasWon(oMoves)) return 5;
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
    int aiSymbol = aiIsX ? 3 : 5;
    int humanSymbol = aiIsX ? 5 : 3;

    if ((move = possWinMagic(aiSymbol))) {}

    else if ((move = possWinMagic(humanSymbol))) {}

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

    cout << "AI chooses square " << move << "\n";
    Go(move);
}

int main() 
{
    cout << "Welcome to Tic-Tac-Toe with Magic Square Logic!\n";
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

- **Time Complexity:** O(1) per move  
The AI and winner checks perform fixed operations over a small, constant-sized board and precomputed combinations.

- **Space Complexity:** O(1)  
Uses a fixed-size vector and arrays; no dynamic memory grows with input size.

---

## Use Cases
- Play Tic-Tac-Toe against an AI that uses mathematical properties (magic squares) to make optimal moves.
- Demonstrate how magic squares can simplify game logic and win detection.
- Educational example of integrating mathematical concepts into game AI.
- Foundation for expanding to more complex game AI or incorporating alternative board evaluation techniques.

---

## Limitations

- Better move detection but still one-move lookahead.
- No full game-tree search or advanced planning.
- Only detects draws at game end.
- Fixed symbol mapping limits flexibility.
- Magic square method only works for 3x3 Tic-Tac-Toe.
