#include <bits/stdc++.h>

typedef std::pair<int, int> Position;

const std::unordered_map<int, Position> directions{
    {0, {-1, 0}},  // Norte
    {1, {0, 1}},   // Este
    {2, {1, 0}},   // Sur
    {3, {0, -1}}   // Oeste
};

struct State {
  int cost, x, y, dir;
  bool operator>(const State& other) const { return cost > other.cost; }
};

std::vector<std::string> readInput(const std::string& file) {
  std::ifstream in{file};
  if (!in) {
    std::cerr << "Cannot open the File: " << file << std::endl;
    return {};
  }

  std::string str;
  std::vector<std::string> maze;
  while (std::getline(in, str)) {
    maze.push_back(str);
  }

  return maze;
}

Position findCell(const std::vector<std::string>& maze, char cell) {
  int rows = maze.size();
  int cols = maze[0].size();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (maze[i][j] == cell) {
        return {i, j};
      }
    }
  }

  return {-1, -1};
}

int heuristicManhattan(const Position& current, const Position& end,
                       int W = 3) {
  return (std::abs(end.first - current.first) +
          std::abs(end.second - current.second)) *
         W;
}

long long part1(const std::vector<std::string>& maze, const Position& start,
                const Position& end) {
  int rows = maze.size();
  int cols = maze[0].size();

  std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
  std::set<std::tuple<int, int, int>> visited;

  for (int dir = 0; dir < 4; dir++) {
    pq.push({heuristicManhattan(start, end), start.first, start.second, dir});
  }

  while (!pq.empty()) {
    auto [cost, x, y, dir] = pq.top();
    pq.pop();

    if (x == end.first && y == end.second) {
      return cost;
    }

    if (visited.count({x, y, dir})) continue;
    visited.insert({x, y, dir});

    // Avanzar
    int nx = x + directions.at(dir).first;
    int ny = y + directions.at(dir).second;

    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != '#') {
      pq.push({cost + 1, nx, ny, dir});
    }

    // Girar
    for (int turn = -1; turn <= 1; turn += 2) {
      int newDir = (dir + turn + 4) % 4;
      pq.push({cost + 1000, x, y, newDir});
    }
  }

  return -1;  // Si no se encuentra un camino
}

int main() {
  std::string file{"input.txt"};
  std::vector<std::string> maze = readInput(file);

  if (maze.empty()) return 1;

  Position start = findCell(maze, 'S');
  Position end = findCell(maze, 'E');

  if (start.first == -1 || end.first == -1) {
    std::cerr << "Start or End cell not found!" << std::endl;
    return 1;
  }

  std::cout << "Start: " << start.first << " " << start.second << std::endl;
  std::cout << "End: " << end.first << " " << end.second << std::endl;

  std::cout << "Lowest cost: " << part1(maze, start, end) << std::endl;

  return 0;
}
