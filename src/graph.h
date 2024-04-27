#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#define INF INT32_MAX

template <typename vertex, typename edge>
struct uniquePair {
    bool operator()(const std::pair<vertex, edge> &lhs, const std::pair<vertex, edge> &rhs) const {
        return lhs.first < rhs.first;
    }
};

template <typename vertex, typename edge>
struct weightedOrder {
    bool operator()(const std::pair<vertex, edge> &lhs, const std::pair<vertex, edge> &rhs) const {
        return (lhs.second < rhs.second) || (!(lhs.second < rhs.second) && (lhs.first < rhs.first));
    }
};

template <typename vertex = int, // Graph::vertex_type
          typename edge = double // Graph::edge_type
          >
class Graph {
private:
    // Map key stores the vertex
    // Map value is a set of connected vertices with corresponding edge weights
    std::unordered_map<vertex, std::set<std::pair<vertex, edge>, uniquePair<vertex, edge>>> network;

    // An iterator to value of network map
    typedef typename std::set<std::pair<vertex, edge>, uniquePair<vertex, edge>>::iterator set_iterator;

    // isConnected checks if 2 vertices are connected by one or more edges
    set_iterator isConnected(const vertex &, const vertex &);

public:
    typedef vertex vertex_type;
    typedef edge edge_type;
    typedef std::pair<const vertex, std::set<std::pair<vertex, edge>, uniquePair<vertex, edge>>> list_type;
    typedef typename std::unordered_map<vertex, std::set<std::pair<vertex, edge>, uniquePair<vertex, edge>>>::iterator iterator;
    typedef size_t size_type;

    void add_edge(const vertex &, const vertex &, const edge = edge_type());
    void delete_edge(const vertex, const vertex);
    void modify_edge(const vertex, const vertex, const edge = edge_type());
    typename Graph<vertex, edge>::iterator find(const vertex);
    std::map<vertex, std::pair<vertex, edge>> dijkstra(const vertex &);
    Graph<vertex, edge>::size_type size() const;
    bool empty() const;

    iterator begin();
    iterator end();
    void display();

    class breadth_first_search_iterator {
    private:
        std::unordered_map<vertex, bool> visited;
        std::queue<Graph<vertex, edge>::iterator> q;
        typename Graph<vertex, edge>::iterator it;
        Graph<vertex, edge> &obj;

    public:
        breadth_first_search_iterator(Graph<vertex, edge> &, Graph<vertex, edge>::iterator);

        breadth_first_search_iterator &operator++();
        breadth_first_search_iterator operator++(int);
        Graph<vertex, edge>::list_type &operator*();
        typename Graph<vertex, edge>::iterator operator->();
        bool operator==(const breadth_first_search_iterator &) const;
        bool operator!=(const breadth_first_search_iterator &) const;
        bool bfsend();
    };

    class depth_first_search_iterator {
    private:
        std::unordered_map<vertex, bool> visited;
        std::stack<Graph<vertex, edge>::iterator> st;
        typename Graph<vertex, edge>::iterator it;
        Graph<vertex, edge> &obj;

    public:
        depth_first_search_iterator(Graph<vertex, edge> &, Graph<vertex, edge>::iterator);

        depth_first_search_iterator &operator++();
        depth_first_search_iterator operator++(int);
        Graph<vertex, edge>::list_type &operator*();
        typename Graph<vertex, edge>::iterator operator->();
        bool operator==(const depth_first_search_iterator &rhs) const;
        bool operator!=(const depth_first_search_iterator &rhs) const;
        bool dfsend();
    };
};

/**
 * Checks if two nodes in the graph are connected.
 *
 * @param node1 The first node to check.
 * @param node2 The second node to check.
 *
 * @return An iterator pointing to the edge connecting the two nodes, or the end iterator if no such edge exists.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::set_iterator Graph<vertex, edge>::isConnected(const vertex &node1, const vertex &node2) {
    Graph<vertex, edge>::set_iterator b = network[node1].begin();
    Graph<vertex, edge>::set_iterator e = network[node1].end();
    while (b != e) {
        if (b->first == node2)
            break;
        ++b;
    }
    return b;
}

/**
 * Adds an edge between two nodes in the graph with the given weight.
 *
 * @param node1 The first node of the edge.
 * @param node2 The second node of the edge.
 * @param weight The weight of the edge.
 *
 * @return void
 */
template <typename vertex, typename edge>
void Graph<vertex, edge>::add_edge(const vertex &node1, const vertex &node2, const edge weight) {
    std::cout << "Adding vertex: <" << node1 << "," << node2 << "," << weight << ">" << '\n';
    network[node1].insert(std::make_pair(node2, weight));
    network[node2].insert(std::make_pair(node1, weight));
}

/**
 * Deletes an edge between two nodes in the graph.
 *
 * @param node1 The first node of the edge to be deleted.
 * @param node2 The second node of the edge to be deleted.
 *
 * @return void
 */
template <typename vertex, typename edge>
void Graph<vertex, edge>::delete_edge(vertex node1, vertex node2) {
    typename Graph<vertex, edge>::set_iterator it1;

    // Find the set iterator at which node1 is connected to node2
    it1 = isConnected(node1, node2);
    if (it1 != network[node1].end()) {
        network[node1].erase(it1);
    }

    // Find the set iterator at which node2 is connected to node1
    it1 = isConnected(node2, node1);
    if (it1 != network[node2].end()) {
        network[node2].erase(it1);
    }
}

/**
 * Modifies an existing edge between two nodes in the graph by deleting the current edge and adding a new edge with updated weight.
 *
 * @param node1 The first node of the existing edge to be modified.
 * @param node2 The second node of the existing edge to be modified.
 * @param new_weight The new weight for the updated edge.
 *
 * @return void
 */
template <typename vertex, typename edge>
void Graph<vertex, edge>::modify_edge(const vertex node1, const vertex node2, const edge new_weight) {
    delete_edge(node1, node2);
    add_edge(node1, node2, new_weight);
}

/**
 * Finds the iterator pointing to the given node in the graph.
 *
 * @param node The node to find in the graph.
 *
 * @return An iterator pointing to the node in the graph or the end iterator if no such node exists.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::iterator Graph<vertex, edge>::find(const vertex node) {
    return network.find(node);
}

/**
 * Finds the shortest path from the given source vertex to all other vertices in the graph using Dijkstra's algorithm.
 *
 * @param src The source vertex from which to find the shortest path.
 *
 * @return A map of vertices to pairs containing the previous vertex and the weight of the shortest path.
 */
template <typename vertex, typename edge>
std::map<vertex, std::pair<vertex, edge>> Graph<vertex, edge>::dijkstra(const vertex &src) {
    std::set<std::pair<vertex, edge>, weightedOrder<vertex, edge>> setds;
    std::map<vertex, std::pair<vertex, edge>> path;

    for (Graph<vertex, edge>::list_type x : network)
        path[x.first].second = INF;

    setds.insert(std::make_pair(src, edge()));
    path[src] = std::make_pair(src, edge());

    std::pair<vertex, edge> tmp;
    vertex u, v;
    edge weight;

    while (!setds.empty()) {
        tmp = *(setds.begin());
        setds.erase(setds.begin());
        u = tmp.first;

        for (typename Graph<vertex, edge>::set_iterator i = network[u].begin(); i != network[u].end(); ++i) {
            v = (*i).first;
            weight = (*i).second;

            if (path[v].second > path[u].second + weight) {
                if (path[v].second != INF)
                    setds.erase(setds.find(std::make_pair(v, path[v].second)));

                path[v].second = path[u].second + weight;
                path[v].first = u;
                setds.insert(std::make_pair(v, path[v].second));
            }
        }
    }
    return path;
}

/**
 * Returns the number of vertices in the graph.
 *
 * @return The size of the graph, i.e., the number of vertices.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::size_type Graph<vertex, edge>::size() const {
    return network.size();
}

/**
 * Checks if the graph is empty.
 *
 * @return true if the graph is empty, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::empty() const {
    return network.empty();
}

/**
 * Returns an iterator pointing to the beginning of the graph.
 *
 * @return An iterator pointing to the beginning of the graph.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::iterator Graph<vertex, edge>::begin() {
    return network.begin();
}

/**
 * Returns an iterator pointing to the past-the-end element in the graph.
 *
 * @return An iterator pointing to the past-the-end element in the graph.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::iterator Graph<vertex, edge>::end() {
    return network.end();
}

/**
 * Displays the contents of the graph.
 *
 * @return void
 */
template <typename vertex, typename edge>
void Graph<vertex, edge>::display() {
    for (Graph<vertex, edge>::list_type x : network) {
        std::cout << x.first << " : ";
        for (std::pair<vertex, edge> y : x.second) {
            std::cout << '<' << y.first << ',' << y.second << '>' << ", ";
        }
        std::cout << "\n";
    }
}

/**
 * Constructor for the breadth first search iterator.
 *
 * @param obj_ The graph object reference.
 * @param it_ The iterator to the start node.
 *
 * @return None
 */
template <typename vertex, typename edge>
Graph<vertex, edge>::breadth_first_search_iterator::breadth_first_search_iterator(Graph<vertex, edge> &obj_, Graph<vertex, edge>::iterator it_)
    : obj(obj_), it(it_) {
    visited[it->first] = true;
    q.push(it);
}

/**
 * Overloads the pre-increment operator for the breadth_first_search_iterator class.
 *
 * @return Returns a breadth_first_search_iterator object.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::breadth_first_search_iterator &Graph<vertex, edge>::breadth_first_search_iterator::operator++() {
    if (!q.empty()) {
        Graph<vertex, edge>::set_iterator begin = q.front()->second.begin();
        Graph<vertex, edge>::set_iterator end = q.front()->second.end();
        q.pop();
        while (begin != end) {
            if (!visited[begin->first]) {
                visited[begin->first] = true;
                q.push(obj.find(begin->first));
            }
            ++begin;
        }
        it = q.front();
    }
    return *this;
}

/**
 * Overloads the post-increment operator for the breadth_first_search_iterator class.
 *
 * @param int The integer parameter (unused).
 *
 * @return Returns a breadth_first_search_iterator object.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::breadth_first_search_iterator Graph<vertex, edge>::breadth_first_search_iterator::operator++(int) {
    breadth_first_search_iterator tmp(*this);
    ++(*this);
    return tmp;
}

/**
 * @brief Dereference operator to access the current vertex in the traversal.
 *
 * @return Reference to the current vertex.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::list_type &Graph<vertex, edge>::breadth_first_search_iterator::operator*() {
    return *it;
}

template <typename vertex, typename edge>
typename Graph<vertex, edge>::iterator Graph<vertex, edge>::breadth_first_search_iterator::operator->() {
    return it;
}

/**
 * Overloads the equality operator for the breadth_first_search_iterator class.
 *
 * @param rhs The right-hand side breadth_first_search_iterator object to compare with.
 *
 * @return Returns true if the iterators are equal, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::breadth_first_search_iterator::operator==(const breadth_first_search_iterator &rhs) const {
    return it == rhs.it;
}

/**
 * Overloads the inequality operator for the breadth_first_search_iterator class.
 *
 * @param rhs The right-hand side breadth_first_search_iterator object to compare with.
 *
 * @return Returns true if the iterators are inequal, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::breadth_first_search_iterator::operator!=(const breadth_first_search_iterator &rhs) const {
    return !(*this == rhs);
}

/**
 * @brief Checks if the breadth first search iterator has reached the end of traversal.
 *
 * @return true if the iterator has reached the end, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::breadth_first_search_iterator::bfsend() {
    return q.empty();
}

/**
 * @brief Constructor for initializing the depth first search iterator.
 *
 * @param obj_ Reference to the graph object.
 * @param it_ Iterator to the start node.
 */
template <typename vertex, typename edge>
Graph<vertex, edge>::depth_first_search_iterator::depth_first_search_iterator(Graph<vertex, edge> &obj_, Graph<vertex, edge>::iterator it_)
    : obj(obj_), it(it_) {
    visited[it->first] = true;
    Graph<vertex, edge>::set_iterator begin = it->second.begin();
    Graph<vertex, edge>::set_iterator end = it->second.end();
    while (begin != end) {
        st.push(obj.find(begin->first));
        ++begin;
    }
}

/**
 * @brief Overloaded pre-increment operator for the depth first search iterator.
 *
 * @return Reference to the updated iterator.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::depth_first_search_iterator &Graph<vertex, edge>::depth_first_search_iterator::operator++() {
    while (!st.empty() && visited[st.top()->first])
        st.pop();

    if (!st.empty()) {
        it = st.top();
        st.pop();
        visited[it->first] = true;
        Graph<vertex, edge>::set_iterator begin = it->second.begin();
        Graph<vertex, edge>::set_iterator end = it->second.end();
        while (begin != end) {
            if (!visited[begin->first])
                st.push(obj.find(begin->first));
            ++begin;
        }
        if (st.empty())
            st.push(it);
    }
    return *this;
}

/**
 * @brief Overloaded post-increment operator for the depth first search iterator.
 *
 * @param int Dummy parameter to differentiate from prefix increment.
 * @return A copy of the iterator before increment.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::depth_first_search_iterator Graph<vertex, edge>::depth_first_search_iterator::operator++(int) {
    depth_first_search_iterator temp(*this);
    ++(*this);
    return temp;
}

/**
 * @brief Dereference operator to access the current vertex in the traversal.
 *
 * @return Reference to the current vertex.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::list_type &Graph<vertex, edge>::depth_first_search_iterator::operator*() {
    return *it;
}

/**
 * @brief Member access operator to access the current iterator.
 *
 * @return The current iterator.
 */
template <typename vertex, typename edge>
typename Graph<vertex, edge>::iterator Graph<vertex, edge>::depth_first_search_iterator::operator->() {
    return it;
}

/**
 * @brief Equality comparison operator for depth first search iterators.
 *
 * @param rhs The iterator to compare with.
 * @return true if equal, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::depth_first_search_iterator::operator==(const depth_first_search_iterator &rhs) const {
    return it == rhs.it;
}

/**
 * @brief Inequality comparison operator for depth first search iterators.
 *
 * @param rhs The iterator to compare with.
 * @return true if not equal, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::depth_first_search_iterator::operator!=(const depth_first_search_iterator &rhs) const {
    return !(*this == rhs);
}

/**
 * @brief Check if the depth first search iterator has reached the end of traversal.
 *
 * @return true if the iterator has reached the end, false otherwise.
 */
template <typename vertex, typename edge>
bool Graph<vertex, edge>::depth_first_search_iterator::dfsend() {
    return st.empty();
}