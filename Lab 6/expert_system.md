# Simple Expert System – Animal Identification (Common Lisp)

## Problem Statement
This project implements a basic **rule-based expert system** in **Common Lisp** for **animal identification**.
It guides a user through a series of yes/no questions about an animal's features (e.g., "Does it have fur?", "Can it fly?") to deduce its identity.

The goal is to demonstrate a simple AI using **knowledge representation** and a **rule-based inference engine** to solve a classification problem.

---

## Approach
1.  **Knowledge Base (Rules)**
    -   The knowledge is encoded as a **decision tree** using nested `cond` statements.
    -   Each path through the tree represents a logical rule:
        -   (Has Feathers) AND (Cannot Fly) AND (Swims) → **Penguin**
        -   (Has Fur) AND (Has Dark Spots) AND (Runs Fast) → **Cheetah**
        -   (Has Feathers) AND (Can Fly) → **Sparrow**

2.  **User Interaction**
    -   A helper function `ask` is used to pose a yes/no question to the user.
    -   It standardizes the input (accepts "y", "yes", etc.) and returns a boolean (`t` for yes, `nil` for no).

3.  **Inference Mechanism**
    -   The system uses **forward chaining**. It starts with no facts and gathers new facts by asking the user.
    -   As facts are established (e.g., `(has fur) = t`), the system moves deeper into the `cond` logic, narrowing down the possibilities until it reaches a conclusion.

4.  **Execution Flow**
    -   `start-identification` → The main entry point.
    -   `identify-animal` → Contains the core `cond` logic (the "inference engine").
    -   `ask` → The I/O function called by `identify-animal`.

---

## Implementation Details
-   **Language**: Common Lisp
-   **Main Functions**:
    -   `ask` → Manages user I/O and parses y/n responses.
    -   `identify-animal` → The core function containing the rule-base.
    -   `start-identification` → A simple wrapper to begin the program.
-   **Inference Type**: Rule-based forward chaining.
-   **Rule Structure**: A nested `cond` provides a more efficient question-asking flow than flat `and` clauses, as it avoids asking irrelevant questions.

---

## Code

```lisp
;; --- Simple Expert System in Common Lisp ---
;; Example: Animal Identification System

(defun ask (prompt)
  "Asks a user a yes/no question and returns a boolean."
  (format t "~%~a [y/n]: " prompt)
  (finish-output) ; Ensures the prompt appears before read-line
  (let ((response (string-downcase (read-line))))
    (or (string= response "y") (string= response "yes"))))

(defun identify-animal ()
  "Core inference engine with the knowledge base."
  (format t "~%--- Animal Identification Expert System ---~%")
  (format t "Please answer 'y' (yes) or 'n' (no) to the following...~%")
  
  (cond
    ;; Rule branch 1: Mammals
    ((ask "Does the animal have fur?")
     (cond
       ((ask "Does it have a very long neck?")
        (format t "Diagnosis: It might be a GIRAFFE.~%"))
       ((ask "Does it have dark spots and run very fast?")
        (format t "Diagnosis: It might be a CHEETAH.~%"))
       ((ask "Does it bark?")
        (format t "Diagnosis: It might be a DOG.~%"))
       (t (format t "Diagnosis: A mammal, but not one I know.~%"))))
    
    ;; Rule branch 2: Birds
    ((ask "Does the animal have feathers?")
     (cond
       ((ask "Can it fly?")
        (format t "Diagnosis: It might be a SPARROW.~%"))
       ((ask "Is it a large bird that cannot fly and swims?")
        (format t "Diagnosis: It might be a PENGUIN.~%"))
       (t (format t "Diagnosis: A bird, but not one I know.~%"))))
    
    ;; Rule branch 3: Reptiles
    ((ask "Does the animal have scales?")
     (cond
       ((ask "Does it have legs?")
        (format t "Diagnosis: It might be a LIZARD.~%"))
       ((ask "Does it have no legs?")
        (format t "Diagnosis: It might be a SNAKE.~%"))
       (t (format t "Diagnosis: A reptile, but not one I know.~%"))))
    
    ;; Default case
    (t
     (format t "Diagnosis: I cannot identify this animal.~%"))))

(defun start-identification ()
  "Entry point for the system."
  (identify-animal))

;; To run, call: (start-identification)
```
## Time Complexity
- **O(n)** where *n* is the number of questions (symptom checks).  
- Each rule is evaluated sequentially.

## Space Complexity
- **O(1)** — Minimal memory usage for rule evaluation.

---

## Use Cases
- Demonstrates **basic expert systems** in AI.  
- Educational example for **knowledge representation and reasoning**.  
- Can be extended for:
  - Animal disease diagnosis  
  - Fault detection systems  
  - Medical chatbot prototypes  

---

## Limitations
- **Limited Knowledge Base** — Only a few predefined conditions.  
- **No Uncertainty Handling** — Does not account for probabilistic reasoning.  
- **Static Rules** — Rules must be manually modified to add new diseases or symptoms.  
- **No Learning Mechanism** — Does not a
