# STL: `map` vs `unordered_map`

## 1. What these are

Both are **associative containers**: they store key-value pairs and let you look up a value by its key. Neither is a raw array-hashing trick like `docs/hashing.md` — both handle arbitrary key types (not just small non-negative ints), resizing, and collisions automatically. They differ entirely in **how** they achieve fast lookup, which is what the rest of this doc is about.

```cpp
map<string, int> m;              // ordered
unordered_map<string, int> um;   // unordered (hash table)

m["apple"] = 3;
um["apple"] = 3;
```

Same interface on the surface — the difference is all internal, and it's the internals that decide complexity, ordering, and pitfalls.

## 2. `map`: ordered, backed by a self-balancing BST

### 2.1 What's actually inside

`map` is implemented as a **self-balancing binary search tree** — in practice, a **red-black tree** in both libstdc++ (GCC) and MSVC's standard library. Every key-value pair is a node; nodes are arranged so that for any node, everything in its left subtree has a smaller key and everything in its right subtree has a larger key — the standard BST invariant.

**"Self-balancing"** means the tree actively keeps its height close to `log₂(n)` after every insert/erase, instead of letting it degrade into a linked-list shape (which plain unbalanced BSTs can do on sorted input). It does this via rotations and recoloring during insert/erase — the mechanics of red-black rebalancing are a tree-specific topic on their own (planned under Trees & BSTs), but the takeaway for `map`'s complexity is simple: **balance is actively maintained, so height is always O(log n), never O(n).**

### 2.2 Why operations are O(log n)

Every core operation — `insert`, `find`, `erase`, `count`, `operator[]`, `at` — works by **walking down the tree from the root**, comparing the target key at each node and going left or right, exactly like binary search. Since the tree's height is kept at O(log n), that walk is O(log n) comparisons, worst case, every time — not just on average.

### 2.3 Why iteration comes out sorted

A BST's **in-order traversal** (left subtree, node, right subtree) visits keys in ascending order — that's a structural property of the BST invariant itself, not something `map` computes separately. This is *why* `map` gives you sorted-by-key iteration for free:

```cpp
map<int, string> m = {{3, "c"}, {1, "a"}, {2, "b"}};
for (auto& [key, val] : m) cout << key << " ";   // prints: 1 2 3 (always sorted)
```

### 2.4 Operations that only make sense because it's ordered

Because the keys are kept sorted, `map` supports queries that `unordered_map` fundamentally cannot offer efficiently:

```cpp
m.lower_bound(2);   // iterator to first key >= 2
m.upper_bound(2);   // iterator to first key > 2
m.equal_range(2);   // pair of {lower_bound, upper_bound}
```

These work by continuing the same O(log n) tree walk and stopping at the right point — there's no equivalent efficient operation on a hash table, because a hash table has no concept of "next key in order" (see §3).

### 2.5 Uniqueness

`map` keys are unique — inserting an existing key with `operator[]` overwrites the value, and `insert` on an existing key is a no-op (returns `false` via the `pair<iterator, bool>` result). `multimap` is the sibling container that allows duplicate keys, not covered here.

## 3. `unordered_map`: average O(1), backed by a hash table

### 3.1 What's actually inside

`unordered_map` is a **hash table**: an array of **buckets**, where each bucket holds the (usually short) list of key-value pairs whose keys hashed to that bucket index. Concretely:

```
bucket_count = 8

hash("apple")  % 8 = 3   -> bucket 3: [("apple", 3)]
hash("banana") % 8 = 3   -> bucket 3: [("apple", 3), ("banana", 5)]   <- collision, chained
hash("cherry") % 8 = 6   -> bucket 6: [("cherry", 7)]
```

Two different keys landing in the same bucket is a **collision** — the standard library's usual strategy (used by libstdc++/MSVC) is **separate chaining**: each bucket is a small linked list, and a collision just means the new pair gets appended to that bucket's list instead of causing any special handling.

### 3.2 How lookup actually works

```cpp
um.find("banana");
```

1. Compute `hash("banana")` — a `std::hash<string>` specialization turns the key into a `size_t`.
2. Reduce it to a bucket index: `hash_value % bucket_count`.
3. Jump straight to that bucket — O(1).
4. Scan that bucket's (short) list for a pair whose key equals `"banana"` — O(bucket size).

Step 3 is the O(1) part; step 4 is why the *average* case is O(1) but not the guaranteed worst case (§3.3).

### 3.3 Average O(1) vs worst-case O(n)

- **Average case O(1)**: when the hash function spreads keys evenly across buckets, each bucket holds only a handful of elements (ideally close to 1), so step 4 above is essentially constant work.
- **Worst case O(n)**: if many keys collide into the *same* bucket (a bad hash function, or an adversarial input crafted to collide, or too many elements crammed into too few buckets), that one bucket's list can grow to size n — and looking anything up in it degrades to a linear scan, exactly like `docs/recursion.md`-style worst-case reasoning applies here too: the complexity you quote should match the case you mean (see `docs/complexity.md` §7 on this exact pitfall).

This asymmetry — average O(1), worst O(n) — is the single most important thing to know about `unordered_map`, and is *why* competitive programmers sometimes deliberately avoid it against adversarial test data in favor of `map`'s guaranteed O(log n).

### 3.4 Load factor and rehashing

**Load factor** = `size() / bucket_count()` — roughly, the average number of elements per bucket. As more elements are inserted, load factor rises; once it crosses `max_load_factor()` (default 1.0), the table **rehashes**: it allocates a larger bucket array (roughly doubling) and reinserts every existing element into its new bucket (since `hash % bucket_count` changes when `bucket_count` changes).

Rehashing is O(n) when it happens, but it happens rarely enough (bucket count grows geometrically) that the **amortized** cost per insertion stays O(1) — the same amortized-doubling argument as `std::vector`'s `push_back`.

### 3.5 No ordering guarantee

Iteration order over an `unordered_map` is **unspecified** and can change after any insertion that triggers a rehash (since elements get redistributed into different buckets). Never write code that depends on `unordered_map` iteration order being stable or matching insertion order.

## 4. Side-by-side comparison

| | `map` | `unordered_map` |
|---|---|---|
| Underlying structure | Self-balancing BST (red-black tree) | Hash table (buckets + chaining) |
| `find`/`insert`/`erase` | O(log n), guaranteed | O(1) average, O(n) worst case |
| Iteration order | Sorted by key, always | Unspecified, can change on rehash |
| Extra ops | `lower_bound`, `upper_bound`, `equal_range` | None equivalent |
| Key requirement | `operator<` (or custom comparator) | `std::hash<Key>` + `operator==` (or custom hasher/equality) |
| Memory overhead | Tree node pointers (parent/left/right/color) | Bucket array + chain pointers |

## 5. Common pitfalls

- **`operator[]` silently inserts.** `m[key]` on a key that doesn't exist **creates it** with a default-constructed value (0 for `int`, `""` for `string`, etc.) instead of signaling "not found." If you only want to check existence, use `find` or `count` — not `operator[]`, which mutates the container as a side effect of a "read."

```cpp
if (m["missing"] == 0) { /* ... */ }   // BUG: this just inserted "missing" -> 0
if (m.find("missing") != m.end()) { /* ... */ }   // correct: doesn't insert
```

- **Assuming `unordered_map` is always O(1).** It's O(1) *average*; under adversarial input or a poor hash it degrades to O(n) (§3.3). If worst-case guarantees matter (e.g. competitive programming judges with adversarial tests, or a security-sensitive lookup), `map`'s O(log n) is safer than `unordered_map`'s optimistic-but-unguaranteed O(1).
- **Relying on `unordered_map` iteration order.** It's unspecified and can shift after any rehash-triggering insert — never treat it as stable or as insertion order.
- **Custom key types need extra work.** `map<MyType, V>` needs `MyType` to support `operator<` (or you supply a comparator). `unordered_map<MyType, V>` needs both a hash specialization (`std::hash<MyType>` or a custom hasher) *and* `operator==` — forgetting either is a compile error, not a silent bug, but it catches people off guard coming from `map`.
- **Iterator invalidation on erase.** Erasing a key from either container invalidates iterators/references to the erased element (and, for `unordered_map`, potentially others if it triggers implementation-specific bookkeeping) — don't hold onto an iterator across an `erase` call to a different key without confirming it's still valid; the safe pattern is `it = m.erase(it);` when erasing while iterating.

## 6. When to reach for which

- **Need sorted keys, range queries (`lower_bound`/`upper_bound`), or guaranteed worst-case performance** → `map`.
- **Need the fastest average lookup and don't care about order** → `unordered_map`.
- **Unsure and input isn't adversarial** → `unordered_map` is the common default in practice (faster in the typical case); reach for `map` when ordering or worst-case guarantees actually matter to the problem.

## 7. What's next

This is the associative-container half of STL coverage; `set`/`unordered_set` follow the exact same internal story (BST vs. hash table) minus the mapped value — a `set` is essentially a `map` where the "value" is the key itself. Practice problems using these containers will accumulate under `practice/hashing/` and future topic folders as they come up naturally in problem-solving, rather than as a dedicated STL practice folder.
