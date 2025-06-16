#pragma once
#include <unordered_map>
#include <unordered_set>
class BigCellGraph {
public:
    std::unordered_map<int, std::unordered_set<int>> adj;
    void addEdge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
    }
    const std::unordered_set<int>& neighbors(int u) const {
        static const std::unordered_set<int> empty;
        auto it = adj.find(u);
        if (it != adj.end()) return it->second;
        return empty;
    }
};