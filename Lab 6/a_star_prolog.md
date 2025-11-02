# A* Search Algorithm – Prolog Implementation

This repository contains a pure Prolog implementation of the **A* (A-star)** search algorithm, a popular and efficient algorithm for finding the optimal path in a weighted graph.

---

## Problem Statement

This project implements the A* search algorithm to find the lowest-cost path from a start node to a goal node in a graph. The algorithm uses a heuristic to guide its search, balancing the cost-so-far with an estimated cost-to-go.

---

## Approach

A* is an informed, best-first search algorithm. It works by maintaining a priority queue of paths to explore, always expanding the path that has the lowest "f-cost".

1.  **Evaluation Function:** The priority of each node is determined by the function:
    **$f(n) = g(n) + h(n)$**
    * **$g(n)$:** The actual accumulated cost from the start node to the current node $n$.
    * **$h(n)$:** The heuristic, an *admissible* (never overestimating) estimate of the cost from node $n$ to the goal.

2.  **Prolog Data Structures:**
    * **Open List:** A list of nodes to be explored, maintained as a priority queue. In this implementation, it's a **list of `state/5` terms, sorted by their f-cost**.
        * `state(Node, G, H, F, ParentState)`
    * **Closed List:** A list of nodes that have already been explored to prevent re-expansion and cycles.

3.  **Algorithm Flow:**
    * The algorithm begins with the start node in the Open List.
    * It recursively takes the best node (lowest f-cost) from the Open List.
    * **If the node is the goal,** the algorithm terminates and reconstructs the path by backtracking through the `ParentState` terms.
    * **If the node is not the goal,** it is moved to the Closed List.
    * All of its neighbors are generated. For each neighbor:
        * Its $f$, $g$, and $h$ costs are calculated.
        * If the neighbor is *not* in the Closed List, it is added to the Open List in its correctly sorted position.
    * The search repeats. If the Open List becomes empty, no path exists.

---

## Implementation Details and Code

The implementation defines the graph using `edge/3` facts and the heuristic using `h/3` facts. The core logic is in the `a_star_search/4` predicate, which manages the Open and Closed lists.

### Full Prolog Code (`a_star.pl`)

```prolog
% --- A* Search Algorithm in Prolog ---

:- op(400, yfx, ^). % Used for state representation if needed, but we use terms.

% --- Entry Point ---
% a_star(Start, Goal, Path)
% Finds the optimal Path from Start to Goal.
a_star(Start, Goal, Path) :-
    h(Start, Goal, H),
    % state(Node, G_Cost, H_Cost, F_Cost, Parent_State)
    StartNode = state(Start, 0, H, H, nil),
    
    % Call the recursive search with the initial open list
    a_star_search([StartNode], [], Goal, RevPath),
    
    % The path is built in reverse, so we reverse it.
    reverse(RevPath, Path),
    !. % Cut to commit to the first path found (which is optimal)

a_star(Start, Goal, _) :-
    format('No path found from ~w to ~w.~n', [Start, Goal]),
    fail.

% --- Recursive Search Predicates ---

% Base Case 1: Goal is found
% The head of the Open List is the goal. Build the path and stop.
a_star_search([state(Goal, _G, _H, _F, Parent) | _], _, Goal, Path) :-
    build_path(state(Goal, _G, _H, _F, Parent), [], Path).

% Recursive Step: Explore the best node
a_star_search([Current | OpenRest], Closed, Goal, Path) :-
    % Get the node name from the state term
    state(Node, _, _, _, _) = Current,
    
    % Move current node to closed list (add node name, not the whole state)
    % We check if the *node* is in the closed list
    \+ member(Node, Closed), 
    
    % Find all neighbors
    findall(Neighbor, edge(Node, Neighbor, _), Neighbors),
    
    % Process all neighbors and get the updated Open List
    process_neighbors(Neighbors, Current, Goal, OpenRest, [Node | Closed], NewOpen),
    
    % Recurse with the new open list and closed list
    a_star_search(NewOpen, [Node | Closed], Goal, Path).

% Recursive Step 2: A node is already in the closed list, so skip it.
a_star_search([Current | OpenRest], Closed, Goal, Path) :-
    state(Node, _, _, _, _) = Current,
    member(Node, Closed),
    a_star_search(OpenRest, Closed, Goal, Path).
    
% Base Case 2: No path found
a_star_search([], _, _, _) :-
    fail.

% --- Neighbor Processing ---

% process_neighbors(ListOfNeighbors, ParentState, Goal, OpenListIn, ClosedList, OpenListOut)
process_neighbors([], _, _, Open, _, Open). % Base case: no neighbors left
process_neighbors([Node | Rest], ParentState, Goal, OpenIn, Closed, OpenOut) :-
    % Skip if neighbor is already in the closed list
    member(Node, Closed),
    process_neighbors(Rest, ParentState, Goal, OpenIn, Closed, OpenOut).

process_neighbors([Node | Rest], ParentState, Goal, OpenIn, Closed, OpenOut) :-
    % Process if neighbor is NOT in the closed list
    \+ member(Node, Closed),
    state(ParentNode, ParentG, _, _, _) = ParentState,
    
    % Get cost from parent to this neighbor
    edge(ParentNode, Node, Cost),
    
    % Calculate costs for this neighbor
    NewG is ParentG + Cost,
    h(Node, Goal, NewH),
    NewF is NewG + NewH,
    NewState = state(Node, NewG, NewH, NewF, ParentState),
    
    % Add to open list (maintaining sorted order by F cost)
    insert_sorted(NewState, OpenIn, NewOpen),
    
    % Recurse for the rest of the neighbors
    process_neighbors(Rest, ParentState, Goal, NewOpen, Closed, OpenOut).

% --- List Utilities ---

% insert_sorted(Element, List, NewList)
% Inserts a state into the list, sorted by F cost (lowest first).
insert_sorted(State, [], [State]).
insert_sorted(State1, [State2 | Rest], [State1, State2 | Rest]) :-
    state(_, _, _, F1, _) = State1,
    state(_, _, _, F2, _) = State2,
    F1 =< F2. % If new state's F is lower or equal, insert at head
insert_sorted(State1, [State2 | Rest], [State2 | NewRest]) :-
    state(_, _, _, F1, _) = State1,
    state(_, _, _, F2, _) = State2,
    F1 > F2, % If new state's F is greater, check next element
    insert_sorted(State1, Rest, NewRest).

% build_path(State, Accumulator, Path)
% Backtracks from the goal state to build the path.
build_path(state(Node, _, _, _, nil), Acc, [Node | Acc]).
build_path(state(Node, _, _, _, Parent), Acc, Path) :-
    build_path(Parent, [Node | Acc], Path).

% --- Sample Graph Definition ---
% A simple map of cities.

% edge(From, To, Cost)
edge(a, b, 1).
edge(a, c, 3).
edge(b, d, 2).
edge(c, d, 1).
edge(b, g, 4).
edge(c, g, 5).
edge(d, g, 2).
edge(s, a, 1).
edge(s, g, 10).

% Heuristic (h) - Estimated (admissible) cost to goal 'g'
% This is just a lookup table; in a real app, it might be a calculation.
h(s, g, 6).
h(a, g, 5).
h(b, g, 4).
h(c, g, 3).
h(d, g, 2).
h(g, g, 0).

% --- Query Example ---
%
% ?- a_star(s, g, Path).
% Path = [s, a, b, d, g]
%
```
## Time and Space Complexity
- **Time Complexity:** \( O(b^d) \) in the worst case  
  (where \( b \) = branching factor, \( d \) = depth of the optimal solution).  
- **Space Complexity:** \( O(b^d) \), as all generated nodes are stored in memory.

---

## Use Cases
- Pathfinding in AI (maps, games, robotics).  
- Network routing and navigation systems.  
- Educational demonstration of informed search algorithms.

---

## Limitations
- **Heuristic quality impacts performance** — poor heuristics reduce efficiency.  
- **Memory-intensive** — A* stores all visited and frontier nodes.  
- **Not suitable for very large graphs** due to space complexity.  
- **Static graph only** — does not handle dynamic edge cost changes.
