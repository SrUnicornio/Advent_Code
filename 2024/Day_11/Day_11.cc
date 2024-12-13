#include <bits/stdc++.h>

using namespace std;

/**
 * @brief Separa una cadena en un vector de cadenas utilizando un delimitador
 * @param str cadena a separar
 * @param splitter separador
 * @return vector<string> con las cadenas separadas
 */
vector<string> splitString(string str, char splitter) {
    vector<string> result;
    string current = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == splitter) {
            if (current != "") {
                result.push_back(current);
                current = "";
            }
            continue;
        }
        current += str[i];
    }
    if (current.size() != 0) {
        result.push_back(current);
    }
    return result;
}

/**
 * @brief Función que verifica si un punto está dentro de los límites de una matriz
 */
struct pair_hash {
    template <class T1, class T2>
    long long operator () (const pair<T1,T2> &p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

/**
 * @brief Función que verifica si un punto está dentro de los límites de una matriz
 * @param r fila
 * @param c columna
 * @param aRow número de filas de la matriz
 * @param aCol número de columnas de la matriz
 * @return true si el punto está dentro de los límites de la matriz, false en caso contrario
 */
unordered_map<pair<long long, long long>, long, pair_hash> cache;
long long compute(vector<long long> a, int i) {
    if (i == 0) {
        return a.size();
    }
    long long res = 0;
    for (auto x : a) {
        pair<long long, long long> xp = make_pair(x, i);
        if (cache.find(xp) != cache.end()) {
            res += cache[xp];
        } else {
            vector<long long> xpres;
            if (x == 0) {
                xpres.push_back(1);
            } else if (to_string(x).size() % 2 == 0) {
                string xs = to_string(x);
                string h1 = xs.substr(0, xs.size() / 2);
                string h2 = xs.substr(xs.size() / 2);
                xpres.push_back(stoll(h1));
                xpres.push_back(stoll(h2));
            } else {
                xpres.push_back(x * 2024);
            }
            cache[xp] = compute(xpres, i - 1);
            res += cache[xp];
        }
    }
    return res;
}

int main() {
    vector<long long> d;

    ifstream file("input.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
                for (auto x : splitString(line, ' ')) {
                    d.push_back(stoll(x));
                }
            }
    } else {
        cout << "Unable to read from input file\n";
        return 1;
    }
    
    cout << "Part 1: " << compute(d, 25) << endl;
    cout << "Part 2: " << compute(d, 75) << endl;

    return 0;
}