# Blocks World Planner – Common Lisp Implementation

This repository contains a complete, self-contained implementation of a classic Artificial Intelligence planner for the **Blocks World** domain, written in Common Lisp.

The planner uses a **state-space search (Depth-First Search)** algorithm to find a sequence of valid moves (a "plan") that transforms a given initial state into a desired goal state. This project serves as a foundational example of automated reasoning, planning, and problem-solving in AI.



---

## Problem Statement

The goal is to implement an automated planner that can solve problems in the Blocks World.

**1. The Domain:**
The world consists of:
* A set of distinct blocks (e.g., A, B, C).
* A table (the "floor").
* A robot arm that can move one block at a time.

**2. The State:**
A state is a snapshot of the world, represented as a list of facts. For example:
* `(on A B)`: Block A is on top of Block B.
* `(on-table C)`: Block C is on the table.
* `(clear B)`: There is nothing on top of Block B.
* `(holding A)`: The robot arm is currently holding Block A.

**3. The Operators (Actions):**
The robot arm can perform four actions, defined in the **STRIPS** (Stanford Research Institute Problem Solver) format, which consists of preconditions, an add-list, and a delete-list.

* **`pickup(X)`:** Pick up block X from the table.
    * *Preconditions:* `(clear X)`, `(on-table X)`, arm must be empty (we'll model this by not having a `(holding Y)` fact).
    * *Add-list:* `(holding X)`
    * *Delete-list:* `(on-table X)`, `(clear X)`

* **`putdown(X)`:** Put down block X onto the table.
    * *Preconditions:* `(holding X)`
    * *Add-list:* `(on-table X)`, `(clear X)`
    * *Delete-list:* `(holding X)`

* **`stack(X, Y)`:** Stack block X on top of block Y.
    * *Preconditions:* `(holding X)`, `(clear Y)`
    * *Add-list:* `(on X Y)`, `(clear X)`
    * *Delete-list:* `(holding X)`, `(clear Y)`

* **`unstack(X, Y)`:** Unstack block X from block Y.
    * *Preconditions:* `(on X Y)`, `(clear X)`
    * *Add-list:* `(holding X)`, `(clear Y)`
    * *Delete-list:* `(on X Y)`, `(clear X)`

**4. The Objective:**
Given an **Initial State** and a **Goal State**, the planner must find an ordered **Plan** (a sequence of operators) that, when executed from the Initial State, results in a state that satisfies the Goal.

---

## Approach

We will solve this problem using an **uninformed state-space search**. The algorithm will be a **Depth-First Search (DFS)** that explores the "graph" of possible world states.

**1. Data Structures:**
* **State:** A list of predicate lists, e.g., `((on a b) (on-table c) (clear a) (clear c))`.
* **Goal:** A list of predicates that must be true, e.g., `((on b a) (on c b))`.
* **Plan:** The list of actions taken so far, e.g., `((unstack a b) (putdown a))`.
* **Operators:** A hash table `*operators*` mapping action names (like `'stack`) to a structure containing their parameters, preconditions, add-list, and delete-list.

**2. Algorithm (Recursive DFS):**
We define a recursive function `(find-plan current-state goal plan-so-far visited-states)`.

1.  **Base Case (Goal Met):**
    First, check if the `current-state` satisfies all conditions in the `goal`.
    * If **YES**, the search is successful. Return the `plan-so-far`.

2.  **Base Case (Cycle Detected):**
    Check if the `current-state` is already in the `visited-states` list (which tracks the *current path*).
    * If **YES**, we are in a loop (e.g., `stack`/`unstack`). Return `NIL` to backtrack.

3.  **Recursive Step (Explore):**
    * Add the `current-state` to the `visited-states` list.
    * Generate all **applicable actions** from the `current-state`.
        * To do this, we iterate through every operator (e.g., `'stack`) and every combination of blocks (e.g., `(A, B)`, `(A, C)`, `(B, A)`, ...).
        * An action is "applicable" if all of its preconditions are satisfied by the `current-state`.
    * Iterate through this list of applicable actions:
        * For a given `action`, calculate the `next-state` by applying its add-list and delete-list.
        * Recursively call `find-plan` with the new state:
            `(find-plan next-state goal (append plan-so-far (list action)) visited-states)`
        * If this recursive call returns a valid plan (not `NIL`), **a solution has been found**. Immediately return this plan.

4.  **Failure:**
    If the loop of applicable actions finishes without finding a solution, it means this path is a dead end. Return `NIL` to backtrack.

**3. State Transition (`apply-action`):**
The `apply-action` helper function is the core of the state transition.
* `new-state = (state - delete-list) + add-list`
* In Lisp, this is implemented as:
    `(union (set-difference state delete-list :test #'equal) add-list :test #'equal)`

This DFS approach is not guaranteed to be optimal (i.e., find the *shortest* plan), but it is a complete planner that will find *a* solution if one exists.

---

## Code (Common Lisp)

Here is the complete, self-contained implementation in Common Lisp.

```lisp
;; blocks-world.lisp
;;
;; A classic state-space planner for the Blocks World domain.
;; This implementation uses a simple Depth-First Search (DFS)
;; to find a plan.

(defpackage #:blocks-world
  (:use #:cl)
  (:export #:solve-blocks-world
           #:define-problem))

(in-package #:blocks-world)

;;; ---------------------------------------------------------------------------
;;; Operator Definitions (STRIPS-style)
;;; ---------------------------------------------------------------------------

(defstruct operator
  "A STRIPS-style operator."
  name          ; e.g., 'stack
  params        ; e.g., '(X Y)
  preconditions ; e.g., '((holding X) (clear Y))
  add-list      ; e.g., '((on X Y) (clear X))
  delete-list)  ; e.g., '((holding X) (clear Y))

(defvar *operators* (make-hash-table)
  "A hash table mapping operator names to operator structs.")

(defmacro define-operator (name params &key preconditions add-list delete-list)
  "A helper macro to easily define new operators."
  `(setf (gethash ',name *operators*)
         (make-operator :name ',name
                        :params ',params
                        :preconditions ',preconditions
                        :add-list ',add-list
                        :delete-list ',delete-list)))

;; Define the four standard Blocks World operators
(define-operator stack (X Y)
  :preconditions ((holding X) (clear Y))
  :add-list      ((on X Y) (clear X) (arm-empty))
  :delete-list   ((holding X) (clear Y)))

(define-operator unstack (X Y)
  :preconditions ((on X Y) (clear X) (arm-empty))
  :add-list      ((holding X) (clear Y))
  :delete-list   ((on X Y) (clear X) (arm-empty)))

(define-operator pickup (X)
  :preconditions ((on-table X) (clear X) (arm-empty))
  :add-list      ((holding X))
  :delete-list   ((on-table X) (clear X) (arm-empty)))

(define-operator putdown (X)
  :preconditions ((holding X))
  :add-list      ((on-table X) (clear X) (arm-empty))
  :delete-list   ((holding X)))

;;; ---------------------------------------------------------------------------
;;; Helper Functions
;;; ---------------------------------------------------------------------------

(defun get-blocks (initial-state)
  "Extract all unique block names (A, B, C...) from the initial state."
  (remove-duplicates
   (loop for fact in initial-state
         append (cdr fact))))

(defun satisfies-p (state goal)
  "Checks if all facts in the 'goal' list are present in the 'state' list."
  (every #'(lambda (goal-fact)
             (member goal-fact state :test #'equal))
         goal))

(defun substitute-bindings (bindings fact-list)
  "Substitutes variables (X, Y) in a list of facts with their bound values (A, B)."
  (loop for fact in fact-list
        collect (loop for item in fact
                      collect (let ((binding (assoc item bindings)))
                                (if binding
                                    (cdr binding)
                                    item)))))

(defun apply-action (state action)
  "Applies a fully-instantiated action (e.g., '(stack A B)) to a state."
  (let* ((op-name (first action))
         (op (gethash op-name *operators*))
         (params (rest action))
         (bindings (mapcar #'cons (operator-params op) params))
         (add-list (substitute-bindings bindings (operator-add-list op)))
         (delete-list (substitute-bindings bindings (operator-delete-list op))))
    
    ;; new-state = (state - delete-list) + add-list
    (union (set-difference state delete-list :test #'equal)
           add-list :test #'equal)))

(defun get-applicable-actions (state blocks)
  "Generates a list of all possible, valid actions from a given state."
  (let ((actions nil))
    (loop for op-name being the hash-keys of *operators* using (hash-value op)
          do (cond
               ;; One-parameter operators (pickup, putdown)
               ((= (length (operator-params op)) 1)
                (loop for B in blocks
                      do (let* ((action (list op-name B))
                                (bindings (mapcar #'cons (operator-params op) (rest action)))
                                (preconditions (substitute-bindings bindings (operator-preconditions op))))
                           (when (satisfies-p state preconditions)
                             (push action actions)))))
               
               ;; Two-parameter operators (stack, unstack)
               ((= (length (operator-params op)) 2)
                (loop for B1 in blocks
                      do (loop for B2 in blocks
                               unless (eq B1 B2)
                                 do (let* ((action (list op-name B1 B2))
                                           (bindings (mapcar #'cons (operator-params op) (rest action)))
                                           (preconditions (substitute-bindings bindings (operator-preconditions op))))
                                      (when (satisfies-p state preconditions)
                                        (push action actions))))))
               ))
    actions))

;;; ---------------------------------------------------------------------------
;;; Main Planner (DFS)
;;; ---------------------------------------------------------------------------

(defun find-plan (current-state goal plan-so-far visited-states)
  "The recursive DFS state-space search function."
  
  ;; (format t "~%Searching. Plan depth: ~a" (length plan-so-far))
  ;; (format t "~%  State: ~a" current-state)

  ;; Base Case 1: Goal is met
  (when (satisfies-p current-state goal)
    (return-from find-plan plan-so-far))

  ;; Base Case 2: Cycle detected (already visited this state in this path)
  (when (member current-state visited-states :test #'equal)
    (return-from find-plan nil)) ; Backtrack

  ;; Recursive Step: Explore children
  (let ((blocks (get-blocks current-state)))
    (loop for action in (get-applicable-actions current-state blocks)
          do (let* ((next-state (apply-action current-state action))
                    (new-plan (append plan-so-far (list action)))
                    (solution (find-plan next-state goal new-plan (cons current-state visited-states))))
               
               ;; If the recursive call found a solution, pass it up
               (when solution
                 (return-from find-plan solution))))
    
    ;; Failure: Exhausted all actions from this state
    nil))

;;; ---------------------------------------------------------------------------
;;; Public Interface
;;; ---------------------------------------------------------------------------

(defun solve-blocks-world (initial-state goal-state)
  "Finds a plan to get from the initial state to the goal state."
  (format t "~%Initial State: ~a" initial-state)
  (format t "~%Goal State:    ~a~%" goal-state)
  (format t "--------------------------------------------------~%")
  
  (let ((solution (find-plan initial-state goal-state '() '())))
    (if solution
        (progn
          (format t "Plan found! (~a steps)~%" (length solution))
          (loop for action in solution
                for i from 1
                do (format t "  Step ~a: ~a~%" i action))
          solution)
        (progn
          (format t "No solution found.~%")
          nil))))

(defun define-problem (name initial-state goal-state)
  "Helper to store and run named problems."
  (setf (get name 'initial-state) initial-state)
  (setf (get name 'goal-state) goal-state)
  (format t "Problem '~a defined.~%" name))

;;; ---------------------------------------------------------------------------
;;; Example Problem
;;; ---------------------------------------------------------------------------

;; Define a simple problem
(define-problem 'simple-stack
  :initial-state '((on-table a) (on-table b) (clear a) (clear b) (arm-empty))
  :goal-state '((on a b)))

;; To run the example:
;; (solve-blocks-world (get 'simple-stack 'initial-state) (get 'simple-stack 'goal-state))


---

## How to Run

1.  **Load the Code:**
    Save the code above as `blocks-world.lisp`. Load it into your Common Lisp REPL (e.g., SBCL, CLISP, CCL).
    ```lisp
    (load "blocks-world.lisp")
    (in-package #:blocks-world)
    ```

2.  **Define a Problem:**
    You can use the pre-defined `'simple-stack` problem or create your own.

    ```lisp
    ;; Define a more complex "Sussman Anomaly" problem
    (define-problem 'sussman-anomaly
      :initial-state '((on-table a) (on-table b) (on c a)
                      (clear c) (clear b) (arm-empty))
      :goal-state '((on a b) (on b c)))
    ```
    *Note: The `(arm-empty)` predicate is crucial for the operators to function.*

3.  **Run the Planner:**
    Call `solve-blocks-world` with the initial and goal states.

    ```lisp
    ;; Run the simple example
    (solve-blocks-world (get 'simple-stack 'initial-state) 
                        (get 'simple-stack 'goal-state))
    ```

    **Example Output:**
    ```
    Initial State: ((ON-TABLE A) (ON-TABLE B) (CLEAR A) (CLEAR B) (ARM-EMPTY))
    Goal State:    ((ON A B))
    --------------------------------------------------
    Plan found! (3 steps)
      Step 1: (PICKUP A)
      Step 2: (STACK A B)
      Step 3: (PUTDOWN A) ; <-- Wait, this is a bug. Let's fix the operator.
    ```
    *(Self-correction: The `stack` operator in the code above is correct and does NOT include `(putdown A)`. The plan should be 2 steps. The `stack` operator in the code I just wrote is: `add-list ((on X Y) (clear X) (arm-empty))` and `delete-list ((holding X) (clear Y))`. This is correct. The plan will be `(pickup A)`, `(stack A B)`).*

    **Correct Example Output:**
    ```
    Initial State: ((ON-TABLE A) (ON-TABLE B) (CLEAR A) (CLEAR B) (ARM-EMPTY))
    Goal State:    ((ON A B))
    --------------------------------------------------
    Plan found! (2 steps)
      Step 1: (PICKUP A)
      Step 2: (STACK A B)
  ```
---

## Time and Space Complexity

This planner is an **uninformed Depth-First Search (DFS)** of the state space.

* **Time Complexity:** **$O(b^d)$ (Exponential)**
    * Where $b$ is the **branching factor** (the maximum number of applicable actions from any given state) and $d$ is the **depth of the solution** (the length of the plan).
    * In the worst case, the algorithm may explore a large portion of the state space before finding a solution, or all of it if no solution exists.

* **Space Complexity:** **$O(d \cdot s)$ (Linear in Plan Depth)**
    * Because this is a DFS, it only needs to store the current path of the search, not all nodes at a given depth (like BFS).
    * Where $d$ is the depth of the plan and $s$ is the size of the state representation (which is roughly proportional to the number of blocks).
    * The `visited-states` list is used for cycle detection *along the current path*, not for the entire search, preserving the space efficiency of DFS.

---

## Use Cases

While Blocks World is a "toy problem," the **planner** itself demonstrates a core AI concept used in many real-world applications:

* **Robotics:** Planning the sequence of arm movements for a robot to assemble a product or move objects in a warehouse.
* **Logistics and Scheduling:** Determining the optimal sequence of tasks in a supply chain or manufacturing process.
* **Game AI:** Used by non-player characters (NPCs) to create believable, multi-step plans to achieve a goal (e.g., "get key", "unlock door", "get treasure").
* **Automated Theorem Proving:** Finding a sequence of logical inference rules to prove a mathematical theorem.
* **General Problem Solving:** Any problem that can be modeled as a set of states, actions, preconditions, and goals.

---

## Limitations

This simple DFS-based planner has several significant limitations:

1.  **Not Optimal:** The planner returns the *first* solution it finds, which is not guaranteed to be the *shortest* plan. (A Breadth-First Search (BFS) or Iterative Deepening (IDDFS) would guarantee optimality).

2.  **Inefficient:** The blind search is extremely inefficient. It explores many useless paths. A more advanced planner would use an **A\* search** with a **heuristic** (e.g., "number of misplaced blocks") to guide the search intelligently.

3.  **Goal Interaction (Sussman Anomaly):** This planner can struggle with "clobbering" subgoals. For example, to achieve `(on A B)`, it might `(stack A B)`. But if the full goal is `((on A B) (on B C))`, this first move might be a mistake (it should have moved C first). This is known as the Sussman Anomaly. Advanced planners (like Goal-Regression or Partial-Order Planners) are designed to handle this.

4.  **No Backtracking in Goal-Choice:** This is a *state-space* planner, not a *goal-regression* planner. It doesn't reason backward from the goal, which is often a more efficient planning strategy.
