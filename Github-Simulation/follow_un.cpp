#include "follow_un.h"
#include <fstream>
#include <iostream>
#include<queue>
#include<stack>
using namespace std;

void SocialFeature::load()
{
    std::ifstream file("user_accounts.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open user_accounts.csv" << std::endl;
        return;
    }

    std::string username, password, email;
    while (std::getline(file, username, ',') && std::getline(file, password, ',') && std::getline(file, email))
    {
        followersMap[username].insert(password); 
        int follower = stoi(password);
        int user = stoi(username);
        graph[user][follower] = 1;
    }
    file.close();
}

void SocialFeature::follow(int follower, int followee)
{
    /*followersMap[followee].insert(follower);
    std::ofstream followerFile("followers.csv");
    if (followerFile.is_open()) {
        followerFile << follower << std::endl;
        std::cout << "Follower added: " << follower << " follows " << followee << std::endl; 
        followerFile.close(); 
    }
    else {
        std::cerr << "Failed to open " << followee << "_followers.txt" << std::endl;
    }*/
    graph[follower][followee] = 1;
}

void SocialFeature::unfollow(int follower, int followee) {
   /* auto it = followersMap.find(followee);
    if (it != followersMap.end()) {
        it->second.erase(follower); 
        std::ofstream followerFile("un_followers.csv");
        if (followerFile.is_open()) {
            for (const auto& f : it->second) {
                followerFile << f << std::endl;
            }
            followerFile.close();
        }
        else {
            std::cerr << "Failed to open file for writing follower data: " << followee + "_followers.csv" << std::endl;
        }
    }
    else {
        std::cerr << followee << " does not exist or has no followers" << std::endl;
    }*/
    graph[follower][followee] = 1;
}
void SocialFeature::dfs(int user)
{
    
    visited[user] = 1;
    for (int i = 0;i < 10;i++)
    {
        if (graph[user][i] && !visited[i])
        {
            cout << i << endl;
            dfs(i);
        }

    }
}
void SocialFeature::displayFollowers(int username)
{
    queue<int>q;
   
    q.push(username);
    visited[username] = 1; 
    cout << "Fllowers list:" << endl;
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        for (int i = 0;i < 10;i++)
        {
            if (graph[current][i] && !visited[i])
            {
                cout << i << endl;
                visited[i] = true;
                q.push(i);
            }
        }
    }
    cout << "With dfs:" << endl;
    dfs(username);
    /*std::ifstream followerFile("dispaly.csv");
    if (followerFile.is_open()) {
        std::cout << "Followers of " << username << ":" << std::endl;
        std::string follower;
        while (std::getline(followerFile, follower)) {
            std::cout << follower << std::endl;
        }
        followerFile.close();
    }
    else {
        std::cerr << "Failed to open " << username << "_followers.txt" << std::endl;
    }*/
}
