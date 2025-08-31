# A* Pathfinding with Tkinter Visualization

## Problem Statement
This program implements the **A\*** pathfinding algorithm to find the shortest path in a grid environment where:
- **1** represents traversable land, and  
- **0** represents blocked areas (river).  

It also provides a **Tkinter-based GUI visualization** to animate the pathfinding process step by step.

---

## Approach
1. **Grid Representation**:  
   - Input grid is represented as a 2D list where cells are either land (1) or river (0).
2. **A\* Algorithm**:  
   - Uses a priority queue (`heapq`) to explore paths based on `f = g + h`,  
     where:
     - \( g \) = cost from start to current node  
     - \( h \) = Euclidean distance heuristic from current node to goal  
3. **Moves Allowed**:  
   - 8 directions (up, down, left, right, and 4 diagonals).  
   - Cost = 1 for straight moves, 1.5 for diagonal moves.
4. **Visualization**:  
   - Tkinter draws the grid.  
   - Start = Yellow (S), Goal = Red (E), Path = Green, Obstacles = Blue.  
   - The path is animated cell by cell with a short delay.

---

## Implementation Details
- **`a_star`**:  
  - Implements A\* algorithm with Euclidean distance heuristic.  
  - Returns the path and its cost if found.
- **`heuristic`**:  
  - Computes Euclidean distance between two grid points.  
- **`visualize`**:  
  - Uses Tkinter to display the grid and animate the discovered path.  
- **`main`**:  
  - Takes user input for grid, start, and goal.  
  - Calls `a_star` and visualizes the result.

---
## Code

```python

import tkinter as tk
import math
import heapq
import time

def a_star_search(grid_map, start_cell, goal_cell):
    rows, cols = len(grid_map), len(grid_map[0])

    moves = [
        (-1, 0, 1), (1, 0, 1), (0, -1, 1), (0, 1, 1),
        (-1, -1, 1.5), (-1, 1, 1.5), (1, -1, 1.5), (1, 1, 1.5)
    ]

    def heuristic(cell_a, cell_b):
        return math.sqrt((cell_a[0]-cell_b[0])**2 + (cell_a[1]-cell_b[1])**2)

    open_queue = []
    heapq.heappush(open_queue, (heuristic(start_cell, goal_cell), 0, start_cell, [start_cell]))
    visited_cells = set()

    while open_queue:
        f_score, g_score, current_cell, path_so_far = heapq.heappop(open_queue)

        if current_cell in visited_cells:
            continue
        visited_cells.add(current_cell)

        if current_cell == goal_cell:
            return path_so_far, g_score

        for dx, dy, cost in moves:
            nx, ny = current_cell[0] + dx, current_cell[1] + dy
            if 0 <= nx < rows and 0 <= ny < cols and grid_map[nx][ny] == 1:
                neighbor = (nx, ny)
                if neighbor not in visited_cells:
                    new_g = g_score + cost
                    new_f = new_g + heuristic(neighbor, goal_cell)
                    heapq.heappush(open_queue, (new_f, new_g, neighbor, path_so_far + [neighbor]))

    return None, float('inf')


def visualize_path(grid_map, path_cells, start_cell, goal_cell):
    rows, cols = len(grid_map), len(grid_map[0])
    cell_size = 60

    root = tk.Tk()
    root.title("A* Path Visualization")

    root.lift()
    root.attributes('-topmost', True)
    root.after_idle(root.attributes, '-topmost', False)

    canvas = tk.Canvas(root, width=cols*cell_size, height=rows*cell_size)
    canvas.pack()

    for r in range(rows):
        for c in range(cols):
            color = "blue" if grid_map[r][c] == 0 else "white"
            canvas.create_rectangle(c*cell_size, r*cell_size, (c+1)*cell_size, (r+1)*cell_size, fill=color, outline="black")

    for r, c in path_cells:
        canvas.create_rectangle(c*cell_size, r*cell_size, (c+1)*cell_size, (r+1)*cell_size, fill="green", outline="black")
        root.update()
        time.sleep(0.2)

    si, sj = start_cell
    gi, gj = goal_cell
    canvas.create_rectangle(sj*cell_size, si*cell_size, (sj+1)*cell_size, (si+1)*cell_size, fill="yellow", outline="black")
    canvas.create_text(sj*cell_size + cell_size//2, si*cell_size + cell_size//2, text="S", font=("Arial", 16, "bold"))

    canvas.create_rectangle(gj*cell_size, gi*cell_size, (gj+1)*cell_size, (gi+1)*cell_size, fill="red", outline="black")
    canvas.create_text(gj*cell_size + cell_size//2, gi*cell_size + cell_size//2, text="E", font=("Arial", 16, "bold"))

    root.mainloop()


if __name__ == "__main__":
    n_rows, n_cols = map(int, input("Enter grid size (rows cols): ").split())
    grid_map = []
    print("Enter grid row by row (0 = river, 1 = land):")
    for _ in range(n_rows):
        grid_map.append(list(map(int, input().split())))

    sx, sy = map(int, input("Enter start position (row col): ").split())
    gx, gy = map(int, input("Enter goal position (row col): ").split())
    start_cell, goal_cell = (sx, sy), (gx, gy)

    path_found, min_cost = a_star_search(grid_map, start_cell, goal_cell)

    if path_found:
        print("Path found:", path_found)
        print("Minimum cost:", min_cost)
        visualize_path(grid_map, path_found, start_cell, goal_cell)
    else:
        print("No path found.")

```
---
## Time Complexity
- **Worst case**: \( O(E \log V) \)  
  where:  
  - \( V \) = number of vertices (grid cells)  
  - \( E \) = number of edges (possible moves).  
- In an \( n \times m \) grid:  
  - \( V = n \cdot m \)  
  - Each node has up to 8 neighbors, so \( E = O(8nm) \).  

**Overall**: \( O(nm \log(nm)) \).

## Space Complexity
- **O(nm)** (for visited set, priority queue, and path storage).

---

## Use Cases
- Maze solving and pathfinding in games.  
- Robotics (finding optimal movement paths).  
- GIS and navigation systems (shortest path on a map).  
- Educational demonstrations of A\* search.

---

## Limitations
- **Blocked Paths**: If the start and goal are disconnected, no solution exists.  
- **Performance**: For very large grids, performance and memory usage may become heavy.  
- **Heuristic**: Uses Euclidean distance, which may not always yield the most efficient expansions compared to Manhattan distance (if only 4-direction moves are allowed).  
- **Tkinter Animation**: Slows down visualization for large grids due to per-step rendering.
