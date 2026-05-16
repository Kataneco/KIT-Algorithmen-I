/*
Dr. Meta ist mit der ihm zur Verfügung stehenden Auswahl an Datenstrukturen nicht
zufrieden. Daher beauftragt er dich aus Arrays und Listen eine neue Datenstruktur zu
bauen. Dafür hinterlässt er dir folgende Anweisungen:
Die Datenstruktur besteht aus einem Array A, dessen Einträge sortierte Listen sind.
Mit ℓ bezeichnen wir die Länge von A, also die Anzahl der Listen. Jede Liste A[i] erfüllt
zwei Eigenschaften:
• A[i] hat maximal Länge k = √ℓ
• first(A[i]) ≥ last(A[i − 1])
Das heißt, wenn wir alle Listen in der Reihenfolge konkatenieren wie sie im Array auf-
treten, dann enthält die resultierende Liste h¨ochstens ℓ · k = ℓ · √ℓ Elemente in sortierter
Reihenfolge.

Die Datenstruktur unterstützt nun folgenden Operationen:
• find(x): Gibt aus, ob ein Element x in unserer Datenstruktur ist.
• add(x): Fügt ein Element x passend sortiert in die Datenstruktur ein, sodass x
an das nächst-kleinere Element angehängt wird (Du kannst annehmen, dass ein
solches kleineres Element immer existiert).
• restructure(): Wird automatisch ausgeführt, sobald die Liste, in die add das
Element x einfügt, anschließend eine Länge größer als k hat. Die Methode ordnet
die Elemente aller Listen um und erstellt eine neue Datenstruktur, in der jede Liste
Länge 1 hat (und das Array immer noch sortiert ist). Dieser Prozess des Umbauens
dauert Θ(ℓ′), wobei ℓ′ nun die neue Anzahl Listen ist.
*/

#include <cstdint>
#include <cstring>
#include <cmath>

// Implementation tweaks:
// The inner singly forward linked lists were replaced with arrays to simplify
class MetaSortedList {
public:
    bool find(int x, int& i, int& j) {
        i = 0;
        j = 0;
        if(l == 0) return false;
        int offset = l/2;
        for(i = l/2; i >= 0 && i < l;) {
            if(A[i][0] == x) {
                return true;
            }
            if(offset <= 0) {
                return false;
            }
            if(A[i][0] > x) {
                i -= offset;
                offset /= 2;
                continue;
            }
            int size = A_sizes[i]-1;
            if(A[i][size] < x) {
                i += offset;
                offset /= 2;
                continue;
            }
            int inner_offset = size/2;
            for(j = size/2; j >= 0 && j < size;) {
                if(A[i][j] == x) {
                    return true;
                }
                if(inner_offset <= 0) {
                    return false;
                }
                if(A[i][j] > x) {
                    j -= inner_offset;
                    inner_offset /= 2;
                    continue;
                } else {
                    j += inner_offset;
                    inner_offset /= 2;
                    continue;
                }
            }
            return false;
        }
        return false;
    }

    void add(int x) {
        if(l == 0) {
            A[0][0] = x;
            l = 1;
            k = 1;
            return;
        }
        int i;
        int j;
        find(x, i, j);
        int next = j+1 < A_sizes[i] ? j+1 : j;
        if(A[i][next] <= x) {
            int* buffer = new int[A_sizes[i]+1];
            memcpy(buffer, A[i], sizeof(int)*(next+1));
            buffer[next+1] = x;
            memcpy(buffer+next+2, A[i]+next+1, sizeof(int)*(A_sizes[i]-(next+1)));
            delete[] A[i];
            A_sizes[i]++;
            A[i] = buffer;
        } else {
            int* buffer = new int[A_sizes[i]+1];
            memcpy(buffer, A[i], sizeof(int)*(next));
            buffer[next] = x;
            memcpy(buffer+next+1, A[i]+next, sizeof(int)*(A_sizes[i]-(next)));
            delete[] A[i];
            A[i] = buffer;
            A_sizes[i]++;
        }
        if(A_sizes[i] > k) {
            restructure();
        }
    }

    MetaSortedList() {
        l = 0;
        k = 0;
        A = new int*[1];
        A[0] = new int[1];
        A_sizes = new int[1];
        A_sizes[0] = 1;
    }

    ~MetaSortedList() {
        for(int i = 0; i < l; ++i) {
            delete[] A[i];
        }
        delete[] A_sizes;
        delete[] A;
    }

private:
    int l;
    int k;

    int** A;
    int* A_sizes;

    void restructure() {
        int length = 0;
        for(int i = 0; i < l; ++i) {
            length += A_sizes[i];
        }
        int** buffer = new int*[length];
        int* size_buffer = new int[length];
        int count = 0;
        for(int i = 0; i < l; ++i) {
            for(int j = 0; j < A_sizes[i]; ++j) {
                size_buffer[count] = 1;
                buffer[count] = new int[1];
                buffer[count][0] = A[i][j];
                count++;
            }
        }
        for(int i = 0; i < l; ++i) {
            delete[] A[i];
        }
        delete[] A_sizes;
        delete[] A;
        A = buffer;
        A_sizes = size_buffer;
        l = length;
        k = static_cast<int>(sqrt(length));
    }
};
