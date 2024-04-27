#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>

#include "../src/graph.h"

using namespace std;

int main() {
    cout << boolalpha;

    cout << "---------------------- GRAPH A ----------------------\n";
    Graph<string, int> G1;
    cout << "Initial Size of Graph: " << G1.size() << '\n';

    cout << "\n";
    cout << "\n";

    cout << "Adding Edges\n";
    G1.add_edge("A", "B", 5);
    G1.add_edge("B", "C", 10);
    G1.add_edge("C", "D", 3);
    G1.add_edge("D", "A", 5);
    G1.add_edge("A", "B", 100);
    G1.add_edge("B", "C", 150);
    G1.add_edge("A", "C", 5);

    cout << "\n";

    cout << "Displaying the graph\n";
    cout << "Size: " << G1.size() << '\n';
    G1.display();

    cout << "\n";

    cout << "Modifying Edges\n";
    G1.modify_edge("A", "B", 6);
    G1.modify_edge("B", "C", 150);

    cout << "\n";

    cout << "Displaying the graph\n";
    cout << "Size: " << G1.size() << '\n';
    G1.display();

    cout << "\n";

    cout << "Deleting Edges\n";
    G1.delete_edge("B", "C");

    cout << "\n";

    cout << "Displaying the graph\n";
    cout << "Size: " << G1.size() << '\n';
    G1.display();

    cout << "\n";

    Graph<string, int>::breadth_first_search_iterator bfs1(G1, G1.find("A"));
    cout << "Breadth First Search Traversal (starting with node A): ";
    while (!bfs1.bfsend()) {
        cout << (*bfs1).first << ", ";
        ++bfs1;
    }

    cout << "\n";

    Graph<string, int>::depth_first_search_iterator dfs1(G1, G1.find("C"));
    cout << "Depth First Search Traversal (starting with node C): ";
    while (!dfs1.dfsend()) {
        cout << (*dfs1).first << ", ";
        ++dfs1;
    }

    cout << "\n";

    cout << "\nVertices\tShortest Path Cost (from A)\tShortestPath\n";
    map<string, pair<string, int>> m1 = G1.dijkstra("A");
    string str;
    stack<string> st1;
    for (auto x : m1) {
        cout << x.first << "\t\t" << x.second.second << "\t\t\t";
        str = x.first;
        while (str != "A") {
            st1.push(str);
            str = m1[str].first;
        }
        st1.push("A");
        while (!st1.empty()) {
            cout << "->" << st1.top();
            st1.pop();
        }
        cout << "\n";
    }
}