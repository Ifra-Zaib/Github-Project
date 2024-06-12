#pragma once
#ifndef SOCIALFEATURES_H
#define SOCIALFEATURES_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

class SocialFeature {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> followersMap;
    int graph[10][10] = { 1 };
    bool visited[10] = { 0 };

public:
    void load();
    void dfs(int u);
    void follow(int follower, int followee);
    void unfollow(int follower, int followee);
    void displayFollowers(int username);
};

#endif // SOCIALFEATURES_H
