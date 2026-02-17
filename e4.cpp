#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <memory>

// ==== NODO DEL GRAFO ====

class Node {
private:
    int id;

public:
    Node(int id) : id(id) {}
    int getId() const { return id; }
};

// === GRAFO ===
class Graph {
private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<int>> adjList;

public:
    // Method para agregar arista dirigida
    void addEdge(int from, int to) {
        if(nodes.find(from) == nodes.end())
            nodes.emplace(from, Node(from));
        if(nodes.find(to) == nodes.end())
            nodes.emplace(to, Node(to));
        adjList[from].push_back(to);
    }

    std::vector<int> getNeighbors(int nodeId) const {
        auto it = adjList.find(nodeId);
        if (it != adjList.end())
            return it->second;
        return {};
    }

    bool hasNode(int nodeId) const {
        return nodes.find(nodeId) != nodes.end();
    }
};


//Interfaz iterator
class GraphIterator {
public:
    virtual ~GraphIterator() = default;
    virtual bool hasNext() = 0;
    virtual int next() = 0;
};

// === ITERADOR BFS ===
class BFSIterator : public GraphIterator {
private:
    const Graph& graph;
    std::queue<int> q;
    std::unordered_set<int> visited;

public:
    BFSIterator(class Graph& g, int start): graph(g) {
        if (g.hasNode(start)) {
            q.push(start);
            visited.insert(start);
        }
    };

    bool hasNext() override {
        return !q.empty();
    }

    int next() override {
        int current = q.front();
        q.pop();

        for (int neighbor : graph.getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
        return current;
    }
};

// ==== ITERADOR DFS ====

class DFSIterator : public GraphIterator {
private:
    const Graph& graph;
    std::stack<int> s;
    std::unordered_set<int> visited;

public:
    DFSIterator(const Graph& g, int start): graph(g){
        if (g.hasNode(start)) {
            s.push(start);
            visited.insert(start);
        }
    };

    bool hasNext() override {
        return !s.empty();
    }

    int next() override {
        int current = s.top();
        s.pop();

        // Apilamos en orden inverso para mantener el orden natural
        auto neighbors = graph.getNeighbors(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            int neighbor = *it;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                s.push(neighbor);
            }
        }
        return current;
    }
};

int main() {
    Graph g;

    // Construimos grafo
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    // Ejecutamos recorrido BFS
    std::cout << "BFS desde nodo 1: ";
    std::unique_ptr<GraphIterator> it = std::make_unique<BFSIterator>(g, 1);

    while (it->hasNext()) {
        std::cout << it->next() << " ";
    }
    std::cout << std::endl;

    // Prueba adicional de DFS (no requerida pero muestra funcionamiento)
    std::cout << "DFS desde nodo 1: ";
    it = std::make_unique<DFSIterator>(g, 1);
    while (it->hasNext()) {
        std::cout << it->next() << " ";
    }
    std::cout << std::endl;

    return 0;
}