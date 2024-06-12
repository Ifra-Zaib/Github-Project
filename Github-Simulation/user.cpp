#include "user.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
Hashtable::Hashtable()
{
    for (int i = 0; i < size; i++) {
        arr[i] = nullptr;
    }
}
int Hashtable::hashIndex(const string& key) {
    int sum = 0;
    for (char c : key) {
        sum += c;
    }
    return sum % size;
}
void UserAccounts::load(UserAccounts& user)
{
    ifstream file("user_accounts.csv");
    if (!file.is_open())
    {
        
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string username, password, email;
        vector<string> userData; // Create a vector to store the data for each row
        while (getline(ss, username, ',') &&
            getline(ss, password, ',') &&
            getline(ss, email, ','))
        {
            userData.push_back(username); // Store username
            userData.push_back(password); // Store password
            userData.push_back(email);    // Store email
        }
        if (!userData.empty()) { // Ensure data is not empty
            vector<vector<string>> data = { userData }; // Create 2D vector with single row data
            user.hashtable.insert(data); // Insert data into the hashtable
        }
    }
    file.close();
}

void Hashtable::insert(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        if (row.size() >= 3) { // Ensure row has at least 3 elements (username, password, email)
            const string& username = row[0];
            const string& password = row[1];
            const string& email = row[2];
            int index = hashIndex(username);
            Node* newNode = new Node(username, password, email);

            if (!arr[index]) {
                // If the slot is empty, insert the new node directly
                arr[index] = newNode;
            }
            else {
                // If the slot is not empty, append the new node to the end of the linked list
                Node* temp = arr[index];
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }
}

void Hashtable::insertForfiles(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        if (row.size() >= 3) { // Ensure row has at least 3 elements (username, password, email)
            const string& username = row[0];
            const string& password = row[1];
            const string& email = row[2];
            int index = hashIndex(username);
            Node* newNode = new Node(username, password, email);

            if (!arr[index]) {
                arr[index] = newNode;
            }
            else {
                Node* temp = arr[index];
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }
}
bool Hashtable::search(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        if (row.size() >= 2) { // Ensure row has at least 2 elements (username, password)
            const string& username = row[0];
            const string& password = row[1];
            int index = hashIndex(username);

            readFromFile("user_accounts.csv"); // Call readFromFile function before searching

            Node* current = arr[index];
            while (current) {
                if (current->username == username && current->password == password) {
                    return true;
                }
                current = current->next;
            }
        }
    }
    return false;
}

void Hashtable::display() {
    for (int i = 0; i < size; i++) {
       

        Node* current = arr[i];
        while (current)
        {
            
            current = current->next;
        }
        
    }
}
void Hashtable::writeToFile(const vector<vector<string>>& data) {
    ofstream outFile("user_accounts.csv", ios::app); // Open file in append mode
    if (outFile.is_open()) {
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outFile << row[i];
                // Add a comma if it's not the last element in the row
                if (i < row.size() - 1) {
                    outFile << ",";
                }
            }
            outFile << endl; // End the current row
        }
        outFile.close();
        cout << "Data appended to file successfully." << endl; // Debug output
    }
    else {
        cerr << "Unable to open file: user_accounts.csv" << endl;
    }
}





void Hashtable::readFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string username, password, email;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, email, ',');
        vector<vector<string>> data = { {username, password, email} };//we use 2D vector bcz CSV is a excel sheet
        insert(data);
    }
    inFile.close();
}

void UserAccounts::registration(const string& username, const string& password, const string& email) {

    vector<vector<string>> data = { {username, password, email} };
    if (hashtable.search(data))
    {
        cout << "Already exist this account" << endl;
        return;
    }
    hashtable.insert(data);
    saveToDisk(data);
}
bool Hashtable::searchInHash(string name) {
    int index = hashIndex(name);
    if (arr[index]) {
        Node* current = arr[index];
        while (current) {
            if (current->username == name) {
                cout << "User found!" << endl;
                cout << "Username: " << current->username << endl;
                cout << "Password: " << current->password << endl;
                cout << "Email: " << current->email << endl;
                return true;
            }
            current = current->next;
        }
    }
    cout << "User not found." << endl;
    return false;
}






bool UserAccounts::login(const string& username, const string& password) {
    vector<vector<string>> data = { {username, password} };
    if (hashtable.search(data)) {
        cout << "Login successful!" << endl;
        return true;
    }
    else {
        cout << "Invalid username and password" << endl;
        return false;
    }
}


void UserAccounts::logout() {
    cout << "Logout successfully" << endl;
}

void UserAccounts::saveToDisk(vector<vector<string>> data) {
    hashtable.writeToFile(data);
}

void UserAccounts::loadFromDisk(const string& filename) {
    hashtable.readFromFile(filename + ".csv");
}
