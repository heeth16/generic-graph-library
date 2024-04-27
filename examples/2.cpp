#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>

#include "../src/graph.h"

using namespace std;

int main() {
    cout << boolalpha;

    cout << "---------------------- Graph B ----------------------\n";

    Graph<int, int> G2;
    cout << "Initial Size of Graph: " << G2.size() << '\n';

    cout << "\n";
    cout << "\n";

    cout << "Adding Edges\n";
    G2.add_edge(0, 2);
    G2.add_edge(0, 4);
    G2.add_edge(1, 5, 5);
    G2.add_edge(1, 6, 10);
    G2.add_edge(2, 5, 5);
    G2.add_edge(2, 6, 5);
    G2.add_edge(3, 5, 25);
    G2.add_edge(5, 6, 25);
    G2.add_edge(5, 7, 35);

    cout << "\n";

    cout << "Displaying the graph\n";
    cout << "Size: " << G2.size() << '\n';
    G2.display();

    cout << "\n";

    cout << "Modifying Edges\n";
    G2.modify_edge(1, 2, 6);
    G2.modify_edge(3, 4, 0);
    cout << "\n";

    cout << "Displaying the graph\n";
    cout << "Size: " << G2.size() << '\n';
    G2.display();

    cout << "\n";

    cout << "Deleting Edges\n";
    G2.delete_edge(3, 4);
    G2.delete_edge(6, 2);

    cout << "Displaying the graph\n";
    cout << "Size: " << G2.size() << '\n';
    G2.display();

    cout << "\n";

    Graph<int, int>::breadth_first_search_iterator bfs2(G2, G2.find(1));
    cout << "Breadth First Search Traversal (starting with node 1): ";
    while (!bfs2.bfsend()) {
        cout << (*bfs2).first << ", ";
        ++bfs2;
    }

    cout << "\n";

    Graph<int, int>::depth_first_search_iterator dfs2(G2, G2.find(5));
    cout << "Depth First Search Traversal (starting with node 5): ";
    while (!dfs2.dfsend()) {
        cout << (*dfs2).first << ", ";
        ++dfs2;
    }

    cout << "\n";

    cout << "\nVertices\tShortest Path Cost (from 5)\tShortestPath\n";
    map<int, pair<int, int>> m2 = G2.dijkstra(5);
    int s;
    stack<int> st2;
    for (auto x : m2) {
        cout << x.first << "\t\t" << x.second.second << "\t\t\t";
        s = x.first;
        while (s != 5) {
            st2.push(s);
            s = m2[s].first;
        }
        st2.push(5);
        while (!st2.empty()) {
            cout << "->" << st2.top();
            st2.pop();
        }
        cout << endl;
    }

    // FIND MAX ELEMENT
    int max = INT32_MIN;
    pair<int, int> tmp;
    Graph<int, int>::breadth_first_search_iterator bfsmax(G2, G2.find(0));
    int src, dest;

    while (!bfsmax.bfsend()) {
        tmp = *max_element(bfsmax->second.begin(), bfsmax->second.end(),
                           [](pair<int, int> p1, pair<int, int> p2) { return p1.second < p2.second; });
        if (max < tmp.second) {
            max = tmp.second;
            src = bfsmax->first;
            dest = tmp.first;
        }
        ++bfsmax;
    }
    cout << "MAX EDGE is between " << src << " and " << dest << " with weight " << max << '\n';

    // ACCUMULATE
    Graph<int, int>::breadth_first_search_iterator bfsacc1(G2, G2.find(0));
    Graph<int, int>::breadth_first_search_iterator bfsacc2(G2, G2.find(0));
    while (!bfsacc2.bfsend())
        ++bfsacc2;

    int ele = accumulate(bfsacc1, bfsacc2, 0, [](int x, auto y) { return x + y.first; });
    cout << "ACCUMULATE SUM OF NODES : " << ele << '\n';

    // CYCLE CHECK
    Graph<int, int>::breadth_first_search_iterator bfscyc(G2, G2.find(0));
    unordered_map<int, bool> visited;
    int c;
    bool flag = false;
    while (!bfscyc.bfsend()) {
        c = 0;
        visited[bfscyc->first] = true;
        auto setitr = bfscyc->second.begin();
        while (setitr != bfscyc->second.end()) {
            if (visited[setitr->first] == true)
                ++c;
            else
                visited[setitr->first] = true;
            ++setitr;
        }
        if (c > 1) {
            flag = true;
            break;
        }
        ++bfscyc;
    }
    cout << "CYCLE PRESENT : " << flag << '\n';
}