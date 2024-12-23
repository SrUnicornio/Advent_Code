#include <bits/stdc++.h>

using namespace std;

// Coordenadas de movimiento: derecha, abajo, izquierda, arriba
const vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// Lee el archivo de entrada y carga las paredes y elementos adicionales
void readFile(const string &filename, set<pair<int, int>> &walls,
              vector<pair<int, int>> &additionals) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Cannot open the file: " << filename << endl;
    exit(1);
  }

  string line;
  int count = 0;
  while (getline(file, line)) {
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    stringstream ss(line);
    int x, y;
    char comma;
    ss >> x >> comma >> y;

    if (count < 1024) {
      walls.emplace(x, y);
    } else {
      additionals.emplace_back(x, y);
    }
    count++;
  }

  file.close();
}

// Heurística Manhattan para A*
int heuristic(const pair<int, int> &current, const pair<int, int> &end) {
  return abs(current.first - end.first) + abs(current.second - end.second);
}

// Algoritmo A* para encontrar el camino más corto
pair<int, vector<pair<int, int>>> aStarSearch(
    const pair<int, int> &start, const pair<int, int> &end, int maxX, int maxY,
    const set<pair<int, int>> &walls) {
  priority_queue<tuple<int, int, pair<int, int>, vector<pair<int, int>>>> pq;
  set<pair<int, int>> visited;

  pq.push({0, 0, start, {start}});

  while (!pq.empty()) {
    auto [cost, steps, current, path] = pq.top();
    pq.pop();
    cost = -cost;

    if (visited.count(current)) continue;
    visited.insert(current);

    if (current == end) {
      return {steps, path};
    }

    for (const auto &[dx, dy] : directions) {
      int nx = current.first + dx;
      int ny = current.second + dy;
      pair<int, int> neighbor = {nx, ny};

      if (nx >= 0 && nx <= maxX && ny >= 0 && ny <= maxY &&
          !walls.count(neighbor) && !visited.count(neighbor)) {
        vector<pair<int, int>> newPath = path;
        newPath.push_back(neighbor);
        int newCost = steps + 1 + heuristic(neighbor, end);
        pq.push({-newCost, steps + 1, neighbor, newPath});
      }
    }
  }

  return {-1, {}};
}

void part_1(const pair<int, int> &start, const pair<int, int> &end, int maxX,
            int maxY, const set<pair<int, int>> &walls) {
  auto [score, lastPath] = aStarSearch(start, end, maxX, maxY, walls);
  cout << "Score: " << score << endl;
}

void part_2(const pair<int, int> &start, const pair<int, int> &end, int maxX,
            int maxY, const set<pair<int, int>> &walls,
            const vector<pair<int, int>> &additionals) {
  auto [score, lastPath] = aStarSearch(start, end, maxX, maxY, walls);
  for (size_t i = 0; i < additionals.size(); ++i) {
    if (find(lastPath.begin(), lastPath.end(), additionals[i]) ==
        lastPath.end())
      continue;

    set<pair<int, int>> newWalls = walls;
    newWalls.insert(additionals.begin(), additionals.begin() + i + 1);

    auto [newScore, newPath] = aStarSearch(start, end, maxX, maxY, newWalls);
    if (newScore == -1) {
      cout << "Blocking point: (" << additionals[i].first << ", "
           << additionals[i].second << ")" << endl;
      break;
    }

    lastPath = newPath;
  }
}

int main() {
  string filename = "input.txt";
  set<pair<int, int>> walls;
  vector<pair<int, int>> additionals;

  readFile(filename, walls, additionals);

  pair<int, int> start = {0, 0};
  pair<int, int> end = {70, 70};
  int maxX = 70, maxY = 70;

  part_1(start, end, maxX, maxY, walls);
  part_2(start, end, maxX, maxY, walls, additionals);

  return 0;
}
