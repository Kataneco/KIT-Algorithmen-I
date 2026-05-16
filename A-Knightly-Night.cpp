/*
Dr. Meta hat seinen Lieblingsalgorithmus verloren. Um diesen zu finden, muss er bevor
der Tag beginnt alle seine Geheimverstecke durchsuchen. Da er Angst hat enttarnt zu
werden, kann er nicht von einem Geheimversteck zu einem benachbarten Geheimversteck
gehen und muss stets einen Umweg wählen.
Wir modellieren das Problem wie folgt. Die n · m Geheimverstecke (im Folgenden
Zellen genannt) sind in einem Gitter mit n Zeilen und m Spalten, jeweils eins-indiziert,
angeordnet. Dr. Meta kann in jedem Schritt in eine Zelle ziehen, die zwei Zeilen und
eine Spalte - oder umgekehrt - entfernt ist. Formal gilt also f¨ur jede Zelle (i, j) und ihren
Nachfolger (i′, j′) in der Zugfolge: |i − i′| = 2 ∧ |j − j′| = 1 oder |i − i′| = 1 ∧ |j − j′| = 2.
Ziel ist es Touren zu finden, die jedes Geheimversteck genau einmal besuchen. Jede
Tour beginnt hierbei in der oberen linken Ecke und endet in einer beliebigen Zelle.

Gib Pseudocode für einen möglichst effizienten rekursiven Algorithmus, der alle
Touren für gegebenes n und m findet, an.
*/

// Implementation tweaks:
// The algorithm is iterative instead of recursive which saves me from stack overflows
// A recursive algorithm might have been simpler to write idk but I didn't even notice that keyword
// when solving this problem

#include <vector>
#include <iostream>
#include <string>

class ivec2 {
public:
    int x;
    int y;

    ivec2() {}
    ivec2(int x, int y) : x(x), y(y) {}

    ivec2 operator+(ivec2 rhs) {
        return {x + rhs.x, y + rhs.y};
    }

    ivec2 operator-(ivec2 rhs) {
        return {x - rhs.x, y - rhs.y};
    }
    
    bool operator==(ivec2 rhs) {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(ivec2 rhs) {
        return x != rhs.x || y != rhs.y;
    }
};

struct Node {
    ivec2 value;
    int parent;
};

const std::vector<ivec2> deltas = {
    ivec2(1,2),
    ivec2(-1,2),
    ivec2(1,-2),
    ivec2(-1,-2),
    ivec2(2,1),
    ivec2(-2,1),
    ivec2(2,-1),
    ivec2(-2,-1)
};
std::vector<Node> nodes;
std::vector<std::vector<ivec2>> paths;
unsigned int n;
unsigned int m;

bool isValid(Node node, ivec2 step) {
    ivec2 u = node.value;
    ivec2 h = u+step;
    if(h.x <= 0 || h.y <= 0) return false;
    if(h.x > n || h.y > m) return false;
    Node last = node;
    do {
        if (h == last.value) return false;
        if (last.parent > -1) {
            last = nodes[last.parent];
        }
    } while (last.value != ivec2(1,1));
    return h != last.value;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: ./knights n m" << std::endl;
        return -1;
    }

    n = std::stoul(argv[1]);
    m = std::stoul(argv[2]);

    Node root{};
    root.value = ivec2(1,1);
    root.parent = -1;
    nodes.push_back(root);
    
    int lastleaf = 0;
    for(int i = 0; i < n*m; ++i) {
        int round = 0;
        int end = nodes.size();
        for(int leaf = lastleaf; leaf < end; ++leaf) {
            for(const auto& delta : deltas) {
                if(isValid(nodes[leaf], delta)) {
                    Node n{};
                    n.parent = leaf;
                    n.value = nodes[leaf].value + delta;
                    nodes.push_back(n);
                    round++;
                }
            }
        }
        if(round == 0) break;
        lastleaf = (nodes.size()-1)-round;
    }

    for(int leaf = lastleaf; leaf < nodes.size(); ++leaf) {
        std::vector<ivec2> path;
        Node last = nodes[leaf];
        if(last.value == ivec2(1,1)) break;
        do {
            path.push_back(last.value);
            if(last.parent > -1) {
                last = nodes[last.parent];
            }
        } while (last.value != ivec2(1,1));
        path.push_back(last.value);
        if (path.size() == n*m) {
            paths.push_back(path);
        }
    }

    for(const auto& path : paths) {
        std::cout << "<";
        for(int i = path.size()-1; i >= 0; --i) {
            std::cout << "(" << path[i].x << "," << path[i].y << ")";
        }
        std::cout << ">" << std::endl;
    }
}