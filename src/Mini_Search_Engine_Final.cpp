#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <ctime>

using namespace std;

#define WORD_MAX 50
#define FILE_MAX 50
#define LINE_MAX 500
#define STACK_SIZE 100
#define MAX_FILES 50

// ==================== LINKED LIST NODE ====================
struct FileNode {
    char fileName[FILE_MAX];
    int frequency;
    int lineNumbers[100];  // Store line numbers where word appears
    int lineCount;
    FileNode* next;
    
    FileNode(const char* f, int lineNum) {
        strcpy(fileName, f);
        frequency = 1;
        lineNumbers[0] = lineNum;
        lineCount = 1;
        next = NULL;
    }
    
    void addLine(int lineNum) {
        // Check if this line number already exists
        for (int i = 0; i < lineCount; i++) {
            if (lineNumbers[i] == lineNum) {
                return;  // Line already recorded
            }
        }
        
        if (lineCount < 100) {
            lineNumbers[lineCount++] = lineNum;
        }
    }
};

// ==================== AVL TREE NODE ====================
// Using AVL Tree for balanced search (more efficient than BST)
struct AVLNode {
    char word[WORD_MAX];
    FileNode* files;
    AVLNode *left, *right;
    int height;
    int totalOccurrences;

    AVLNode(const char* w, const char* f, int lineNum) {
        strcpy(word, w);
        files = new FileNode(f, lineNum);
        left = right = NULL;
        height = 1;
        totalOccurrences = 1;
    }
};

// ==================== AVL TREE CLASS ====================
class AVLTree {
private:
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }
    
    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        
        return x;
    }
    
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        
        return y;
    }
    
public:
    AVLNode* root;
    int totalWords;
    
    AVLTree() { 
        root = NULL; 
        totalWords = 0;
    }
    
    AVLNode* insert(AVLNode* node, const char* word, const char* fileName, int lineNum) {
        // Standard BST insertion
        if (node == NULL) {
            totalWords++;
            return new AVLNode(word, fileName, lineNum);
        }
        
        int cmp = strcmp(word, node->word);
        
        if (cmp < 0)
            node->left = insert(node->left, word, fileName, lineNum);
        else if (cmp > 0)
            node->right = insert(node->right, word, fileName, lineNum);
        else {
            // Word exists, update file list
            node->totalOccurrences++;
            FileNode* temp = node->files;
            
            while (temp != NULL) {
                if (strcmp(temp->fileName, fileName) == 0) {
                    temp->frequency++;
                    temp->addLine(lineNum);
                    return node;
                }
                if (temp->next == NULL) break;
                temp = temp->next;
            }
            
            temp->next = new FileNode(fileName, lineNum);
            return node;
        }
        
        // Update height
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        
        // Get balance factor
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && strcmp(word, node->left->word) < 0)
            return rightRotate(node);
        
        // Right Right Case
        if (balance < -1 && strcmp(word, node->right->word) > 0)
            return leftRotate(node);
        
        // Left Right Case
        if (balance > 1 && strcmp(word, node->left->word) > 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Left Case
        if (balance < -1 && strcmp(word, node->right->word) < 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    AVLNode* search(AVLNode* node, const char* query) {
        if (!node) return NULL;
        
        int cmp = strcmp(query, node->word);
        
        if (cmp == 0)
            return node;
        else if (cmp < 0)
            return search(node->left, query);
        else
            return search(node->right, query);
    }
    
    void inorder(AVLNode* node) {
        if (!node) return;
        inorder(node->left);
        
        cout << "Word: " << node->word 
             << " (Total: " << node->totalOccurrences << ") -> ";
        
        FileNode* temp = node->files;
        while (temp) {
            cout << temp->fileName << "(" << temp->frequency << ") ";
            temp = temp->next;
        }
        cout << endl;
        
        inorder(node->right);
    }
    
    int countNodes(AVLNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
    
    int height(AVLNode* node) {
        return getHeight(node);
    }
    
    void deleteTree(AVLNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        
        FileNode* temp = node->files;
        while (temp) {
            FileNode* del = temp;
            temp = temp->next;
            delete del;
        }
        delete node;
    }
    
    ~AVLTree() {
        deleteTree(root);
    }
};

// ==================== STACK FOR HISTORY ====================
struct HistoryNode {
    char query[WORD_MAX];
    char timestamp[30];
    bool found;
    int resultCount;
    HistoryNode* next;
    
    HistoryNode(const char* q, bool f, int count) {
        strcpy(query, q);
        found = f;
        resultCount = count;
        next = NULL;
        
        // Get current timestamp
        time_t now = time(0);
        char* dt = ctime(&now);
        strcpy(timestamp, dt);
        timestamp[strlen(timestamp) - 1] = '\0';  // Remove newline
    }
};

class HistoryStack {
private:
    HistoryNode* top;
    int size;
    
public:
    HistoryStack() {
        top = NULL;
        size = 0;
    }
    
    void push(const char* query, bool found, int resultCount) {
        HistoryNode* newNode = new HistoryNode(query, found, resultCount);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    void display() {
        cout << "\n====== Search History ======" << endl;
        if (!top) {
            cout << "No search history." << endl;
            return;
        }
        
        HistoryNode* temp = top;
        int count = 1;
        while (temp) {
            cout << count++ << ". \"" << temp->query << "\" - "
                 << (temp->found ? "FOUND" : "NOT FOUND")
                 << " (" << temp->resultCount << " results)" << endl;
            cout << "   Time: " << temp->timestamp << endl;
            temp = temp->next;
        }
        cout << "============================" << endl;
    }
    
    void saveToFile(const char* filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Could not save history to file!" << endl;
            return;
        }
        
        file << "========== SEARCH HISTORY ==========\n";
        file << "Total Searches: " << size << "\n";
        file << "====================================\n\n";
        
        HistoryNode* temp = top;
        int count = 1;
        while (temp) {
            file << count++ << ". Query: \"" << temp->query << "\"\n";
            file << "   Status: " << (temp->found ? "FOUND" : "NOT FOUND") << "\n";
            file << "   Results: " << temp->resultCount << "\n";
            file << "   Timestamp: " << temp->timestamp << "\n";
            file << "   ------------------------------------\n";
            temp = temp->next;
        }
        
        file.close();
        cout << "History saved to '" << filename << "'" << endl;
    }
    
    void clear() {
        while (top) {
            HistoryNode* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }
    
    ~HistoryStack() {
        clear();
    }
};

// ==================== QUEUE FOR BATCH PROCESSING ====================
struct QueryNode {
    char query[WORD_MAX];
    QueryNode* next;
    
    QueryNode(const char* q) {
        strcpy(query, q);
        next = NULL;
    }
};

class QueryQueue {
private:
    QueryNode* front;
    QueryNode* rear;
    int count;
    
public:
    QueryQueue() {
        front = rear = NULL;
        count = 0;
    }
    
    void enqueue(const char* query) {
        QueryNode* newNode = new QueryNode(query);
        
        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    bool dequeue(char* result) {
        if (front == NULL) return false;
        
        strcpy(result, front->query);
        QueryNode* temp = front;
        front = front->next;
        
        if (front == NULL)
            rear = NULL;
        
        delete temp;
        count--;
        return true;
    }
    
    bool isEmpty() {
        return front == NULL;
    }
    
    int size() {
        return count;
    }
    
    void display() {
        if (!front) {
            cout << "Queue is empty." << endl;
            return;
        }
        
        cout << "\n=== Query Queue ===" << endl;
        QueryNode* temp = front;
        int num = 1;
        while (temp) {
            cout << num++ << ". " << temp->query << endl;
            temp = temp->next;
        }
        cout << "===================" << endl;
    }
    
    ~QueryQueue() {
        while (front) {
            QueryNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

// ==================== FILE MANAGER ====================
class FileManager {
private:
    char fileList[MAX_FILES][FILE_MAX];
    int fileCount;
    
public:
    FileManager() {
        fileCount = 0;
    }
    
    bool isFileIndexed(const char* filename) {
        for (int i = 0; i < fileCount; i++) {
            if (strcmp(fileList[i], filename) == 0) {
                return true;  // File already indexed
            }
        }
        return false;
    }
    
    bool addFile(const char* filename) {
        // Check if file is already indexed
        if (isFileIndexed(filename)) {
            cout << "Warning: '" << filename << "' is already indexed. Skipping..." << endl;
            return false;
        }
        
        if (fileCount < MAX_FILES) {
            strcpy(fileList[fileCount++], filename);
            return true;
        }
        
        cout << "Error: Maximum file limit reached!" << endl;
        return false;
    }
    
    void displayFiles() {
        cout << "\n=== Indexed Files ===" << endl;
        if (fileCount == 0) {
            cout << "No files indexed yet." << endl;
        } else {
            for (int i = 0; i < fileCount; i++) {
                cout << i + 1 << ". " << fileList[i] << endl;
            }
            cout << "Total: " << fileCount << " files" << endl;
        }
        cout << "====================" << endl;
    }
    
    int getFileCount() {
        return fileCount;
    }
};

// ==================== HELPER FUNCTIONS ====================
void toLower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void removePunctuation(char* str) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isalnum(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Simple stemming: Remove common suffixes
void stemWord(char* word) {
    int len = strlen(word);
    
    // Remove "ing" (searching → search)
    if (len > 5 && strcmp(word + len - 3, "ing") == 0) {
        word[len - 3] = '\0';
        return;
    }
    
    // Remove "ed" (searched → search)
    if (len > 4 && strcmp(word + len - 2, "ed") == 0) {
        word[len - 2] = '\0';
        return;
    }
    
    // Remove "s" (trees → tree, algorithms → algorithm)
    if (len > 3 && word[len - 1] == 's' && word[len - 2] != 's') {
        word[len - 1] = '\0';
        return;
    }
}

bool isStopWord(const char* word) {
    const char* stopWords[] = {"a", "an", "the", "is", "are", "was", "were", 
                               "in", "on", "at", "to", "for", "of", "and", "or"};
    int stopWordCount = 15;
    
    for (int i = 0; i < stopWordCount; i++) {
        if (strcmp(word, stopWords[i]) == 0)
            return true;
    }
    return false;
}

void indexFile(AVLTree &tree, const char* filename, FileManager &fm) {
    // Check if file is already indexed
    if (fm.isFileIndexed(filename)) {
        cout << "File '" << filename << "' is already indexed!" << endl;
        return;
    }
    
    ifstream file(filename);
    
    if (!file) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }
    
    fm.addFile(filename);
    
    char line[LINE_MAX];
    int lineNum = 0;
    
    while (file.getline(line, LINE_MAX)) {
        lineNum++;
        char word[WORD_MAX];
        int j = 0;
        
        for (int i = 0; i <= strlen(line); i++) {
            if (line[i] == ' ' || line[i] == '\0' || line[i] == ',' || 
                line[i] == '.' || line[i] == '!' || line[i] == '?') {
                if (j > 0) {
                    word[j] = '\0';
                    removePunctuation(word);
                    toLower(word);
                    stemWord(word);  // NEW: Apply stemming
                    
                    if (strlen(word) > 2 && !isStopWord(word)) {
                        tree.root = tree.insert(tree.root, word, filename, lineNum);
                    }
                    j = 0;
                }
            } else {
                if (j < WORD_MAX - 1)
                    word[j++] = line[i];
            }
        }
    }
    
    file.close();
    cout << " Indexed: " << filename << " (" << lineNum << " lines)" << endl;
}

void displaySearchResult(AVLNode* result) {
    if (!result) {
        cout << "\nWord not found in any document." << endl;
        return;
    }
    
    cout << "\n========== SEARCH RESULTS ==========" << endl;
    cout << "Word: '" << result->word << "'" << endl;
    cout << "Total occurrences: " << result->totalOccurrences << endl;
    cout << "\nFound in:" << endl;
    
    FileNode* temp = result->files;
    int fileNum = 1;
    while (temp) {
        cout << "\n  " << fileNum++ << ". File: " << temp->fileName << endl;
        cout << "     Frequency: " << temp->frequency << " times" << endl;
        cout << "     Lines: ";
        for (int i = 0; i < temp->lineCount && i < 10; i++) {
            cout << temp->lineNumbers[i];
            if (i < temp->lineCount - 1 && i < 9) cout << ", ";
        }
        if (temp->lineCount > 10)
            cout << "... (+" << (temp->lineCount - 10) << " more)";
        cout << endl;
        
        temp = temp->next;
    }
    cout << "====================================" << endl;
}

void createSampleFiles() {
    // Create sample file 1
    ofstream file1("document1.txt");
    file1 << "Artificial intelligence and machine learning are transforming technology.\n";
    file1 << "Deep learning uses neural networks to process complex data.\n";
    file1 << "Machine learning algorithms learn from data and improve over time.\n";
    file1.close();
    
    // Create sample file 2
    ofstream file2("document2.txt");
    file2 << "Data structures are fundamental concepts in computer science.\n";
    file2 << "Common data structures include arrays, linked lists, stacks, and queues.\n";
    file2 << "Trees and graphs are advanced data structures used in algorithms.\n";
    file2.close();
    
    // Create sample file 3
    ofstream file3("document3.txt");
    file3 << "Binary search trees organize data for efficient searching.\n";
    file3 << "AVL trees are self-balancing binary search trees.\n";
    file3 << "Search algorithms help find specific data in large datasets.\n";
    file3.close();
    
    cout << "Sample files created: document1.txt, document2.txt, document3.txt" << endl;
}

// ==================== MAIN FUNCTION ====================
int main() {
    AVLTree tree;
    HistoryStack history;
    QueryQueue queryQueue;
    FileManager fileManager;
    
    cout<<"\n||===================================||" << endl;
    cout << "||    MINI SEARCH ENGINE v2.0        ||" << endl;
    cout << "||    File-Based Search System       ||" << endl;
    cout << "||===================================||" << endl;
    
    int choice;
    char filename[FILE_MAX];
    char query[WORD_MAX];
    
    do {
        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. Create Sample Files" << endl;
        cout << "2. Index a File" << endl;
        cout << "3. Index Multiple Files" << endl;
        cout << "4. Search Word" << endl;
        cout << "5. Batch Search (Queue)" << endl;
        cout << "6. Show All Indexed Words" << endl;
        cout << "7. Show Search History" << endl;
        cout << "8. Save History to File" << endl;
        cout << "9. Show Indexed Files" << endl;
        cout << "10. Show Statistics" << endl;
        cout << "11. Exit" << endl;
        cout << "===============================" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                createSampleFiles();
                break;
                
            case 2:
                cout << "Enter filename to index: ";
                cin.getline(filename, FILE_MAX);
                indexFile(tree, filename, fileManager);
                break;
                
            case 3: {
                int numFiles;
                cout << "How many files to index? ";
                cin >> numFiles;
                cin.ignore();
                
                for (int i = 0; i < numFiles; i++) {
                    cout << "File " << (i+1) << ": ";
                    cin.getline(filename, FILE_MAX);
                    indexFile(tree, filename, fileManager);
                }
                break;
            }
                
            case 4:
                cout << "Enter word to search: ";
                cin.getline(query, WORD_MAX);
                toLower(query);
                stemWord(query);  // NEW: Stem the search query too
                {
                    AVLNode* result = tree.search(tree.root, query);
                    displaySearchResult(result);
                    
                    int count = 0;
                    if (result) {
                        FileNode* temp = result->files;
                        while (temp) {
                            count++;
                            temp = temp->next;
                        }
                    }
                    history.push(query, result != NULL, count);
                }
                break;
                
            case 5: {
                int numQueries;
                cout << "How many queries to add? ";
                cin >> numQueries;
                cin.ignore();
                
                for (int i = 0; i < numQueries; i++) {
                    cout << "Query " << (i+1) << ": ";
                    cin.getline(query, WORD_MAX);
                    toLower(query);
                    stemWord(query);  // NEW: Stem batch queries
                    queryQueue.enqueue(query);
                }
                
                cout << "\nProcessing batch queries..." << endl;
                char batchQuery[WORD_MAX];
                int processed = 0;
                
                while (queryQueue.dequeue(batchQuery)) {
                    processed++;
                    cout << "\n[Query " << processed << ": " << batchQuery << "]" << endl;
                    
                    AVLNode* result = tree.search(tree.root, batchQuery);
                    displaySearchResult(result);
                    
                    int count = 0;
                    if (result) {
                        FileNode* temp = result->files;
                        while (temp) {
                            count++;
                            temp = temp->next;
                        }
                    }
                    history.push(batchQuery, result != NULL, count);
                }
                cout << "\nProcessed " << processed << " queries." << endl;
                break;
            }
                
            case 6:
                cout << "\n====== All Indexed Words ======" << endl;
                tree.inorder(tree.root);
                cout << "===============================" << endl;
                break;
                
            case 7:
                history.display();
                break;
                
            case 8:
                history.saveToFile("search_history.txt");
                break;
                
            case 9:
                fileManager.displayFiles();
                break;
                
            case 10:
                cout << "\n======= STATISTICS =======" << endl;
                cout << "Unique words: " << tree.countNodes(tree.root) << endl;
                cout << "AVL tree height: " << tree.height(tree.root) << endl;
                cout << "Indexed files: " << fileManager.getFileCount() << endl;
                cout << "==========================" << endl;
                break;
                
            case 11:
                cout << "\nThank you for using Mini Search Engine!" << endl;
                break;
                
            default:
                cout << "Invalid choice!" << endl;
        }
        
    } while (choice != 11);
    
    return 0;
}
