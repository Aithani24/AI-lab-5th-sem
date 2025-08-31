# Magic Square Generator

## Problem Statement
Generate an *n x n* magic square—a square arrangement of numbers where the sums of numbers in each row, column, and diagonal are all equal. The program supports all valid sizes of *n* `(n ≥ 3, n ≠ 2)`.

---
## Approach
The solution uses different algorithms based on the parity of *n*:

- **Odd n:** Uses the `Siamese method (de la Loubere’s method)`, placing numbers sequentially with specific upward-right movement and wrap-around.
- **Doubly Even n (n divisible by 4):** Fills the square sequentially, then inverts certain positions based on a fixed pattern.
- **Singly Even n (even but not divisible by 4):** Divides the square into four quadrants filled with smaller odd-sized magic squares, then performs swaps in specific columns to fix the magic property.

Each method ensures the magic constant  `M = n * n² + 1` is maintained.

---
## Implementation Details
- The program reads input *n*.
- Validates input (n ≥ 3 and n ≠ 2).
- Calls the appropriate generation function based on *n*’s parity.
- Prints the generated magic square formatted neatly.
- Outputs the magic constant for verification.

---

## Code (C++)

```cpp
#include <bits/stdc++.h>
using namespace std;

void printSquare(const vector<vector<int>>& square) {
    for (auto &row : square) {
        for (auto &val : row) {
            cout << setw(4) << val;
        }
        cout << "\n";
    }
}

int magicConstant(int n) {
    return n * (n * n + 1) / 2;
}

vector<vector<int>> oddMagic(int n) {
    vector<vector<int>> magic(n, vector<int>(n, 0));
    int i = 0, j = n / 2;
    for (int num = 1; num <= n * n; ) {
        magic[i][j] = num++;
        int newi = (i - 1 + n) % n;
        int newj = (j + 1) % n;
        if (magic[newi][newj]) {
            i = (i + 1) % n;
        } else {
            i = newi;
            j = newj;
        }
    }
    return magic;
}

vector<vector<int>> doublyEvenMagic(int n) {
    vector<vector<int>> magic(n, vector<int>(n));
    int num = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            magic[i][j] = num++;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i % 4 == j % 4) || ((i % 4) + (j % 4) == 3)) {
                magic[i][j] = (n * n + 1) - magic[i][j];
            }
        }
    }
    return magic;
}

vector<vector<int>> singlyEvenMagic(int n) {
    int half = n / 2;
    int k = (n - 2) / 4;
    vector<vector<int>> sub = oddMagic(half);
    vector<vector<int>> magic(n, vector<int>(n));
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            magic[i][j] = sub[i][j];
            magic[i + half][j] = sub[i][j] + 2 * half * half;
            magic[i][j + half] = sub[i][j] + 3 * half * half;
            magic[i + half][j + half] = sub[i][j] + half * half;
        }
    }
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < k; j++) {
            swap(magic[i][j], magic[i + half][j]);
        }
        for (int j = n - k + 1; j < n; j++) {
            swap(magic[i][j], magic[i + half][j]);
        }
    }
    swap(magic[k][0], magic[k + half][0]);
    swap(magic[k][k], magic[k + half][k]);
    return magic;
}

int main() {
    int n;
    cout << "Enter n (>=3, !=2): ";
    cin >> n;
    if (n < 3 || n == 2) {
        cout << "Magic square not possible\n";
        return 0;
    }
    vector<vector<int>> magic;
    if (n % 2 == 1)
        magic = oddMagic(n);
    else if (n % 4 == 0)
        magic = doublyEvenMagic(n);
    else
        magic = singlyEvenMagic(n);
    cout << "\nMagic Square of size " << n << ":\n";
    printSquare(magic);
    cout << "\nMagic constant: " << magicConstant(n) << "\n";
    return 0;
}

```
---

## Time and Space Complexity

**Time Complexity:** O(n²) — Each method fills the n x n matrix once or twice.

**Space Complexity:** O(n²) — The square matrix stores n² integers.

---
## Use Cases
- Mathematical recreation and education.
- Generating magic squares for puzzles and games.
- Demonstrating combinatorial and parity-based algorithms.
- Teaching different algorithmic techniques for special cases.
