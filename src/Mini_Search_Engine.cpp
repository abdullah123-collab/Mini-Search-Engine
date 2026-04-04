#include <iostream>
#include <cstring>
using namespace std;

#define SIZE 5
#define WORD_MAX 30
#define FILE_MAX 20
#define STACK_SIZE 20
#define QUEUE_SIZE 20

// ==================== LINKED LIST NODE ====================
// Each word may appear in multiple files, so we link them using FileNode.
struct FileNode {
    char fileName[FILE_MAX];
    FileNode* next;
    FileNode(const char* f) {
        strcpy(fileName, f);
        next = NULL;
    }
};

// ==================== TREE NODE ====================
// Each node of the BST stores a word and a linked list of file names.
struct TreeNode {
    char word[WORD_MAX];
    FileNode* files;
    TreeNode *left, *right;

    TreeNode(const char* w, const char* f) {
        strcpy(word, w);
        files = new FileNode(f);
        left = right = NULL;
    }
};

// ==================== BINARY SEARCH TREE ====================
// BST organizes words alphabetically and connects them to their files.
class BST {
public:
    TreeNode* root;
    BST() { root = NULL; }

    // Insert a word and its file into the BST
    TreeNode* insert(TreeNode* node, const char* word, const char* fileName) {
        if (node == NULL)
            return new TreeNode(word, fileName);

        int cmp = strcmp(word, node->word);
        if (cmp < 0)
            node->left = insert(node->left, word, fileName);
        else if (cmp > 0)
            node->right = insert(node->right, word, fileName);
        else {
        	// Word already exists, add new file only if not already linked
            FileNode* temp = node->files;
            while (temp->next != NULL) {
                if (strcmp(temp->fileName, fileName) == 0)
                    return node;  // file already exists for this word
                temp = temp->next;
            }
            if (strcmp(temp->fileName, fileName) != 0)
                temp->next = new FileNode(fileName);
        }
        return node;
    }
 
    // Display all words alphabetically with their file names
    void inorder(TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        cout<<node->word<<"  ";
        FileNode* temp = node->files;
        while (temp) {
            cout<<temp->fileName<<" ";
            temp = temp->next;
        }
        cout<<endl;
        inorder(node->right);
    }

    // Search a word in the BST and display the files it appears in
    void search(TreeNode* node, const char* query) {
        if (!node) {
            cout<<"Word not found."<<endl;
            return;
        }
        int cmp = strcmp(query, node->word);
        if (cmp == 0) {
            cout<<"'"<<query<<"' found in: ";
            FileNode* temp = node->files;
            while (temp) {
                cout<<temp->fileName<<" ";
                temp = temp->next;
            }
            cout<<endl;
        }
        else if (cmp < 0)
            search(node->left, query);
        else
            search(node->right, query);
    }
};

// ==================== QUEUE CLASS ====================
// Stores search queries in FIFO order.
class Queue {
    char data[QUEUE_SIZE][WORD_MAX];
    int front, rear;
public:
    Queue() { front = -1; rear = -1; }

    void enqueue(const char* q) {
        if (rear == QUEUE_SIZE - 1) return;
        if (front == -1) front = 0;
        strcpy(data[++rear], q);
    }

    char* dequeue() {
        if (front == -1 || front > rear)
            return NULL;
        return data[front++];
    }

    bool isEmpty() {
        return (front == -1 || front > rear);
    }
};

// ==================== STACK CLASS ====================
// Stores search queries in FIFO order.
class Stack {
    char data[STACK_SIZE][WORD_MAX];
    int top;
public:
    Stack() { top = -1; }

    void push(const char* x) {
        if (top == STACK_SIZE - 1) return;
        strcpy(data[++top], x);
    }

        void showHistory() {
        cout<<"\nSearch History:\n";
        if (top == -1) {
            cout<<"No word search (empty)"<<endl;
            return;
        }
        for (int i = top; i >= 0; i--)
            cout<<data[i]<<endl;
    }
};

// ==================== HELPER FUNCTIONS ====================
// Convert string to lowercase (for case-insensitive search)
void toLower(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

// Split document text into words and insert into BST
void splitAndInsert(BST &bst, const char* text) {
    char fileName[FILE_MAX];
    char line[200];
    strcpy(line, text);

    int i = 0, j = 0;
    
    // Extract file name before ':'
    while (line[i] != ':' && line[i] != '\0') {
        fileName[j++] = line[i++];
    }
    fileName[j] = '\0';
    i++;

    // Split remaining part into words
    char word[WORD_MAX];
    j = 0;
    while (line[i] != '\0') {
        if (line[i] == ' ') {
            if (j > 0) {
                word[j] = '\0';
                toLower(word);
                bst.root = bst.insert(bst.root, word, fileName);
                j = 0;
            }
        } else {
            word[j++] = line[i];
        }
        i++;
    }
    if (j > 0) {
        word[j] = '\0';
        toLower(word);
        bst.root = bst.insert(bst.root, word, fileName);
    }
}

// ==================== MAIN FUNCTION ====================
int main() {
	// Predefined dataset (simulating 5 text documents)
    const char documents[SIZE][200] = {
        "file1.txt: artificial intelligence and machine learning",
        "file2.txt: data structures and algorithms",
        "file3.txt: deep learning uses neural networks",
        "file4.txt: queue stack and linked list in dsa",
        "file5.txt: binary tree and search engine project"
    };

    BST bst;
    Queue q;
    Stack history;

    cout<<"\n\t***Mini Search Engine***"<<endl;
    cout<<endl;

    // Build index from all documents
    cout<<"Building search index..."<<endl;
    for (int i = 0; i < SIZE; i++)
        splitAndInsert(bst, documents[i]);
    cout<<"Index built successfully!"<<endl;

    int choice;
    char query[WORD_MAX];
    
    // Main menu loop
    do {
        cout<<"\n========== Home =========="<<endl;
        cout<<"1. Search Word"<<endl;
        cout<<"2. Show All Words"<<endl;
        cout<<"3. Show History"<<endl;
        cout<<"4. Exit"<<endl;
        cout<<"=========================="<<endl;
        cout<<"Enter choice: ";
        cin>>choice;

        switch (choice) {
        case 1:
            cout<<"Enter word to search: ";
            cin>>query;
            toLower(query);
            q.enqueue(query);
            history.push(query);
            bst.search(bst.root, query);
            break;

        case 2:
            cout<<"All Indexed Words:"<<endl;
            bst.inorder(bst.root);
            break;

        case 3:
            history.showHistory();
            break;

        case 4:
            cout<<"Exiting mini search engine..."<<endl;
            break;

        default:
            cout<<"Invalid choice! Try again."<<endl;
        }

    } while (choice != 4);
}

