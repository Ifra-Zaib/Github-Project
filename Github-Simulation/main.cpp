#include <iostream>
#include <string>
#include "user.h"
#include "repos.h"
#include "follow_un.h"
#include<Windows.h>
using namespace std;
int convertStringToInt(const string& str) {
    stringstream ss(str);
    int result;
    ss >> result;
    return result;
}
void main_menu()
{// Ignore the newline character in the buffer
    // Create instances of UserAccounts and RepositoryTree
    UserAccounts userAccounts;
    RepositoryTree tree;
    RepositoryTree* root;;
    SocialFeature obj1;
    Graph g;
    // Load user accounts and display them
    userAccounts.load(userAccounts);
    userAccounts.hashtable.display();
    tree.readFromRpoFile(tree);
    tree.LoadFromFile(tree);
    tree.loadFromForksCSV(tree);
    tree.LoadfromBraches(tree);
    tree.LoadFromBranchFiles(tree);
    userAccounts.loadFromFileToGraph(g);

    system("color F1");
    int c;
    cout << "Enter your choice: " << endl;
    cout << "1-Registration" << endl;
    cout << "2-Login" << endl;
    cout << "3-Logout" << endl;
    cout << "4-View Profile" << endl;
    cout << "5-Exit" << endl;
    cin >> c;
    cin.ignore(); 


    if (c == 1)
    {
        system("cls");
        system("color 03");
        string username, password, email;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter email: ";
        getline(cin, email);
        userAccounts.registration(username, password, email);
        cout << "Registration successful!" << endl;
    }
    if (c == 2)
    {
        system("cls");
        system("color  F9");
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        if (userAccounts.login(username, password))
        {
            cout << "Login successful!" << endl;
            string repo_ch;
            cout << "Do you want to Create Repository for your Accout? (yes/no):";
            getline(cin, repo_ch);
            /*string followChoice;
            cout << "Do you want to follow a user? (yes/no): ";
            getline(cin, followChoice);*/
            if (repo_ch == "yes")
            {

                tree.createRepo(username);

                int choice;
                do {
                    //User logged in, now allow repository actions
                   // RepositoryTree repo;// Create repository with owner's username

                    cout << "-------------------Menu----------------------------" << endl;
                    cout << "1-Create Repository" << endl;
                    cout << "2-Add File to Repository" << endl;
                    cout << "3-Delete file from repository" << endl;
                    cout << "4-Fork" << endl;
                    cout << "5-Forks count display" << endl;
                    cout << "6-Branches display" << endl;
                    cout << "7-Branch files display" << endl;
                    cout << "8-View repository" << endl;
                    cout << "9-Delete Repository" << endl;
                    cout << "10-Follow Others" << endl;
                    cout << "11- Un Following" << endl;
                    cout << "12-Following list" << endl;
                    cout << "13-Go to main menue" << endl;
                    cout << "14-Exit" << endl;
                    //add commit and us mein check kro pehla us na fork kia hoa ya naai bphir wo add commit kr sakta
                    cin >> choice;
                    cin.ignore();
                    switch (choice)
                    {
                    case 1:
                    {
                        tree.createRepo(username);
                        break;
                    }
                    case 2:
                    {
                        tree.displayTree(tree.root);
                        tree.addFileRepo(username);
                        break;
                    }
                    case 3:
                    {
                        tree.displayTree(tree.root);
                        tree.deleteFileRepo(username);
                        break;
                    }

                    case 4: {
                        tree.displayTree(tree.root);
                        int repoIDFork;
                        cout << "Enter the ID of the repository you want to fork: ";
                        cin >> repoIDFork;

                        // Check if the repoIDFork exists among the repositories owned by the current user
                        RepoNode* userRepo = tree.searchRepoByID(repoIDFork, tree.root);
                        if (userRepo != nullptr && userRepo->ownername == username) {
                            cout << "You cannot fork your own repository." << endl;
                        }
                        else if (userRepo != nullptr) {
                            // User can fork this repository
                            tree.makeBranch(repoIDFork, username);
                            userRepo->forkcount++;
                            // Open forks.csv for reading and writing
                            ifstream forksFile("forks.csv");
                            if (forksFile.is_open()) {
                                ofstream tempFile("temp.csv");
                                if (tempFile.is_open()) {
                                    bool repoFound = false;
                                    string line;
                                    while (getline(forksFile, line)) {
                                        stringstream ss(line);
                                        string id, count;
                                        getline(ss, id, ',');
                                        getline(ss, count);

                                        if (convertStringToInt(id) == repoIDFork) {
                                            // Update the fork count for this repoIDFork
                                            int newCount = stoi(count) + 1;
                                            line = id + "," + to_string(newCount);
                                            repoFound = true;
                                        }
                                        tempFile << line << endl;
                                    }
                                    forksFile.close();

                                    if (!repoFound) {
                                        // Repository ID not found, add a new entry
                                        tempFile << repoIDFork << ",1" << endl;
                                    }

                                    tempFile.close();
                                    // Replace forks.csv with temp.csv
                                    if (remove("forks.csv") != 0) {
                                        cerr << "Error: Unable to remove forks.csv." << endl;
                                    }
                                    else if (rename("temp.csv", "forks.csv") != 0) {
                                        cerr << "Error: Unable to rename temp.csv to forks.csv." << endl;
                                    }

                                }
                                else {
                                    cerr << "Error: Unable to create temp.csv." << endl;
                                }
                            }
                            else {
                                cerr << "Error: Unable to open forks.csv." << endl;
                            }
                        }
                        else {
                            cout << "Repository not found." << endl;
                        }
                        break;


                    }
                    case 5:
                    {
                        tree.displayTree(tree.root);
                        int id;
                        cout << "Enter id of repoid for which forkcount you want to see: ";
                        cin >> id;
                        RepoNode* repo = tree.searchRepoByID(id, tree.root);
                        if (repo != nullptr) {
                            cout << "RepoID: " << id << " fork count: " << repo->forkcount << endl;
                        }
                        else {
                            cout << "Repository with ID " << id << " not found." << endl;
                        }
                        break;
                    }
                    case 6:
                    {
                        tree.displayTree(tree.root);
                        int id;
                        cout << "Enter id of repoid fo which you want to see branches: ";
                        cin >> id;
                        RepoNode* repo = tree.searchRepoByID(id, tree.root);
                        bool found = false;
                        cout << "Branchees for repo ID:" << id << endl;
                        BranchNode* curr = repo->branches.head;
                        while (curr != NULL)
                        {
                            cout << curr->branchName << endl;
                            curr = curr->next;
                            found = true;
                        }
                        if (found == false)
                        {
                            cout << "This repo id have no branches" << endl;
                        }
                        break;
                    }
                    case 7:
                    {
                        tree.displayTree(tree.root);
                        int id;
                        cout << "Enter ID of the repository for which you want to see branch files: ";
                        cin >> id;
                        cin.ignore(); // Consume newline character in the input buffer

                        string branchName;
                        cout << "Enter branch name of this repository: ";
                        getline(cin, branchName);

                        RepoNode* repo = tree.searchRepoByID(id, tree.root);
                        if (repo != nullptr) {
                            BranchNode* currentBranch = repo->branches.head;
                            while (currentBranch != nullptr) {
                                if (currentBranch->branchName == branchName) {
                                    cout << "Branch found: " << branchName << endl;
                                    FileNode* currentFile = currentBranch->branchfiles.head;
                                    if (currentFile != nullptr) {
                                        cout << "Files in branch " << branchName << " of repository ID " << id << ":" << endl;
                                        while (currentFile != nullptr) {
                                            cout << currentFile->filename << endl;
                                            currentFile = currentFile->next;
                                        }
                                    }
                                    else {
                                        cout << "No files found in branch " << branchName << endl; // Debug statement
                                    }
                                    break; // Exit the loop after finding the specified branch
                                }
                                currentBranch = currentBranch->next;
                            }
                            if (currentBranch == nullptr) {
                                cout << "Branch " << branchName << " not found in repository with ID " << id << endl; // Debug statement
                            }
                        }
                        else {
                            cout << "Repository with ID " << id << " not found." << endl;
                        }
                        break;
                    }
                    case 8:
                    {
                        int id;
                        cout << "Enter repository ID whose repo You want to see: ";
                        cin >> id;
                        RepoNode* repo = tree.searchRepoByID(id, tree.root);
                        if (repo == NULL)
                        {
                            cout << "ID not found" << endl;
                            break;
                        }
                        tree.viewRepository(repo, username);
                        break;
                    }
                    case 9:
                    {
                        int id;
                        cout << "Enter id for delete repository: " << endl;
                        cin >> id;
                        RepoNode* repo = tree.searchRepoByID(id, tree.root);
                        if (repo == NULL)
                        {
                            cout << "Repo id does not exist" << endl;

                        }
                        else {
                            tree.deleteRepository(repo, tree.root, tree.root);
                            cout << " After deleting repo:" << endl;
                            tree.displayTree(tree.root);
                        }

                        break;
                    }
                    case 10:
                    {
                        string userToFollow;
                        cout << "Enter username of the user you want to follow: ";
                        getline(cin, userToFollow);

                        if (userAccounts.hashtable.searchInHash(userToFollow))
                        {
                            userAccounts.follow(userToFollow, username);
                        }
                    }
                    case 11:
                    {
                        string unfollowChoice;
                        cout << "Do you want to unfollow a user? (yes/no): ";
                        getline(cin, unfollowChoice);
                        if (unfollowChoice == "yes")
                        {
                            string userToUnfollow;
                            cout << "Enter username of the user you want to unfollow: ";
                            getline(cin, userToUnfollow);

                            if (userAccounts.hashtable.searchInHash(userToUnfollow))
                            {
                                userAccounts.unfollow(userToUnfollow, username);
                            }
                        }
                    }
                    case 12:
                    {
                        /*userAccounts.showFollowingList(username);*/
                        int user;
                        cout << "Enter user id you want to see:";
                        cin >> user;
                        obj1.follow(2, 3); 
                        obj1.follow(2, 3);
                        obj1.follow(2, 3);
                        obj1.follow(2, 3);
                        obj1.follow(2, 3);
                        obj1.displayFollowers(user);
                        break;
                    }
                    case 13:
                    {
                        cout << "Going to main menu..." << endl;
                        main_menu();
                    }
                    case 14:
                    {
                        cout << "Exiting...." << endl;
                        exit(0);
                    }
                    }
                } while (choice != 14);

            }
            else if (repo_ch == "no")
            {
                cout << "Go to main menue" << endl;
                main_menu();
            }


        }

    }
    if (c == 3)
    {
        userAccounts.logout();
    }
    if (c == 4)
    {
        tree.display_inorder(tree.root);
        /*int id;
        cout << "Enter repository ID whose repo You want to see: ";
        cin >> id;
        RepoNode* repo = tree.searchRepoByID(id, tree.root);
        if (repo == NULL)
        {
            cout << "ID not found" << endl;
            
        }
        tree.viewProfile(id);*/
    }
    if (c == 5)
    {
        cout << "Exiting......" << endl;
        exit(0);
    }
}
int main()
{
    main_menu();
    return 0;
}
