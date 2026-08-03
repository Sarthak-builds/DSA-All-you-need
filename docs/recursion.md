# Recursion

## 1. What is recursion?

A function that solves a problem by calling itself on a **smaller version of the same problem**, until it reaches a version so small it can be answered directly.

Every recursive function needs two parts:

1. **Base case** — the smallest input, answered directly, no further recursive call. This is what stops the recursion.
2. **Recursive case** — breaks the current problem into a smaller version of itself, calls the function again, and combines that result into the answer for the current call.

If you forget the base case (or never actually reach it), the function calls itself forever until the program crashes with a **stack overflow** (see §4).

```cpp
returnType functionName(parameters) {
    if (/* base case condition */) {
        return baseResult;              // stop here
    }
    // recursive case: shrink the problem, then trust the function to solve it
    return combine(currentPiece, functionName(smallerParameters));
}
```

## 2. The call stack — how recursion actually runs

Every function call (recursive or not) gets its own **stack frame**: memory holding that call's parameters, local variables, and where to resume in the caller once it returns. Recursive calls stack these frames on top of each other.

Trace `factorial(4)`:

```cpp
int factorial(int n) {
    if (n <= 1) return 1;          // base case
    return n * factorial(n - 1);   // recursive case
}
```

Calls go **down** until the base case, then results come back **up**:

```
factorial(4)
  = 4 * factorial(3)
        = 3 * factorial(2)
              = 2 * factorial(1)
                    = 1                 <- base case hit, start unwinding
              = 2 * 1 = 2
        = 3 * 2 = 6
  = 4 * 6 = 24
```

Stack of frames at the deepest point (just before unwinding starts):

```
| factorial(1) |  <- top of stack, executing now
| factorial(2) |
| factorial(3) |
| factorial(4) |  <- bottom, called first
```

Each frame is waiting on the one above it to return before it can finish its own `n * ...` multiplication. This "wait for the call above, then finish my own work" pattern is exactly why recursion uses **O(depth) memory** — every pending call sits on the stack until it gets its answer back.

## 3. Tracing with a recursion tree

For recursion that makes **more than one call per frame**, a stack trace isn't enough — draw a tree. Take Fibonacci:

```cpp
int fib(int n) {
    if (n <= 1) return n;              // base case
    return fib(n - 1) + fib(n - 2);    // two recursive calls
}
```

`fib(4)` expands into:

```
                    fib(4)
                 /          \
            fib(3)          fib(2)
           /      \         /     \
       fib(2)   fib(1)   fib(1)  fib(0)
      /    \
  fib(1)  fib(0)
```

Notice `fib(2)` gets computed twice, `fib(1)` three times. This tree makes the **redundant work** visible in a way the stack trace above wouldn't — which is exactly why plain recursive Fibonacci is exponential time (see §5) and why memoization (caching results of calls you've already made) fixes it.

**Rule of thumb:** one recursive call per frame → trace it as a stack (linear chain). More than one recursive call per frame → draw the tree, because the same sub-problem often gets recomputed.

## 4. Stack overflow

Each pending call keeps its stack frame alive until it returns. If the recursion never reaches its base case — or the input is just too large for the recursion depth to fit in the call stack — the program crashes with a stack overflow.

```cpp
int broken(int n) {
    return broken(n - 1);   // no base case at all -> crashes
}

int alsoBroken(int n) {
    if (n == 0) return 0;
    return alsoBroken(n + 1);   // moves away from the base case -> never terminates
}
```

Two things to check in every recursive function you write:
- Does a base case exist for every possible input?
- Does every recursive call move the input **strictly closer** to a base case?

## 5. Time and space complexity

- **Time**: count the total number of calls made, and the work done per call (excluding the recursive call itself).
  - `factorial(n)`: n calls, O(1) work each → **O(n)**.
  - `fib(n)` (naive, two calls per frame, no caching): the call count roughly doubles per level → **O(2^n)**.
- **Space**: determined by the **maximum depth** of the call stack at any one time, not the total number of calls.
  - `factorial(n)`: deepest chain is n frames → **O(n)** space.
  - `fib(n)` (naive): even though there are exponentially many calls total, only one root-to-leaf path is on the stack at once → **O(n)** space, despite O(2^n) time.

This time/space distinction trips people up — a function can make an exponential number of calls while only ever using linear stack space, because most branches finish and pop off before the next one starts.

## 6. Worked examples

### 6.1 Sum of first n natural numbers

```cpp
int sum(int n) {
    if (n == 0) return 0;          // base case
    return n + sum(n - 1);         // recursive case
}
```
`sum(5)` → `5 + sum(4)` → ... → `5+4+3+2+1+0` = 15. O(n) time, O(n) space.

### 6.2 Power (x^n)

```cpp
// Naive: O(n) time
int power(int x, int n) {
    if (n == 0) return 1;
    return x * power(x, n - 1);
}

// Fast exponentiation: O(log n) time, by halving n each call
int fastPower(int x, int n) {
    if (n == 0) return 1;
    int half = fastPower(x, n / 2);
    int result = half * half;
    if (n % 2 != 0) result *= x;    // odd n: one leftover factor of x
    return result;
}
```
`fastPower` is the standard trick for turning O(n) recursion into O(log n): halve the problem instead of shrinking it by 1.

### 6.3 Reverse a string

```cpp
#include <string>
using namespace std;

string reverseStr(const string& s) {
    if (s.length() <= 1) return s;                       // base case
    return reverseStr(s.substr(1)) + s[0];                // recursive case
}
```
Each call peels off the first character and appends it after the reversed remainder: `reverseStr("abc")` = `reverseStr("bc") + 'a'` = `(reverseStr("c") + 'b') + 'a'` = `"cba"`.

### 6.4 Print 1 to n, and n to 1

```cpp
void printAscending(int n) {
    if (n == 0) return;
    printAscending(n - 1);   // recurse FIRST
    cout << n << " ";        // then print -> prints in increasing order
}

void printDescending(int n) {
    if (n == 0) return;
    cout << n << " ";         // print FIRST
    printDescending(n - 1);   // then recurse -> prints in decreasing order
}
```
This pair is the cleanest illustration of **where you put the work relative to the recursive call**: work before the call executes on the way *down* the stack (root to leaf); work after the call executes on the way *back up* (leaf to root).

### 6.5 GCD (Euclidean algorithm)

```cpp
int gcd(int a, int b) {
    if (b == 0) return a;      // base case
    return gcd(b, a % b);      // recursive case
}
```
O(log(min(a,b))) time — shrinks fast because of the modulo, not just by 1 each time.

### 6.6 Array sum / max (recursion over an index instead of a whole container)

```cpp
int arraySum(int arr[], int n) {
    if (n == 0) return 0;
    return arr[n - 1] + arraySum(arr, n - 1);
}

int arrayMax(int arr[], int n) {
    if (n == 1) return arr[0];
    int restMax = arrayMax(arr, n - 1);
    return max(arr[n - 1], restMax);
}
```
Pattern: shrink the **index/size**, not the array itself — avoids copying data on every call (unlike the `substr` approach in 6.3, which is simple to read but wastes memory copying strings).

## 7. Types of recursion

| Type | Description | Example |
|---|---|---|
| **Linear** | One recursive call per frame | `factorial`, `sum` |
| **Tree / multi-branch** | More than one recursive call per frame | `fib`, subset/combination generation |
| **Tail recursion** | The recursive call is the very last operation, nothing left to do after it returns | `gcd` above |
| **Head recursion** | The recursive call happens first, work is done after it returns | `printAscending` |
| **Indirect / mutual** | `A` calls `B`, `B` calls `A` | `isEven`/`isOdd` pair |

**Tail recursion note:** in `gcd`, once the recursive call is made, the current frame has nothing left to do — it just returns whatever comes back. Some languages/compilers optimize this into a loop (no growing stack). **C++ does not guarantee this** (no mandated tail-call optimization), so a deeply tail-recursive C++ function can still overflow the stack — don't rely on tail position alone to avoid that in C++.

## 8. Recursion vs. iteration

- Anything recursive can be rewritten iteratively (usually with an explicit stack/loop), and vice versa.
- Recursion trades stack memory (O(depth) space) for code that mirrors the problem's natural self-similar structure — often clearer for trees, graphs, backtracking, divide-and-conquer.
- Iteration avoids the stack-overflow risk and per-call overhead, and is usually preferred when the problem is naturally a simple linear loop (e.g. summing an array).
- Rule of thumb: reach for recursion when the problem is defined in terms of smaller versions of itself (trees, backtracking, divide & conquer); reach for a loop when you're just repeating the same flat step n times.

## 9. How to design a recursive solution (the "leap of faith")

1. **Define what the function promises to return**, in plain words, for arbitrary valid input (e.g. "returns n!", "returns true if a path to the target exists").
2. **Find the base case(s)**: the smallest input(s) where the answer is obvious/direct.
3. **Assume the function already works correctly for a smaller input** — don't try to mentally unroll the whole stack. This is the "leap of faith."
4. **Write the recursive case** by expressing the current answer in terms of that trusted smaller-input result.
5. **Check progress toward the base case**: does every recursive call move strictly closer to a base case?

## 10. Common pitfalls

- Missing or unreachable base case → stack overflow.
- Recursive call doesn't shrink the problem (or shrinks it in the wrong direction) → infinite recursion.
- Doing expensive work (like `substr`/slicing a string or array) on every call → hidden O(n) or worse cost per call, inflating total complexity.
- Recomputing the same sub-problem repeatedly in tree recursion (see `fib` in §3) → exponential blow-up; fix with memoization (covered later under Dynamic Programming).
- Off-by-one errors in the base case condition (`n == 0` vs `n == 1`) — always trace the smallest 1–2 inputs by hand before trusting the code.

## 11. What's next

Recursion is the foundation for: backtracking, divide-and-conquer, tree/graph traversal, and dynamic programming (recursion + memoization). Practice problems for this topic live in `practice/recursion/`.
