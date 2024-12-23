#include <bits/stdc++.h>
using namespace std;

// Estructura que define el estado del juego
struct Game {
    int nbLayers;
    vector<string> codes;
    map<string, vector<vector<char>>> grids;
};

// Divide una cadena en un vector de cadenas usando un delimitador
vector<string> splitString(const string &str, char splitter) {
    vector<string> vec;
    size_t start = 0, end = 0;
    while ((end = str.find(splitter, start)) != string::npos) {
        if (end != start) vec.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    if (start < str.size()) vec.push_back(str.substr(start));
    return vec;
}

// Lee el contenido del archivo de entrada
string readInput(char *argv[]) {
    if (argv[1] == nullptr) {
        cerr << "Error: Proporcione la ruta al archivo de entrada.\n";
        exit(EXIT_FAILURE);
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada.\n";
        exit(EXIT_FAILURE);
    }

    string content((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Elimina saltos de línea adicionales al final
    while (!content.empty() && content.back() == '\n') content.pop_back();
    return content;
}

// Crea el estado inicial del juego
Game createGame(const string &input, int nbLayers) {
    Game game;
    game.codes = splitString(input, '\n');
    game.grids["numpad"] = {
        {'7', '8', '9'}, {'4', '5', '6'}, {'1', '2', '3'}, {' ', '0', 'A'}};
    game.grids["arrowPad"] = {
        {' ', '^', 'A'},
        {'<', 'v', '>'}};
    game.nbLayers = nbLayers;
    return game;
}

// Obtiene la parte numérica de un código
int getDigitPart(const string &code) {
    regex re("\\d+");
    smatch match;
    if (regex_search(code, match, re)) {
        return stoi(match.str());
    }
    return 0;  // Devuelve 0 si no se encuentra un número
}

// Encuentra la posición de un carácter en una cuadrícula
vector<int> getPos(char c, const Game &game, const string &grid) {
    const auto &rows = game.grids.at(grid);
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] == c) return {i, j};
        }
    }
    return {-1, -1};  // Devuelve una posición inválida si no se encuentra
}

// Calcula las direcciones necesarias para moverse de un punto a otro
tuple<char, char> getDirections(const vector<int> &start, const vector<int> &end) {
    char dirVertical = (end[0] < start[0]) ? '^' : (end[0] > start[0]) ? 'v' : '\0';
    char dirHorizontal = (end[1] < start[1]) ? '<' : (end[1] > start[1]) ? '>' : '\0';
    return {dirHorizontal, dirVertical};
}

// Encuentra el camino más corto utilizando memoización
int64_t findShortestPath(const vector<int> &start, const vector<int> &end, const Game &game,
                         int layer, unordered_map<string, int64_t> &cache) {
    if (layer == 0) return 1;

    string key = to_string(start[0]) + to_string(start[1]) + to_string(end[0]) +
                 to_string(end[1]) + to_string(layer);

    if (cache.count(key)) return cache[key];

    auto [dirHorizontal, dirVertical] = getDirections(start, end);
    vector<int> nextHorizontal = getPos(dirHorizontal, game, "arrowPad");
    vector<int> nextVertical = getPos(dirVertical, game, "arrowPad");
    vector<int> posA = getPos('A', game, "arrowPad");

    int64_t result = 0;

    if (dirHorizontal == '\0' && dirVertical == '\0') {
        result = findShortestPath(posA, posA, game, layer - 1, cache);
    } else if (dirHorizontal != '\0' && dirVertical == '\0') {
        result = findShortestPath(posA, nextHorizontal, game, layer - 1, cache) +
                 abs(end[1] - start[1]) *
                     findShortestPath(nextHorizontal, nextHorizontal, game, layer - 1, cache) +
                 findShortestPath(nextHorizontal, posA, game, layer - 1, cache);
    } else if (dirHorizontal == '\0' && dirVertical != '\0') {
        result = findShortestPath(posA, nextVertical, game, layer - 1, cache) +
                 abs(end[0] - start[0]) *
                     findShortestPath(nextVertical, nextVertical, game, layer - 1, cache) +
                 findShortestPath(nextVertical, posA, game, layer - 1, cache);
    } else {
        result = min(
            findShortestPath(posA, nextHorizontal, game, layer - 1, cache) +
                abs(end[1] - start[1]) *
                    findShortestPath(nextHorizontal, nextHorizontal, game, layer - 1, cache) +
                findShortestPath(nextHorizontal, nextVertical, game, layer - 1, cache) +
                abs(end[0] - start[0]) *
                    findShortestPath(nextVertical, nextVertical, game, layer - 1, cache) +
                findShortestPath(nextVertical, posA, game, layer - 1, cache),
            findShortestPath(posA, nextVertical, game, layer - 1, cache) +
                abs(end[0] - start[0]) *
                    findShortestPath(nextVertical, nextVertical, game, layer - 1, cache) +
                findShortestPath(nextVertical, nextHorizontal, game, layer - 1, cache) +
                abs(end[1] - start[1]) *
                    findShortestPath(nextHorizontal, nextHorizontal, game, layer - 1, cache) +
                findShortestPath(nextHorizontal, posA, game, layer - 1, cache));
    }

    cache[key] = result;
    return result;
}

// Resuelve el problema
int64_t resolve(const string &input, int nbLayers) {
    Game game = createGame(input, nbLayers);
    int64_t result = 0;
    unordered_map<string, int64_t> cache;

    for (const string &code : game.codes) {
        int digitPart = getDigitPart(code);
        string fullCode = 'A' + code;
        vector<tuple<char, char>> pairs;

        for (size_t i = 0; i < fullCode.size() - 1; ++i) {
            pairs.emplace_back(fullCode[i], fullCode[i + 1]);
        }

        int64_t totalPresses = 0;
        for (const auto &[startChar, endChar] : pairs) {
            vector<int> startPos = getPos(startChar, game, "numpad");
            vector<int> endPos = getPos(endChar, game, "numpad");
            totalPresses += findShortestPath(startPos, endPos, game, nbLayers, cache);
        }

        result += totalPresses * digitPart;
    }

    return result;
}

// Soluciones para las partes 1 y 2
int64_t solution_1(const string &input) { return resolve(input, 3); }
int64_t solution_2(const string &input) { return resolve(input, 26); }

int main(int argc, char *argv[]) {
    string input = readInput(argv);

    cout << "Part 1: " << solution_1(input) << '\n';
    cout << "Part 2: " << solution_2(input) << '\n';

    return 0;
}
