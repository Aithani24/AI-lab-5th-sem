# Chess AI with Minimax (Python)

## Problem Statement
This project implements a **Chess game AI** using the **Minimax algorithm**.  
- The AI evaluates moves by looking ahead a few turns (default search depth = 3).  
- The human player can choose to play as **White (first)** or **Black (second)**.  
- The AI plays legal moves only and tries to maximize its advantage.  

---

## Approach
1. **Board Representation**:  
   - Uses the [`python-chess`](https://pypi.org/project/chess/) library for board setup, move generation, and game rules.  

2. **Evaluation Function**:  
   - Assigns material values to pieces:  
     - Pawn = 1  
     - Knight = 3  
     - Bishop = 3  
     - Rook = 5  
     - Queen = 9  
     - King = 0 (not scored, since losing the king ends the game)  
   - Score = (White material − Black material).  
   - Special handling:  
     - **Checkmate**: ±9999  
     - **Stalemate / Insufficient Material**: 0  

3. **Minimax Algorithm**:  
   - Explores all legal moves up to a given depth.  
   - **Maximizing Player**: AI, tries to maximize score.  
   - **Minimizing Player**: Opponent, tries to minimize score.  

4. **Best Move Selection**:  
   - Iterates over all possible moves for AI.  
   - Chooses the move with the highest evaluation after running Minimax.  

5. **Game Loop**:  
   - Human player makes moves in **UCI format** (e.g., `e2e4`).  
   - AI responds with its calculated best move.  
   - Game continues until **checkmate, stalemate, or draw**.  

---

## Implementation Details
- **`evaluate_board(board)`** → Evaluates board based on material balance and endgame conditions.  
- **`minimax(board, depth, maximizing)`** → Recursively explores possible future moves.  
- **`find_best_move(board, depth)`** → Finds the AI’s best move using Minimax.  
- **`play_game()`** → Handles the interactive game loop with user input and AI responses.  

---

## Code

```python
import chess

def evaluate_board(board: chess.Board) -> int:
    if board.is_checkmate():
        return 9999 if board.turn == chess.BLACK else -9999
    if board.is_stalemate() or board.is_insufficient_material():
        return 0

    piece_values = {
        chess.PAWN: 1,
        chess.KNIGHT: 3,
        chess.BISHOP: 3,
        chess.ROOK: 5,
        chess.QUEEN: 9,
        chess.KING: 0
    }

    score = 0
    for piece_type in piece_values:
        score += len(board.pieces(piece_type, chess.WHITE)) * piece_values[piece_type]
        score -= len(board.pieces(piece_type, chess.BLACK)) * piece_values[piece_type]

    return score


def minimax(board: chess.Board, depth: int, maximizing: bool) -> int:
    if depth == 0 or board.is_game_over():
        return evaluate_board(board)

    if maximizing:
        max_eval = -99999
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, False)
            board.pop()
            max_eval = max(max_eval, eval)
        return max_eval
    else:
        min_eval = 99999
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, True)
            board.pop()
            min_eval = min(min_eval, eval)
        return min_eval


def find_best_move(board: chess.Board, depth: int) -> chess.Move:
    best_move = None
    best_value = -99999

    for move in board.legal_moves:
        board.push(move)
        board_value = minimax(board, depth - 1, False)
        board.pop()
        if board_value > best_value:
            best_value = board_value
            best_move = move

    return best_move


def play_game():
    board = chess.Board()
    print("Welcome to Chess AI (Minimax)!")
    print("Moves limited to depth search (default depth = 3).")
    print(board)

    choice = input("Do you want to play as White and move first? (y/n): ").strip().lower()
    user_is_white = (choice == "y")

    while not board.is_game_over():
        if (board.turn == chess.WHITE and user_is_white) or (board.turn == chess.BLACK and not user_is_white):
            move = None
            while move not in board.legal_moves:
                user_input = input("Your move (UCI format, e.g. e2e4): ")
                try:
                    move = chess.Move.from_uci(user_input)
                except:
                    print("Invalid format. Try again.")
                    continue
                if move not in board.legal_moves:
                    print("Illegal move. Try again.")
            board.push(move)
            print("\nBoard after your move:")
            print(board)
        else:
            ai_move = find_best_move(board, depth=3) 
            board.push(ai_move)
            print(f"\nAI plays: {ai_move}")
            print(board)

    print("\nGame Over:", board.result())


if __name__ == "__main__":
    play_game()
```
---        

## Time Complexity
- Minimax explores **O(b^d)** states:  
  - b = branching factor (~35 average legal moves in chess).  
  - d = search depth (default = 3).  
- Worst-case: **O(35^3) ≈ 42,875 evaluations**.  

## Space Complexity
- **O(d)** recursion depth (stack).  
- Board state handled by `python-chess`.  

---

## Use Cases
- Demonstrates **AI decision-making in board games**.  
- Educational tool for **game theory, recursion, and adversarial search**.  
- Can be extended with **Alpha-Beta pruning, heuristics, and deeper searches**.  

---

## Limitations
- **No Alpha-Beta pruning** → AI explores all moves, making it slow at higher depths.  
- **Evaluation function is simple** → Only counts material, ignores positioning, pawn structure, king safety, etc.  
- **Limited depth (3 by default)** → Cannot see far-ahead tactics or strategies.  



