#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Use standard DSU Implimentation from geeksforgeeks.org (Kruskal’s Minimum Spanning Tree (MST) Algorithm)
class DSU 
{
    std::vector<int> parent, rank;

public:
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n);
        
        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
            rank[i] = 1;
        }
    }

    int find(int i)
    {
        return (parent[i] == i) ? i : (parent[i] = find(parent[i]));
    }

    void unite(int x, int y)
    {
        int s1 = find(x), s2 = find(y);
        if (s1 != s2)
        {
            if (rank[s1] < rank[s2]) parent[s1] = s2;
            else if (rank[s1] > rank[s2]) parent[s2] = s1;
            else parent[s2] = s1, rank[s1]++;
        }
    }
};

bool comparator(std::vector<int> &a, std::vector<int> &b)
{
   return a[2] < b[2]; 
}

int kruskalsMST(int V, std::vector<std::vector<int>> &edges)
{
    
    // Sort all edges
    sort(edges.begin(), edges.end(),comparator);
    
    // Traverse edges in sorted order
    DSU dsu(V);
    int cost = 0, count = 0;
    
    for (auto &e : edges)
    {
        int x = e[0], y = e[1], w = e[2];
        
        // Make sure that there is no cycle
        if (dsu.find(x) != dsu.find(y))
        {
            dsu.unite(x, y);
            cost += w;
            if (++count == V - 1) break;
        }
    }
    return cost;
}

// Helper function
static inline int decodeCost(char c) 
{
    if ('A' <= c && c <= 'Z') return c - 'A';   // Capitals
    return 26 + (c - 'a');                      // Lowercase
}

static std::vector<std::string> splitByComma(const std::string& s) // Split the inputs by comma
{
    std::vector<std::string> parts; parts.reserve(32);
    std::string cur;
    for (char ch : s) {
        if (ch == ',') { parts.push_back(cur); cur.clear(); }
        else cur.push_back(ch);
    }
    parts.push_back(cur);
    return parts;
}

int main() {
    
    // Get user inputs
    std::string scountry, sbuild, sdestroy;
    if (!(std::cin >> scountry >> sbuild >> sdestroy)) return 0;

    // Create matrices by split inputs
    std::vector<std::string> C = splitByComma(scountry);
    std::vector<std::string> B = splitByComma(sbuild);
    std::vector<std::string> D = splitByComma(sdestroy);
    int N = (int)C.size();

    // We will destroy all roads and then rebuild using the MST
    int baseline = 0; 
    std::vector<std::vector<int>> edges;
    edges.reserve((int)N * (N - 1) / 2);

    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            bool exists = (C[i][j] == '1');
            int b = decodeCost(B[i][j]);
            int d = decodeCost(D[i][j]);
            if (exists) baseline += d;
            int w = exists ? -d : +b;
            edges.push_back({i, j, w}); // Each edge has an adjusted weight
        }
    }

    int mst_sum = kruskalsMST(N, edges);

    int answer = baseline + mst_sum;
    std::cout << answer << '\n';

    return 0;
}