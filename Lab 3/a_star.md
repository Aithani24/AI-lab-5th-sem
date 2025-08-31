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

def a_star(grid, start, goal):
    rows, cols = len(grid), len(grid[0])
    
    directions = [
        (-1, 0, 1), (1, 0, 1), (0, -1, 1), (0, 1, 1),
        (-1, -1, 1.5), (-1, 1, 1.5), (1, -1, 1.5), (1, 1, 1.5) 
    ]
    
    def heuristic(a, b):
        return math.sqrt((a[0]-b[0])**2 + (a[1]-b[1])**2)
    
    open_set = []
    heapq.heappush(open_set, (heuristic(start, goal), 0, start, [start])) 
    visited = set()
    
    while open_set:
        f, g, current, path = heapq.heappop(open_set)
        
        if current in visited:
            continue
        visited.add(current)
        
        if current == goal:
            return path, g  
        
        for dx, dy, cost in directions:
            nx, ny = current[0] + dx, current[1] + dy
            if 0 <= nx < rows and 0 <= ny < cols and grid[nx][ny] == 1:
                if (nx, ny) not in visited:
                    new_g = g + cost
                    new_f = new_g + heuristic((nx, ny), goal)
                    heapq.heappush(open_set, (new_f, new_g, (nx, ny), path + [(nx, ny)]))
                    
    return None, float('inf')  

def visualize(grid, path, start, goal):
    rows, cols = len(grid), len(grid[0])
    cell_size = 60
    
    root = tk.Tk()
    root.title("A* Path Visualization")

    root.lift()
    root.attributes('-topmost', True)
    root.after_idle(root.attributes, '-topmost', False)
    
    canvas = tk.Canvas(root, width=cols*cell_size, height=rows*cell_size)
    canvas.pack()
    
    for i in range(rows):
        for j in range(cols):
            color = "blue" if grid[i][j] == 0 else "white"
            canvas.create_rectangle(
                j*cell_size, i*cell_size, (j+1)*cell_size, (i+1)*cell_size,
                fill=color, outline="black"
            )

    for (i, j) in path:
        canvas.create_rectangle(
            j*cell_size, i*cell_size, (j+1)*cell_size, (i+1)*cell_size,
            fill="green", outline="black"
        )
        root.update()
        time.sleep(0.2)
    
    si, sj = start
    gi, gj = goal

    canvas.create_rectangle(sj*cell_size, si*cell_size, (sj+1)*cell_size, (si+1)*cell_size, fill="yellow", outline="black")
    canvas.create_text(sj*cell_size + cell_size//2, si*cell_size + cell_size//2, text="S", font=("Arial", 16, "bold"))

    canvas.create_rectangle(gj*cell_size, gi*cell_size, (gj+1)*cell_size, (gi+1)*cell_size, fill="red", outline="black")
    canvas.create_text(gj*cell_size + cell_size//2, gi*cell_size + cell_size//2, text="E", font=("Arial", 16, "bold"))
    
    root.mainloop()

if __name__ == "__main__":
    n, m = map(int, input("Enter grid size (rows cols): ").split())
    grid = []
    print("Enter grid row by row (0 = river, 1 = land):")
    for i in range(n):
        row = list(map(int, input().split()))
        grid.append(row)

    sx, sy = map(int, input("Enter start position (row col): ").split())
    gx, gy = map(int, input("Enter goal position (row col): ").split())
    start, goal = (sx, sy), (gx, gy)

    path, cost = a_star(grid, start, goal)

    if path:
        print("Path found:", path)
        print("Minimum cost:", cost)
        visualize(grid, path, start, goal)
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
