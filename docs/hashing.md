# Hashing

## 1. What is hashing?

Hashing is a technique for answering "how many times does this value appear?" or "have I seen this value before?" in **O(1)** average time, instead of scanning the whole input every time (O(n) with linear search).

The core idea: **map each value to a location you can jump straight to**, so "looking it up" is just reading that location instead of searching for it. That location is called a **hash**, and the simplest possible hash is: *use the value itself as an array index.*

```cpp
int arr[] = {1, 3, 2, 3, 4, 3, 1};
int hash[5] = {0};   // index 0..4 covers every possible value in arr

for (int i = 0; i < 7; i++)
    hash[arr[i]]++;   // hash[value] now holds count of that value

cout << hash[3];   // "how many 3s?" -> O(1), just read the slot
```

No searching happens at query time — the work was already done once, up front. This is the whole trade being made: **spend O(n) once to precompute, so every later question costs O(1)** instead of O(n) each time.

## 2. Why hashing (the complexity trade)

| Approach | Time per query | Setup cost |
|---|---|---|
| Linear scan every time | O(n) | none |
| Sort once, binary search each query | O(log n) | O(n log n) once |
| Hashing (precompute once) | **O(1)** | O(n) once (or O(n + maxVal), see §3) |

Hashing wins whenever you'll ask **many queries** against the **same data** — the one-time precompute cost gets amortized across every query that follows. If you only ever ask one question about the data, precomputing is pure overhead; the payoff comes from reuse.

## 3. Array-based hashing: the three-step pattern

This is the simplest and fastest form of hashing — no hash function, no collisions, just direct indexing — and it applies whenever the values being hashed are **small non-negative integers with a known (or boundable) maximum**.

### Step 1: create a hash array sized to the max value

```cpp
int maxVal = 100;               // largest value that can appear
int hash[maxVal + 1] = {0};     // +1 because indices run 0..maxVal inclusive
```

**Why `maxVal + 1`, not `maxVal`**: if the largest value is 100, you need a valid index `100` to store its count — an array of size 100 only has valid indices `0..99`. This off-by-one is the single most common bug in array-based hashing (see §7).

### Step 2: pre-calculation — one pass to populate the hash array

```cpp
for (int i = 0; i < n; i++)
    hash[arr[i]]++;
```

One linear pass over the input, O(n) total. After this, `hash[v]` holds the frequency of value `v` in the array — the *entire* answer for every possible query has already been computed, before a single query was even asked.

### Step 3: fetching — answer any query in O(1)

```cpp
cout << "Count of 7: " << hash[7] << endl;   // just an array read
```

No loop, no search — the precompute step already did the work. This is the payoff: turning repeated O(n) scans into repeated O(1) reads, at the one-time cost of a single O(n) pass.

**Put together, this is the pattern**: *build → precompute → fetch*. Every array-based hashing solution in `practice/hashing/` follows exactly these three steps; only what gets counted/stored at each index changes.

## 4. Worked example

```
arr = [1, 3, 2, 3, 4, 3, 1]
maxVal = 4
```

**Step 1**: `hash[5] = {0, 0, 0, 0, 0}` (indices 0..4)

**Step 2** — trace the precompute pass:

| i | arr[i] | hash after this step |
|---|---|---|
| 0 | 1 | [0,1,0,0,0] |
| 1 | 3 | [0,1,0,1,0] |
| 2 | 2 | [0,1,1,1,0] |
| 3 | 3 | [0,1,1,2,0] |
| 4 | 4 | [0,1,1,2,1] |
| 5 | 3 | [0,1,1,3,1] |
| 6 | 1 | [0,2,1,3,1] |

Final: `hash = [0, 2, 1, 3, 1]` — reads as "value 1 appears twice, value 2 once, value 3 three times, value 4 once."

**Step 3** — any query is now a single read: "how many 3s?" → `hash[3]` → `3`. "How many 0s?" → `hash[0]` → `0`. No re-scanning `arr`, no matter how many queries follow.

## 5. Handling negative numbers: the offset trick

Array indices can't be negative, so if values can be negative, shift every value by a constant **offset** before indexing:

```cpp
// values range from -50 to 50
int offset = 50;
int hash[101] = {0};             // 101 slots cover -50..50 after shifting

for (int i = 0; i < n; i++)
    hash[arr[i] + offset]++;     // shift into a valid non-negative index

cout << hash[-7 + offset];       // query for -7, same shift applied
```

The offset must be applied **consistently** on both the precompute pass and every fetch — forgetting it on one side is a silent bug (wrong slot read, not a crash), so read/write always use the exact same `value + offset` expression.

## 6. Character hashing: a common special case

When hashing lowercase letters (a very common pattern in string problems — anagrams, character frequency, etc.), the "value" is a character and the natural hash array size is fixed at 26:

```cpp
int hash[26] = {0};

for (char c : s)
    hash[c - 'a']++;   // maps 'a'->0, 'b'->1, ..., 'z'->25

cout << hash['e' - 'a'];   // frequency of 'e'
```

`c - 'a'` is exactly the same idea as the offset trick in §5 — shifting a value (here, a char's ASCII code) into a valid `0..25` index range. Uppercase letters, digits, or the full ASCII range just need a differently-sized array and a matching shift (`c - 'A'`, `c - '0'`, or no shift at all for `size 256`).

## 7. Common pitfalls

- **Off-by-one on array size** — `hash[maxVal]` instead of `hash[maxVal + 1]` leaves the largest value with no valid slot to write to (undefined behavior: out-of-bounds write). Always size for the **largest index you need**, not the count of distinct values.
- **Negative values without an offset** — indexing with a raw negative value is undefined behavior in C++ (no bounds check catches it at compile time). Any time values *could* be negative, apply an offset (§5) — don't assume input is always non-negative.
- **Forgetting the offset on one side** — applying `+ offset` when writing but not when reading (or vice versa) silently reads/writes the wrong slot instead of crashing, which makes it a hard bug to spot. Keep the exact same shift expression on both sides.
- **Array hashing when the max value is huge or unknown** — if `maxVal` is, say, 10⁹, an array of that size wastes enormous memory (or won't even allocate). This is exactly the case where you reach for a **hash map** (`unordered_map`) instead of a raw array — same *idea* (map value → O(1) slot), but backed by a real hash function and a table sized to the number of *distinct* values actually present, not the range of possible values. Hash maps, hash functions, and collision handling are a deeper topic covered separately as this repo's hashing coverage grows.

## 8. What's next

Array-based hashing (this doc) is the direct-indexing special case of the more general idea of hashing — mapping keys to locations via a hash function, with strategies for resolving collisions when two keys map to the same slot. That general machinery (`unordered_map`/`unordered_set`, hash functions, collision resolution) is planned as a follow-up once more hashing practice problems are worked through. Practice problems for this topic live in `practice/hashing/`.
