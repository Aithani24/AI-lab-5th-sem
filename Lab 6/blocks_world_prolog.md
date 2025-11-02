# Blocks World Planner – Prolog Implementation

This repository contains a classic Artificial Intelligence planner for the **Blocks World** domain, implemented in pure Prolog.

Unlike a simple state-space search, this planner uses a **goal-regression** (or **Means-Ends Analysis**) approach. It works backward from the desired goal, finding actions to solve subgoals and recursively finding plans to solve the preconditions for those actions. Prolog's built-in backtracking and unification mechanisms make it an incredibly elegant and powerful language for this type of planner.



---

## Problem Statement

The goal is to implement an automated planner that can find a sequence of valid moves to transform an initial state into a goal state.

**1. The Domain:**
* A set of distinct blocks (e.g., `a`, `b`, `c`).
* A table (the "floor").
* A robot arm that can move one block at a time.

**2. The State:**
A state is represented as a **list of Prolog facts (predicates)**.
* `[on(a, b), on_table(c), clear(a), clear(c), arm_empty]`

**3. The Operators (Actions):**
Actions are defined as Prolog facts, detailing their **preconditions**, **add-list**, and **delete-list** (in STRIPS format).

* **`pickup(X)`:** Pick up block `X` from the table.
    * *Preconditions:* `[clear(X), on_table(X), arm_empty]`
    * *Add-list:* `[holding(X)]`
    * *Delete-list:* `[on_table(X), clear(X), arm_empty]`

* **`putdown(X)`:** Put down block `X` onto the table.
    * *Preconditions:* `[holding(X)]`
    * *Add-list:* `[on_table(X), clear(X), arm_empty]`
    * *Delete-list:* `[holding(X)]`

* **`stack(X, Y)`:** Stack block `X` on top of block `Y`.
    * *Preconditions:* `[holding(X), clear(Y)]`
    * *Add-list:* `[on(X, Y), clear(X), arm_empty]`
    * *Delete-list:* `[holding(X), clear(Y)]`

* **`unstack(X, Y)`:** Unstack block `X` from block `Y`.
    * *Preconditions:* `[on(X, Y), clear(X), arm_empty]`
    * *Add-list:* `[holding(X), clear(Y)]`
    * *Delete-list:* `[on(X, Y), clear(X), arm_empty]`

**4. The Objective:**
Given an **`InitialState`** (a list of facts) and a **`GoalState`** (a list of facts), find a **`Plan`** (a list of actions) that achieves the goal.

---

## Approach

This planner implements a **recursive, goal-directed search** (Means-Ends Analysis). The core of the planner is the `plan(State, Goal, Plan)` predicate.

**Core Logic:**
1.  **`plan(State, Goal, Plan)`:** This predicate asserts, "The `Plan` will transform the `State` into a new state that satisfies the `Goal`."

2.  **Base Case:**
    * `plan(State, Goal, []) :- is_subset(Goal, State).`
    * If the `Goal` is already a subset of the `State`, then the goal is achieved. The plan required is empty (`[]`).

3.  **Recursive Step:**
    * `plan(State, Goal, [Action | PlanTail]) :- ...`
    * If the goal is *not* met, we must find an `Action` to perform.
    * The planner first **selects a subgoal `G`** from the `Goal` list that is not yet true in the `State`.
    * It then **finds an `Action`** (using the `action/4` facts) that has `G` in its `add-list`.
    * Prolog's **backtracking** is key here: it will automatically try *every* action that could satisfy the subgoal.
    * Once an `Action` is chosen, the planner gets its `Preconditions`.
    * It then **recursively calls `plan` to achieve these `Preconditions`**. This generates a sub-plan, `PrePlan`.
    * After the `PrePlan` is "executed" (applied to the state), the `Action` itself is applied.
    * Finally, the planner **recursively calls `plan` again to solve for the original `Goal`** in this new, updated state. The result is the `PlanTail`.
    * The final plan is constructed by appending: `[PrePlan, Action | PlanTail]`.

**Helper Predicates:**
* **`is_subset(List1, List2)`:** True if all members of `List1` are present in `List2`.
* **`apply_action(State, Add, Del, NewState)`:** Creates `NewState` by removing `Del` from `State` and adding `Add`.
* **`apply_plan(State, Plan, FinalState)`:** Recursively applies each action in a `Plan` to a `State` to get a `FinalState`.

This approach is far more focused than a blind state-space search, as it only considers actions that are relevant to achieving an unsatisfied goal.

---

## Code (Prolog)

This is the complete, runnable Prolog code for the planner.

```prolog
% blocks_world.pl
% A goal-regression planner for the Blocks World problem.

:- module(blocks_world,
    [
        plan/3,
        initial_state/1,
        goal_state/1
    ]).

:- op(100, xfx, 'pre'). % for preconditions
:- op(100, xfx, 'add'). % for add-list
:- op(100, xfx, 'del'). % for delete-list

% ---
% PLANNER CORE
% ---

% plan(State, Goal, Plan)
% Base Case: The goal is already satisfied.
plan(State, Goal, []) :-
    is_subset(Goal, State).

% Recursive Step: The goal is not satisfied.
plan(State, Goal, [Action | PlanTail]) :-
    % 1. Select a goal G that is not in the current state
    member(G, Goal),
    \+ member(G, State),
    
    % 2. Find an action that achieves G
    action(Action, Pre, Add, Del),
    member(G, Add),
    
    % 3. Find a plan to achieve the preconditions of that action
    plan(State, Pre, PrePlan),
    
    % 4. Apply the PrePlan
    apply_plan(State, PrePlan, StateAfterPre),
    
    % 5. Apply the Action itself
    apply_action(StateAfterPre, Add, Del, StateAfterAction),
    
    % 6. Plan for the original goal in the new state
    plan(StateAfterAction, Goal, PlanTail).

% ---
% ACTION DEFINITIONS (STRIPS)
% action(Name, Preconditions, AddList, DeleteList)
% ---

action(stack(X, Y),
    [clear(Y), holding(X)],
    [on(X, Y), clear(X), arm_empty],
    [clear(Y), holding(X)]
).

action(unstack(X, Y),
    [on(X, Y), clear(X), arm_empty],
    [clear(Y), holding(X)],
    [on(X, Y), clear(X), arm_empty]
).

action(pickup(X),
    [on_table(X), clear(X), arm_empty],
    [holding(X)],
    [on_table(X), clear(X), arm_empty]
).

action(putdown(X),
    [holding(X)],
    [on_table(X), clear(X), arm_empty],
    [holding(X)]
).

% ---
% HELPER PREDICATES
% ---

% apply_plan(State, Plan, FinalState)
% Applies a list of actions to a state.
apply_plan(State, [], State).
apply_plan(State, [Action | RestPlan], FinalState) :-
    action(Action, _, Add, Del),
    apply_action(State, Add, Del, NextState),
    apply_plan(NextState, RestPlan, FinalState).

% apply_action(State, Add, Del, NewState)
% The core state transition function.
apply_action(State, Add, Del, NewState) :-
    list_difference(State, Del, StateAfterDel),
    union(StateAfterDel, Add, NewState).

% is_subset(Subset, Set)
% True if all elements of Subset are in Set.
is_subset([], _).
is_subset([H | T], Set) :-
    member(H, Set),
    is_subset(T, Set).

% ---
% LIST UTILITIES (Standard Prolog definitions)
% ---

% list_difference(List, Del, Result)
% Removes all elements of Del from List.
list_difference([], _, []).
list_difference([H | T], Del, Result) :-
    member(H, Del),
    !,
    list_difference(T, Del, Result).
list_difference([H | T], Del, [H | Result]) :-
    list_difference(T, Del, Result).

% union(List1, List2, Result)
% Combines two lists as sets.
union([], L, L).
union([H | T], L, Result) :-
    member(H, L),
    !,
    union(T, L, Result).
union([H | T], L, [H | Result]) :-
    union(T, L, Result).

% member(X, List) - This is built-in in most Prologs,
% but included for completeness.
member(X, [X | _]).
member(X, [_ | T]) :-
    member(X, T).

% ---
% EXAMPLE PROBLEM
% ---

% The Sussman Anomaly
% Initial: C on A, B on table
% Goal: A on B, B on C
initial_state([on(c, a), on_table(a), on_table(b), clear(c), clear(b), arm_empty]).
goal_state([on(a, b), on(b, c)]).

% Simple stack
% initial_state([on_table(a), on_table(b), clear(a), clear(b), arm_empty]).
% goal_state([on(a, b)]).
```
## Time Complexity
- **Worst Case:** O(b^d)  
  - b = branching factor (number of possible actions per state).  
  - d = depth (number of steps to reach the goal).  
- As this uses **DFS**, performance can degrade for deep or cyclic search spaces.

## Space Complexity
- **O(d)** in recursion depth (stack-based search).  
- Additional space for visited states.

---

## Use Cases
- Demonstrating **AI planning** and **state-space search** concepts.  
- Foundation for understanding **STRIPS**, **Goal Regression**, and **Automated Planning**.  
- Educational use in **Artificial Intelligence** or **Logic Programming** courses.

---

## Limitations
- Uses **DFS**, which is **not optimal** — may not find the shortest plan.  
- **No heuristic guidance** — exhaustive search may take longer for large problems.  
- State representation is simplified; scaling to many blocks is computationally expensive.  
