#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct FileNode {
    string filename;
    string FileContent;
    FileNode* next;
    FileNode(string name, string content);
};

struct FileLL {
    FileNode* head;

public:
    FileLL() : head(nullptr) {}
    ~FileLL() {
        clear();
    }
    string getfilename()
    {
        return head->filename;
    }
    void insert(string name, string content) {
        FileNode* newNode = new FileNode(name, content);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            FileNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

    }
    void printFileNames() {
        FileNode* current = head;
        while (current != nullptr) {
            cout << current->filename << endl;
            current = current->next;
        }
    }
    void remove(string filename) {
        FileNode* current = head;
        FileNode* prev = nullptr;
        // Traverse the list to find the node with the given filename
        while (current != nullptr && current->filename != filename) {
            prev = current;
            current = current->next;
        }
        // If the node is found, remove it from the list
        if (current != nullptr) {
            if (prev == nullptr) {
                // If the node to be removed is the head
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
        }
    }
    void clear() {
        FileNode* current = head;
        while (current != nullptr) {
            FileNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }
};
struct BranchNode {
    string branchName;
    string branchowner;
    FileLL branchfiles;
    bool branchmode;
    FileNode* branchreadme;
    BranchNode* next;
    BranchNode(string BO, string BN, bool m, FileNode* read)
    {
        branchName = BN;
        branchowner = BO;
        branchmode = m;
        branchreadme = read;
        branchfiles = FileLL();
        next = NULL;
    }
};
class BranchLL {


public:
    BranchNode* head;
    BranchLL() : head(nullptr) {}

    // Function to insert a new branch node at the end of the linked list
    void insert(const string& branchOwner, const string& branchName, bool mode, FileNode* readme) {
        BranchNode* newNode = new BranchNode(branchOwner, branchName, mode, readme);
        newNode->branchfiles = FileLL(); // Initialize the FileLL for the new node

        if (head == nullptr) {
            head = newNode;
        }
        else {
            BranchNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Function to delete a branch node with the given branch name
    void deleteBranch(const string& branchName) {
        if (head == nullptr) {
            return; // Empty list, nothing to delete
        }

        BranchNode* current = head;
        BranchNode* prev = nullptr;

        // Traverse the list to find the node with the given branch name
        while (current != nullptr && current->branchName != branchName) {
            prev = current;
            current = current->next;
        }

        // If the node is found, delete it
        if (current != nullptr) {
            if (prev == nullptr) {
                // If the node to be removed is the head
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
        }
    }
};

struct RepoNode {
    string ownername;
    string RepoName;
    int repoID;
    static int lastRepoID;
    bool mode;
    int forkcount;
    FileNode* readme;
    FileLL files;
    BranchLL branches;
    RepoNode* left;
    RepoNode* right;
    RepoNode(string reponame, string Oname, bool m, FileNode* readme);
};

class RepositoryTree {
    void insertRepoInBST(RepoNode*& root, RepoNode* newRepo);
    int convertStringToInt(const string& str);
    int count = 0;
public:
    RepoNode* root;

    RepositoryTree();
    bool iscommit();
    RepoNode* searchRepoByID(int id, RepoNode* root);
    void display_inorder(RepoNode* root);
    void createRepo(string ownername);
    void displayTree(RepoNode* root);
    void removeEntryFromFileCSV(int repoID, const string& filename);
    void addFileRepo(const string& loggedInUser);
    void deleteFileRepo(string& loggedInUser);
    void makeBranch(int id, string loggedInUser);
    void addcommit(int id, BranchNode* branch);
    void viewProfile(const int& id);
    void viewRepository(RepoNode* repo, string loggedinUser);
    void deleteRepository(RepoNode*& repo, RepoNode*& parent, RepoNode* root);
    RepoNode* findMin(RepoNode* node);
    void LoadFromFile(RepositoryTree& tree);
    void WriteInRepoFile(int ID, string ownername, string reponame, bool m, string read);
    void readFromRpoFile(RepositoryTree& tree);
    void writeInFiles(int repoID, const string& filename, string ownername);
    void writeInBranches(int id, BranchNode* branch);
    void LoadfromBraches(RepositoryTree& tree);
    void loadFromForksCSV(RepositoryTree& tree);
    void writeInBranchFiles(int repoid, string filename, string ownername, string branchname);
    void LoadFromBranchFiles(RepositoryTree& tree);
    void deleteBranchFromRepo(const string& branchname, RepoNode*& repo);
    void deleteBranchFiles(const string& branchname);
    void deleteRepoFromFile(int repoID);
};
