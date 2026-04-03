# 🔍 Mini Search Engine — DSA Project (C++)

A console-based **Mini Search Engine** built in C++ using core **Data Structures & Algorithms** concepts. It indexes words from multiple documents and allows fast keyword lookup using a Binary Search Tree, with search history tracked via a Stack and query management via a Queue.

---

## 📌 Features

- 📂 **Multi-document indexing** — indexes words across 5 simulated text files
- 🌳 **BST-based word index** — fast O(log n) word search
- 🔗 **Linked List per word** — tracks all files a word appears in
- 📋 **Search History** — stores previous queries using a Stack (LIFO)
- 🔁 **Query Queue** — manages search queries in FIFO order
- 🔡 **Case-insensitive search** — all words normalized to lowercase
- 💡 **No STL used** — all data structures implemented from scratch

---

## 🧠 Data Structures Used

| Structure | Role |
|---|---|
| Binary Search Tree (BST) | Stores and searches indexed words alphabetically |
| Linked List (FileNode) | Links each word to the files it appears in |
| Stack | Maintains search history (LIFO) |
| Queue | Manages search queries (FIFO) |

---

## 📁 Project Structure

```
Mini-Search-Engine/
│
├── src/
│   ├── Mini_Search_Engine_Final.cpp   # Main source file (latest version)
│   └── Mini_Search_Engine.cpp         # Earlier version (reference)
│
├── sample_data/
│   └── documents.txt                  # Sample documents used for indexing
│
├── docs/
│   └── DSA_Concepts.md                # Explanation of data structures used
│
├── .gitignore
└── README.md
```

---

## ▶️ How to Run

### Using g++ (MinGW / Linux / Mac)

```bash
g++ src/Mini_Search_Engine_Final.cpp -o search_engine
./search_engine
```

### Using an IDE
- Open `Mini_Search_Engine_Final.cpp` in **Code::Blocks**, **Dev-C++**, or **VS Code**
- Compile and Run (F9 in Code::Blocks)

---

## 🖥️ Menu Options

```
========== Home ==========
1. Search Word
2. Show All Words
3. Show History
4. Exit
==========================
```

- **Search Word** — Enter a keyword; engine returns which file(s) contain it
- **Show All Words** — Displays all indexed words in alphabetical order (BST inorder)
- **Show History** — Displays all previously searched words (Stack)

---

## 💻 Sample Run

```
        ***Mini Search Engine***

Building search index...
Index built successfully!

========== Home ==========
1. Search Word
...
Enter choice: 1
Enter word to search: learning
'learning' found in: file1.txt file3.txt
```

---

## 📚 Predefined Document Dataset

| File | Content |
|---|---|
| file1.txt | artificial intelligence and machine learning |
| file2.txt | data structures and algorithms |
| file3.txt | deep learning uses neural networks |
| file4.txt | queue stack and linked list in dsa |
| file5.txt | binary tree and search engine project |

---

## 🛠️ Tech Stack

- **Language:** C++
- **Paradigm:** Object-Oriented Programming (OOP)
- **Compiler:** g++ / MinGW
- **IDE:** Code::Blocks / Dev-C++

---

## 👤 Author

**Abdullah** — BSCS Student, SZABIST University  

---

## 📄 License

This project is for academic purposes. Feel free to reference it for learning.
