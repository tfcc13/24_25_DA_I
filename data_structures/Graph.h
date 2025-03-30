// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

/**
* @file Graph.h
* @brief Header file for the graph structure used in the project.
*
* This file defines the `Vertex`, `Edge`, and `Graph` classes for representing
* a directed or undirected graph, along with auxiliary functions for manipulating
* and querying vertices, edges, and the overall graph structure.
*/

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

/**
* @class Vertex
* @brief Represents a vertex in the graph.
*
* A vertex holds information about its outgoing edges, incoming edges,
* distances, and other properties like visited status and indegree.
*
* @tparam T Type of the data held in the vertex.
*/
template <class T>
class Vertex {
public:
    /**
    * @brief Constructs a Vertex with the specified information.
    * @param in The information to be stored in the vertex.
    */
    Vertex(T in);

    /**
    * @brief Comparison operator for sorting in a priority queue.
    * @param vertex Another vertex to compare against.
    * @return True if the current vertex's distance is less than the other's.
    */
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    /** Getters and setters for the vertex's properties */
    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setProcessing(bool processing);

    int getLow() const;
    void setLow(int value);
    int getNum() const;
    void setNum(int value);
    int getQueueIndex() const;  // needed for heap
    void setQueueIndex(int value);    // needed for heap

    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);

    /**
    * @brief Adds an outgoing edge from this vertex to the destination vertex.
    * @param d The destination vertex.
    * @param walking_time The time required to walk along this edge.
    * @param driving_time The time required to drive along this edge.
    * @return A pointer to the created edge.
    */
    Edge<T> * addEdge(Vertex<T> *dest,  double walking_time, double driving_time);

    /**
    * @brief Removes an outgoing edge to a specified destination vertex.
    * @param in The information of the destination vertex.
    * @return True if the edge was removed, false otherwise.
    */
    bool removeEdge(T in);

    /**
    * @brief Removes all outgoing edges from this vertex.
    */
    void removeOutgoingEdges();

protected:
    T info;                ///< Information of the vertex
    std::vector<Edge<T> *> adj;  ///< Outgoing edges from the vertex

    // auxiliary fields
    // used by DFS, BFS, Prim ...
    bool visited = false; ///< Flag for visited status
    bool processing = false;  ///< Flag for processing status
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;  ///< Shortest distance from source
    Edge<T> *path = nullptr; ///< Shortest path edge


    std::vector<Edge<T> *> incoming; ///< Incoming edges to the vertex


    int queueIndex = 0; 		///< Required for heap-based priority queue operations

    /**
    * @brief Deletes a specific edge from the vertex.
    * @param edge The edge to be deleted.
    */
    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

/**
* @class Edge
* @brief Represents an edge in the graph.
*
* An edge connects two vertices and stores properties like walking and driving times,
* as well as flow for flow-related problems.
*
* @tparam T Type of the data held in the vertices.
*/
template <class T>
class Edge {
public:
    /**
    * @brief Constructs an edge from the source to the destination vertex.
    * @param orig The origin vertex.
    * @param dest The destination vertex.
    * @param walking_time The time required to walk along this edge.
    * @param driving_time The time required to drive along this edge.
    */
    Edge(Vertex<T> *orig, Vertex<T> *dest,  double walking_time, double driving_time);

    /** Getters and setters for the edge's properties */
    Vertex<T> * getDest() const;
    double getWalkingTime() const;
    double getDrivingTime() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
    void setFlow(double flow);
protected:
    // used for bidirectional edges
    Vertex<T> *orig; ///< Origin vertex
    Vertex<T> * dest;  ///< Destination vertex
    double walking_time_; ///< Walking time for the edge
    double driving_time_; ///< Driving time for the edge
    // auxiliary fields
    bool selected = false; ///< Whether the edge is selected


    Edge<T> *reverse = nullptr; ///< Reverse edge for bidirectional edges

    double flow; ///< Flow for flow-related problems
};

/********************** Graph  ****************************/

/**
* @class Graph
* @brief Represents a graph structure.
*
* The graph consists of a collection of vertices and edges. It provides methods
* for adding/removing vertices and edges, and for querying graph properties.
*
* @tparam T Type of the data held in the vertices.
*/
template <class T>
class Graph {
public:
    ~Graph();
    /**
    * @brief Finds a vertex in the graph by its information.
    * @param in The information of the vertex to find.
    * @return A pointer to the vertex if found, nullptr otherwise.
    */
    Vertex<T> *findVertex(const T &in) const;

    /**
    * @brief Adds a vertex to the graph.
    * @param in A pointer to the vertex to add.
    * @return True if the vertex was added, false if it already exists.
    */
    bool addVertex(Vertex<T> *in);

    /**
    * @brief Removes a vertex from the graph.
    * @param in The information of the vertex to remove.
    * @return True if the vertex was removed, false otherwise.
    */
    bool removeVertex(const T &in);

    /**
    * @brief Adds an edge between two vertices in the graph.
    * @param sourc The source vertex information.
    * @param dest The destination vertex information.
    * @param walking_time The walking time for the edge.
    * @param driving_time The driving time for the edge.
    * @return True if the edge was added, false if the vertices don't exist.
    */
    bool addEdge(const T &sourc, const T &dest, double walking_time, double driving_time);

    /**
    * @brief Removes an edge between two vertices in the graph.
    * @param sourc The source vertex information.
    * @param dest The destination vertex information.
    * @return True if the edge was removed, false otherwise.
    */
    bool removeEdge(const T &source, const T &dest);

    /**
    * @brief Adds a bidirectional edge between two vertices.
    * @param sourc The source vertex information.
    * @param dest The destination vertex information.
    * @param walking_time The walking time for the edge.
    * @param driving_time The driving time for the edge.
    * @return True if the bidirectional edge was added, false if the vertices don't exist.
    */
    bool addBidirectionalEdge(const T &sourc, const T &dest,  double walking_time, double driving_time);

    /**
    * @brief Gets the number of vertices in the graph.
    * @return The number of vertices.
    */
    int getNumVertex() const;

    /**
     * @brief Gets a vector of all the vertices in the graph.
     * @return A vector containing all vertices.
     */
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;    ///< Collection of vertices in the graph

    double ** distMatrix = nullptr;   ///< Distance matrix for algorithms like Floyd-Warshall
    int **pathMatrix = nullptr;  //< Path matrix for algorithms like Floyd-Warshall

    /**
    * @brief Finds the index of a vertex based on its information.
    * @param in The information of the vertex to find.
    * @return The index of the vertex, or -1 if not found.
    */
    int findVertexIdx(const T &in) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double walking_time, double driving_time) {
    auto newEdge = new Edge<T>(this, d, walking_time, driving_time);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template <class T>
int Vertex<T>::getQueueIndex() const {
    return this->queueIndex;
}

template <class T>
void Vertex<T>::setQueueIndex(int value) {
    this->queueIndex = value;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double walking_time, double driving_time): orig(orig), dest(dest), walking_time_(walking_time),driving_time_(driving_time) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

template <class T>
double Edge<T>::getWalkingTime() const {
    return this->walking_time_;
}

template <class T>
double Edge<T>::getDrivingTime() const {
    return this->driving_time_;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */

// alterou-se para se introduzir um vértice genérico
template <class T>
bool Graph<T>::addVertex(Vertex<T> *in) {
    if (findVertex(in->getInfo()) != nullptr)
        return false;
    vertexSet.push_back(in);
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double walking_time, double driving_time) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, walking_time, driving_time);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest,  double walking_time, double driving_time) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2,  walking_time, driving_time);
    auto e2 = v2->addEdge(v1,   walking_time,  driving_time);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */