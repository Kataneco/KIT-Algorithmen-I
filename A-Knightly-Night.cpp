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

unsigned int n;
unsigned int m;
unsigned int** board;
ivec2* path;

inline bool inBounds(ivec2 step) {
    return (step.x >= 0 && step.x < n && step.y >= 0 && step.y < m);
}

void knight(ivec2 pos, int depth) {
    board[pos.x][pos.y] = depth;
    if(depth == n*m - 1) {
        for(int x = 0; x < n; ++x) {
            for(int y = 0; y < m; ++y) {
                path[board[x][y]] = ivec2(x,y);
            }
        }
        std::cout << "<";
        for(int i = 0; i < n*m; ++i) {
            std::cout << "(" << path[i].x+1 << "," << path[i].y+1 << ")";
        }
        std::cout << ">" << std::endl;
    }
    for(const auto& delta : deltas) {
        ivec2 next = pos+delta;
        if(inBounds(next)) {
            if(board[next.x][next.y] == -1) {
                knight(next, depth+1);
            }
        }
    }
    board[pos.x][pos.y] = -1;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: ./knights n m" << std::endl;
        return -1;
    }

    n = std::stoul(argv[1]);
    m = std::stoul(argv[2]);
    
    board = new unsigned int*[n];
    for(int i = 0; i < n; ++i) {
        board[i] = new unsigned int[m];
        for(int j = 0; j < m; ++j) {
            board[i][j] = -1;
        }
    }
    path = new ivec2[n*m];

    knight(ivec2(0,0), 0);
}