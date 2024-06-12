#ifndef USERACCOUNTS_H
#define USERACCOUNTS_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include<Windows.h>
using namespace std;
class Node
{
public:
    string username;
    string password;
    string email;
    Node* next;
    Node(const string& u_name, const string& pass, const string& e_mail)
    {
        username = u_name;
        password = pass; email = e_mail; next = nullptr;

    }
};
class Hashtable
{
private:
    static const int size = 20;
    Node* arr[size];

public:
    Hashtable();
    int hashIndex(const string& key);
    void insert(const vector<vector<string>>& data);
    bool search(const vector<vector<string>>& data);
    void display();
    void writeToFile(const vector<vector<string>>& d);
    void readFromFile(const string& filename);
    void insertForfiles(const vector<vector<string>>& data);
    bool searchInHash(string name);

};
class Edge
{
public:
    string data;
    Edge* next;

    Edge(string data)
    {
        this->data = data;
        this->next = nullptr;
    }
};

class Graph
{
public:
    unordered_map<string, Edge*> graph;
    void add_edge(string user, string follower)
    {
        if (graph.find(user) == graph.end())
        {
            graph[user] = new Edge(user);
        }

        Edge* new_follower = new Edge(follower);
        Edge* current = graph[user];
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_follower;
    }
    Edge* get_followers(const string& user)
    {
        if (graph.find(user) != graph.end())
        {
            return graph[user]->next;
        }
        return nullptr;
    }
    void remove_edge(string unfollowUser, string follower) 
    {
        if (graph.find(follower) != graph.end())
        {
            Edge* prev = nullptr;
            Edge* current = graph[follower];
            
            while (current && current->data != unfollowUser)
            {
                prev = current;
                current = current->next;
            }
            if (current)
            {
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    graph[follower] = current->next;
                }
                delete current;
            }
        }
    }
    void printAdjacencyListFromFile(const string& user, const string& filename)
    {
        ifstream inFile(filename);
        if (!inFile.is_open())
        {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        string line;
        bool userFound = false;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string username;
            getline(ss, username, '_'); 

            if (username == user) 
            {
                cout << user << " -> ";
                string follower;
                while (getline(ss, follower, '_')) 
                {
                    cout << follower << " -> ";
                }
                cout << "nullptr" << endl;
                userFound = true;
                break;
            }
        }

        inFile.close();

        if (!userFound)
        {
            cout << "User not found." << endl;
        }
    }



};
class UserAccounts
{
public:
    Hashtable hashtable;
    Graph socialNetwork;
    void registration(const string& username, const string& password, const string& email);
    bool login(const string& username, const string& password);
    void logout();
    void saveToDisk(vector<vector<string>> data);
    void loadFromDisk(const string& filename);
    void load(UserAccounts& user);
   
    
    void follow(string followUser, string loggedInUser)
    {
        socialNetwork.add_edge(followUser, loggedInUser);
        cout << loggedInUser << " is now following " << followUser << endl;
        writeFollowersToFile(loggedInUser, followUser);

    }
    void writeFollowersToFile(const string& loggedInUser, string follower) 
    {
        ifstream inFile("followers.csv");
        ofstream outFile("followers_temp.csv");

        if (!inFile.is_open() || !outFile.is_open())
        {
            cerr << "Unable to open file: followers.csv" << endl;
            return;
        }

        bool userFound = false;

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string username;
            getline(ss, username, '_'); 

            if (username == loggedInUser) 
            {
                outFile << line << "_" << follower; 

                Edge* current = socialNetwork.get_followers(loggedInUser);
                while (current)
                {
                    outFile << current->data << "_" << follower;
                    current = current->next;
                }

                outFile << endl;
                userFound = true;
            }
            else
            {
                outFile << line << endl; 
            }
        }
        if (!userFound)
        {
            outFile << loggedInUser << "_" << follower;

            Edge* current = socialNetwork.get_followers(loggedInUser);
            while (current) 
            {
                outFile << current->data << ",";
                current = current->next;
            }

            outFile << endl;
        }
        inFile.close();
        outFile.close();
        remove("followers.csv");
        rename("followers_temp.csv", "followers.csv");

        cout << "Followers data written to file successfully." << endl;
    }
    void unfollow(string unfollowUser, string loggedInUser)
    {
        socialNetwork.remove_edge(unfollowUser, loggedInUser);
        cout << loggedInUser << " has unfollowed " << unfollowUser << endl;
        removeFollowerFromFile(loggedInUser, unfollowUser);
    }

    void removeFollowerFromFile(const string& loggedInUser, const string& unfollowUser)
    {
        ifstream inFile("followers.csv");
        ofstream outFile("followers_temp.csv");

        if (!inFile.is_open() || !outFile.is_open())
        {
            cerr << "Unable to open file: followers.csv" << endl;
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string username;
            getline(ss, username, '_');

           
            if (username == loggedInUser) 
            {
                outFile << username << "_"; 

                string follower;
                bool first = true; 
                while (getline(ss, follower, '_'))
                {
                    if (follower != unfollowUser) 
                    {
                        if (!first)
                        {
                            outFile << "_"; 
                        }
                        outFile << follower;
                        first = false;
                    }
                }
                outFile << endl;
            }
            else
            {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();
        remove("followers.csv");
        rename("followers_temp.csv", "followers.csv");

        cout << "Follower removed from file successfully." << endl;
    }
    void showFollowingList(const string& username)
    {

        ifstream inFile("followers.csv");
        if (!inFile.is_open())
        {
            cerr << "Unable to open file: followers.csv" << endl;
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string user;
            getline(ss, user, '_'); 

            // If the line corresponds to the requested username
            if (user == username) {
                cout << "Following list for " << username << ":" << endl;
                string follower;
                bool first = true; // Flag to handle the first follower
                while (getline(ss, follower, '_')) {
                    if (!first) {
                        cout << ", "; // Separate followers with a comma and space
                    }
                    cout << follower;
                    first = false;
                }
                cout << endl;
                inFile.close();
                return; // Stop searching after finding the requested user's list
            }
        }

        inFile.close();
        // If the requested username is not found in the file
        cout << "User not found or has no followers." << endl;
    }


    void loadFromFileToGraph(Graph& list) {
        ifstream inFile("followers.csv");
        if (!inFile.is_open()) 
        {
            
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string user;
            getline(ss, user, '_'); // Get the username from each line


            Edge* current = list.graph[user]; // Access the graph's data structure correctly
            if (!current) {
                list.graph[user] = new Edge(user);
                current = list.graph[user];
            }

            string follower;
            while (getline(ss, follower, '_')) {

                current->next = new Edge(follower);
                current = current->next;
            }
        }

        inFile.close();
    }


};

#endif // USERACCOUNTS_H
