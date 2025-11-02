# Positional Chess AI with Minimax & Alpha-Beta Pruning

This project implements a chess-playing AI in Python using the `python-chess` library. The AI uses the **Minimax** algorithm with **Alpha-Beta Pruning** to determine the best move.

Unlike simple material-counting engines, this AI also uses **Piece-Square Tables (PSTs)** for a more sophisticated positional evaluation.

## 🚀 Features

* **Minimax Algorithm:** Explores future game states to find the optimal move.
* **Alpha-Beta Pruning:** Significantly optimizes the Minimax search by "pruning" branches that are guaranteed to be worse than a previously found move.
* **Positional Evaluation:** The AI doesn't just count material. It uses Piece-Square Tables to understand that pieces are more valuable in certain positions (e.g., knights in the center).
* **Interactive Play:** Play a full game against the AI from the console.
* **Configurable Depth:** Choose the AI's search depth (i.e., how many moves it looks ahead) at the start of the game.

## 🛠️ How It Works

### 1. Board Representation
The project uses the `python-chess` library to handle all board logic, including move generation, move validation, and checking for game-over conditions (checkmate, stalemate).

### 2. Evaluation Function (`evaluate_position`)
This is the "brain" of the AI. It assigns a numerical score to a given board state. A positive score favors White, and a negative score favors Black.

The score is a combination of two things:
1.  **Material:** The standard piece values (Pawn=100, Knight=320, etc.).
2.  **Position:** A bonus or penalty from **Piece-Square Tables (PSTs)**. For example, a pawn on the 7th rank gets a large bonus, while a knight on the back rank gets a penalty.

### 3. Search Algorithm (`minimax_search`)
The AI uses a recursive Minimax algorithm to explore the game tree.

* The **Maximizing Player** (White) tries to find the move that leads to the highest possible score.
* The **Minimizing Player** (Black) tries to find the move that leads to the lowest possible score.

### 4. Optimization (`alpha-beta pruning`)
A standard Minimax search is incredibly slow ($O(b^d)$). This project implements **Alpha-Beta Pruning**, which dramatically speeds up the search.

* `alpha`: The best (highest) score found so far for the Maximizing player.
* `beta`: The best (lowest) score found so far for the Minimizing player.

If the algorithm finds a move for the minimizing player that is *worse* (higher) than what the maximizing player is *already guaranteed* (alpha), it stops searching that branch. This "pruning" results in a best-case time complexity of $O(b^{d/2})$.

## ⚙️ How to Run

1.  **Install dependencies:**
    ```bash
    pip install python-chess
    ```

2.  **Run the script:**
    ```bash
    python chess_ai.py
    ```

3.  **Follow the prompts:**
    * Enter a search depth (3-5 is recommended for reasonable speed).
    * Choose to play as 'w' (White) or 'b' (Black).
    * Enter your moves in UCI format (e.g., `e2e4`, `g1f3`).

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


def minimax(board: chess.Board, depth: int, alpha: int, beta: int, maximizing: bool) -> int:
    if depth == 0 or board.is_game_over():
        return evaluate_board(board)

    if maximizing:
        max_eval = -99999
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, alpha, beta, False)
            board.pop()
            max_eval = max(max_eval, eval)
            alpha = max(alpha, eval)
            if beta <= alpha:  
                break
        return max_eval
    else:
        min_eval = 99999
        for move in board.legal_moves:
            board.push(move)
            eval = minimax(board, depth - 1, alpha, beta, True)
            board.pop()
            min_eval = min(min_eval, eval)
            beta = min(beta, eval)
            if beta <= alpha:  
                break
        return min_eval


def find_best_move(board: chess.Board, depth: int) -> chess.Move:
    best_move = None
    best_value = -99999

    for move in board.legal_moves:
        board.push(move)
        board_value = minimax(board, depth - 1, -100000, 100000, False)
        board.pop()
        if board_value > best_value:
            best_value = board_value
            best_move = move

    return best_move


def play_game():
    board = chess.Board()
    depth = 6  
    print("Welcome to Chess AI (Minimax + Alpha-Beta Pruning)")
    print(f"Search depth fixed at {depth} moves.\n")
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
            ai_move = find_best_move(board, depth)
            board.push(ai_move)
            print(f"\nAI plays: {ai_move}")
            print(board)

    print("\nGame Over:", board.result())


if __name__ == "__main__":
    play_game()
```

---
## Time Complexity
- Plain Minimax: **O(b^d)**  
- With Alpha-Beta Pruning: **O(b^(d/2))** (best case)  
  - **b** = branching factor (~35 legal moves in chess).  
  - **d** = search depth (set to 6).  

## Space Complexity
- **O(d)** recursion depth (stack space).  

---

## Limitations

* **Evaluation is still simple:** While better than material-only, the evaluation doesn't account for complex concepts like pawn structure, king safety, or open files.
* **Fixed Depth:** The search depth is fixed. Modern engines use *iterative deepening* and *quiescence search* to search more efficiently.
* **No Transposition Table:** The AI re-computes the evaluation for the same position if it's reached through different move orders.
