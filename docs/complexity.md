# Time & Space Complexity

## 1. What complexity analysis actually measures

Complexity analysis answers one question: **as the input gets bigger, how does the cost of the algorithm grow?** Not "how many seconds does it take on my laptop" (that depends on hardware, compiler, language) — but the *shape* of the growth curve, which is a property of the algorithm itself.

- **Time complexity**: how the number of basic operations grows as input size `n` grows.
- **Space complexity**: how the amount of extra memory used grows as `n` grows.

Both are expressed as a function of `n`, then simplified down to a **growth rate** — which is what asymptotic notation (§2) captures.

Why bother instead of just timing the code? Because timing tells you about *this* input, on *this* machine, *today*. An O(n²) algorithm might beat an O(n log n) one on a small input, then fall apart at scale. Complexity analysis predicts that crossover before you hit it in production.

## 2. Asymptotic notation: Big-O, Big-Omega, Big-Theta

These describe **bounds on growth rate**, not exact operation counts.

| Notation | Meaning | Describes |
|---|---|---|
| **O(f(n))** — Big-O | Grows *no faster than* `f(n)` | Upper bound — worst case is common usage |
| **Ω(f(n))** — Big-Omega | Grows *no slower than* `f(n)` | Lower bound — best case |
| **Θ(f(n))** — Big-Theta | Grows *exactly at the rate of* `f(n)` (both bounds meet) | Tight bound — typical/average behavior |

In practice, day-to-day conversation almost always uses **Big-O to mean "worst case, tight bound"** — e.g. "linear search is O(n)" really means Θ(n) in the worst case, but nobody says Θ out loud. This doc follows that convention: **O(...) below means the tight worst-case bound unless stated otherwise.**

**Formally**, `f(n) = O(g(n))` means: there exist constants `c > 0` and `n₀` such that `f(n) ≤ c · g(n)` for all `n ≥ n₀`. In plain terms: past some input size, `g(n)` (times a constant) is always an upper limit on `f(n)`. You will rarely need the formal definition day to day — the mechanical rules in §3 give the same answer faster.

## 3. How to calculate time complexity: the mechanical rules

Given code, count "basic operations" (comparisons, arithmetic, assignments — each treated as O(1)), express the count as a function of `n`, then simplify.

### 3.1 Drop constants

```cpp
for (int i = 0; i < n; i++) { ... }   // n iterations
for (int i = 0; i < n; i++) { ... }   // another n iterations
```
Total: `2n` operations → simplify to **O(n)**, not O(2n). Constant factors don't change the growth *shape*, so they're dropped.

### 3.2 Drop lower-order terms

```cpp
for (int i = 0; i < n; i++) { ... }           // O(n)
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) { ... }       // O(n^2)
```
Total: `n + n²` → as `n` grows, `n²` dominates `n` completely (at `n = 1000`, `n² ` is 1000x bigger). Simplify to **O(n²)**; the `n` term is irrelevant at scale.

### 3.3 Sequential blocks: add

```cpp
doA(n);   // O(n)
doB(n);   // O(n^2)
```
Cost is `O(n) + O(n²)` → dominated by the larger term → **O(n²)** (see §3.2).

### 3.4 Nested blocks: multiply

```cpp
for (int i = 0; i < n; i++)          // n times...
    for (int j = 0; j < m; j++) { }  // ...do m work each
```
Outer loop runs `n` times, inner loop does `m` work each time it runs → **O(n · m)**. When both loops run to `n`, that's **O(n²)**.

### 3.5 Loops that shrink multiplicatively → logarithmic

```cpp
int i = n;
while (i > 1) {
    i = i / 2;   // halves every iteration
}
```
Starting at `n` and halving each step, you reach 1 after roughly `log₂(n)` steps (how many times can you halve `n` before hitting 1?) → **O(log n)**. This is the signature pattern of binary search, and of `fastPower` in `docs/recursion.md` §6.2.

### 3.6 Recursive calls: count total calls × work per call

Same idea as loops, but the call count comes from the recursion structure instead of a loop counter (see `docs/recursion.md` §5 for the full recursion-specific treatment):

- One recursive call per frame, problem shrinks by 1 (`factorial(n)`): `n` calls, O(1) work each → **O(n)**.
- One recursive call per frame, problem **halves** (`fastPower`, binary search): `log₂(n)` calls → **O(log n)**.
- Two recursive calls per frame, no shrinking-and-caching (`fib(n)` naive): calls roughly double each level, `log₂(n)` levels deep → **O(2ⁿ)**.

For anything with more complex splitting (e.g. divide-and-conquer that splits into two halves *and* does O(n) merge work, like merge sort), the pattern is `T(n) = 2T(n/2) + O(n)`, which works out to **O(n log n)** — the full derivation belongs under Dynamic Programming/Divide & Conquer coverage, but recognizing the shape (split into k pieces of size n/k, plus f(n) combining work) is the useful takeaway here.

## 4. Common complexity classes

Ordered from fastest to slowest growth, with a typical example of each:

| Complexity | Name | Example |
|---|---|---|
| O(1) | Constant | Array index access `arr[i]`, hash map lookup |
| O(log n) | Logarithmic | Binary search, `fastPower` |
| O(n) | Linear | Linear search, single loop over array, `factorial` (recursive) |
| O(n log n) | Linearithmic | Merge sort, quicksort (average case), heap sort |
| O(n²) | Quadratic | Nested loops over the same input (bubble sort, naive pair-checking) |
| O(2ⁿ) | Exponential | Naive recursive Fibonacci, generating all subsets |
| O(n!) | Factorial | Generating all permutations, brute-force traveling salesman |

**Why the order matters in practice:** at `n = 20`, `2ⁿ` is about a million and `n!` is over 10^18 — the difference between "instant" and "the universe ends first" shows up fast. At `n = 10`, most of these classes look similar; the classification only earns its keep at scale, which is exactly why "it works on my test input" doesn't validate an algorithm's complexity.

## 5. Space complexity

Same idea as time, but counting **memory** instead of **operations**. Two things to separate:

- **Auxiliary space**: extra memory the algorithm uses *beyond* the input itself (temporary variables, recursion stack, extra arrays/structures created).
- **Total space**: auxiliary space + space taken by the input itself.

Most discussions of "space complexity" mean **auxiliary space** unless stated otherwise — the input has to be stored somewhere regardless of which algorithm you pick, so it's usually not the interesting part of the comparison.

Two common sources of auxiliary space:

1. **Extra data structures**: an algorithm that builds a new array/map/set proportional to `n` uses O(n) auxiliary space, even if it never recurses.
2. **Call stack depth** (recursive algorithms only): every pending call holds a stack frame until it returns (see `docs/recursion.md` §4.1 for the mechanics). Space complexity here is the **maximum simultaneous depth**, not the total number of calls — this is the point that trips people up most, covered in the next example.

## 6. Worked examples

### 6.1 Linear search

```cpp
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++)
        if (arr[i] == target) return i;
    return -1;
}
```
**Time**: worst case (target absent, or last element) checks all `n` elements → **O(n)**. Best case (target is `arr[0]`) is O(1), but "the" complexity people quote is the worst case unless stated otherwise (§2).
**Space**: no extra memory that grows with `n` → **O(1)** auxiliary space.

### 6.2 Binary search

```cpp
int binarySearch(int arr[], int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
```
**Time**: the search range halves every iteration (§3.5) → **O(log n)**.
**Space**: iterative version uses a fixed handful of variables regardless of `n` → **O(1)**. (A *recursive* binary search would be O(log n) space instead, because each halving step adds a stack frame — same time complexity, different space complexity, purely because of how it's implemented.)

### 6.3 Bubble sort (nested loops)

```cpp
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}
```
**Time**: outer loop runs `n` times, inner loop runs roughly `n` times too (shrinking slightly each pass, but that's a constant-factor detail — see §3.2) → **O(n²)**.
**Space**: sorts in place, no extra structures → **O(1)** auxiliary space.

### 6.4 Fibonacci: naive recursive vs. memoized

```cpp
// Naive: O(2^n) time, O(n) space (see docs/recursion.md §3 and §5 for the full trace)
int fibNaive(int n) {
    if (n <= 1) return n;
    return fibNaive(n - 1) + fibNaive(n - 2);
}

// Memoized: O(n) time, O(n) space
int fibMemo(int n, vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    return memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
}
```
Same problem, wildly different time complexity, because memoization eliminates the redundant recomputation visible in the recursion tree (`docs/recursion.md` §3). Space complexity goes from O(n) (call stack only) to O(n) (call stack **+** the memo array) — still O(n) overall since both terms are linear, but for a different reason than before. This pair is the cleanest illustration of why time and space complexity have to be reasoned about **separately**, not inferred from each other.

### 6.5 Merge sort (divide and conquer)

```cpp
void mergeSort(int arr[], int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    mergeSort(arr, l, mid);       // T(n/2)
    mergeSort(arr, mid + 1, r);   // T(n/2)
    merge(arr, l, mid, r);        // O(n) to merge two halves
}
```
**Time**: splits into 2 subproblems of size `n/2` each call, plus O(n) merge work to recombine → recurrence `T(n) = 2T(n/2) + O(n)`, which resolves to **O(n log n)**: `log n` levels of recursion, O(n) total work per level.
**Space**: O(n) auxiliary for the merge step's temporary array, plus O(log n) for the recursion stack — the O(n) merge buffer dominates, so **O(n)** overall.

## 7. Common pitfalls

- **Confusing best/worst/average case.** "O(n)" without qualification usually means worst case — but always check which case is being discussed, especially for algorithms like quicksort where average (O(n log n)) and worst (O(n²)) differ a lot.
- **Ignoring constants when they actually matter.** Asymptotically, O(n) beats O(n log n) — but an algorithm with a huge constant factor (`100n`) can lose to one with a small constant and a log factor (`n log n`) for realistic input sizes. Big-O tells you what wins *eventually*, not at every `n`.
- **Confusing time and space complexity of the same algorithm.** They're independent measurements — see the memoized Fibonacci example (§6.4), where fixing time complexity changed the *reason* for the space complexity without changing its class.
- **Miscounting recursive space as "total calls" instead of "max depth."** Naive Fibonacci makes O(2ⁿ) calls but only uses O(n) stack space, because most branches finish and pop off the stack before the next one starts (`docs/recursion.md` §5 covers this in detail).
- **Forgetting hidden costs inside "simple" operations.** `string` concatenation, `vector` resizing past capacity, or slicing (`substr`) can each be O(n) on their own — burying an O(n) cost inside what looks like a single line can silently turn an O(n) loop into O(n²) overall (see the pitfall about `substr` in `docs/recursion.md` §11).

## 8. What's next

Complexity analysis is the lens every other topic gets evaluated through — each future doc (`Sorting & Searching`, `Trees`, `Dynamic Programming`, etc.) will state time/space complexity using the vocabulary defined here. Practice problem files in this repo end every solution with a `Time Complexity` / `Space Complexity` comment block by convention (see root `CLAUDE.md`) — this doc is the reference for *why* those numbers are what they are.
