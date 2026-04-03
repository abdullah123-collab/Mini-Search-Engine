# 📖 DSA Concepts Used — Mini Search Engine

This document explains each data structure used in the project and **why** it was chosen.

---

## 1. 🌳 Binary Search Tree (BST)

### What it does here:
- Every unique word extracted from the documents is stored as a **node** in the BST.
- Words are arranged **alphabetically** (left = smaller, right = larger using `strcmp`).

### Why BST?
- Searching a word takes **O(log n)** on average — much faster than a linear scan.
- Inorder traversal gives all words in **sorted (alphabetical) order** automatically.

### Key operations:
| Operation | Description |
|---|---|
| `insert()` | Adds a new word or links a new file to an existing word |
| `search()` | Finds a word and prints all files it appears in |
| `inorder()` | Prints all words alphabetically |

---

## 2. 🔗 Linked List (FileNode)

### What it does here:
- Each BST node (word) has a **linked list of file names** attached to it.
- If a word appears in multiple files, each file is a new node in the list.

### Why Linked List?
- A word can appear in **any number of files** — dynamic size fits perfectly.
- Easy to append new file names as new documents are indexed.

---

## 3. 📦 Stack

### What it does here:
- Every time the user searches a word, it gets **pushed** onto the stack.
- `showHistory()` prints all searched words from **most recent to oldest** (LIFO).

### Why Stack?
- Search history naturally follows **Last In, First Out** — most recent query shown first.
- Implemented using a fixed-size char array with a `top` pointer.

---

## 4. 🔁 Queue

### What it does here:
- Each search query is also **enqueued** into the Queue.
- Represents the order in which searches were made (FIFO).

### Why Queue?
- Useful for processing queries in the **order they were received**.
- First In, First Out — matches real search engine query processing.

---

## 5. 🔡 String Parsing (splitAndInsert)

### What it does here:
- Takes a raw document string like `"file1.txt: machine learning"`
- Extracts the **file name** (before `:`) and **splits words** by spaces
- Each word is lowercased and inserted into the BST

### Concepts used:
- Character arrays (`char[]`)
- Manual string traversal (no `strtok` or STL)
- `toLower()` for case-insensitive search

---

## 🔄 Overall Flow

```
Documents (raw strings)
        ↓
  splitAndInsert()
        ↓
   toLower(word)
        ↓
  BST.insert(word, fileName)
        ↓
  FileNode linked list per word
        ↓
  User searches → BST.search()
                → Stack.push() [history]
                → Queue.enqueue() [query log]
```
