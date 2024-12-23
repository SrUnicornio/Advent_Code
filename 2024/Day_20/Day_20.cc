#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

typedef long long num;

struct point {
  num i = 0, j = 0;

  bool operator==(const point &pRHS) const {
    return i == pRHS.i && j == pRHS.j;
  }
};

const point DELTA[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

struct cell {
  bool wall;
  bool visited = false;
  num cost = 0;
};

typedef std::vector<std::vector<cell>> map;

struct data {
  map mapData;
  point start, end;
};

void bfs(data &pInput);
num count_skips_adjacent(const data &pInput);
void part1(data pInput);
bool valid(const data &pInput, const point &&pPos);
num count_skips_all(const data &pInput, const num maxDist);
void part2(data pInput);

int main() {
  data input;
  std::ifstream fin("input.txt");
  std::string line;
  int i = 0;

  while (std::getline(fin, line)) {
    int j = 0;
    input.mapData.emplace_back();

    for (const char ch : line) {
      switch (ch) {
        case 'S':
          input.start = {i, j};
          break;
        case 'E':
          input.end = {i, j};
          break;
      }
      input.mapData.back().push_back({ch == '#'});
      j++;
    }

    i++;
  }

  part1(input);
  part2(input);
  return 0;
}

void bfs(data &pInput) {
  struct node {
    int cost;
    point pos;
  };

  std::queue<node> queue;
  queue.push({0, pInput.start});
  pInput.mapData[pInput.start.i][pInput.start.j].visited = true;

  while (!queue.empty()) {
    node data = queue.front();
    queue.pop();

    pInput.mapData[data.pos.i][data.pos.j].cost = data.cost;

    if (pInput.end == data.pos) {
      continue;
    }

    for (int dir = 0; dir < 4; dir++) {
      point next = {data.pos.i + DELTA[dir].i, data.pos.j + DELTA[dir].j};

      cell &c = pInput.mapData[next.i][next.j];
      if (!c.wall && !c.visited) {
        queue.push({data.cost + 1, next});
        c.visited = true;
      }
    }
  }
}

num count_skips_adjacent(const data &pInput) {
  std::unordered_map<num, num> skip_count;

  for (num i = 1; i < pInput.mapData.size() - 1; i++) {
    for (num j = 1; j < pInput.mapData[i].size() - 1; j++) {
      const cell &here = pInput.mapData[i][j];

      if (!here.wall) {
        continue;
      }

      for (int dir = 0; dir < 4; dir++) {
        num deltaI = DELTA[dir].i, deltaJ = DELTA[dir].j;
        const cell &first = pInput.mapData[i + deltaI][j + deltaJ];
        const cell &second = pInput.mapData[i - deltaI][j - deltaJ];

        num save = first.cost - second.cost - 2;

        if (!first.wall && !second.wall && save > 0) {
          skip_count[save]++;
        }
      }
    }
  }

  num result = 0;
  for (auto iter = skip_count.begin(); iter != skip_count.end(); iter++) {
    if (iter->first >= 100) {
      result += iter->second;
    }
  }

  return result;
}

void part1(data pInput) {
  bfs(pInput);
  std::cout << "part 1: " << count_skips_adjacent(pInput) << std::endl;
}

bool valid(const data &pInput, const point &&pPos) {
  return pPos.i >= 0 && pPos.i < pInput.mapData.size() && pPos.j >= 0 &&
         pPos.j < pInput.mapData[pPos.i].size();
}

num count_skips_all(const data &pInput, const num maxDist) {
  std::unordered_map<num, num> skip_count;

  for (num i = 1; i < pInput.mapData.size() - 1; i++) {
    for (num j = 1; j < pInput.mapData[i].size() - 1; j++) {
      const cell &first = pInput.mapData[i][j];

      for (num i2 = i - maxDist; i2 <= i + maxDist; i2++) {
        for (num j2 = j - maxDist; j2 <= j + maxDist; j2++) {
          if (!valid(pInput, {i2, j2})) {
            continue;
          }

          num dist = std::abs(i2 - i) + std::abs(j2 - j);
          if (dist > maxDist) {
            continue;
          }

          const cell &second = pInput.mapData[i2][j2];

          num save = first.cost - second.cost - dist;

          if (!first.wall && !second.wall && save > 0) {
            skip_count[save]++;
          }
        }
      }
    }
  }

  num result = 0;
  for (auto iter = skip_count.begin(); iter != skip_count.end(); iter++) {
    if (iter->first >= 100) {
      result += iter->second;
    }
  }

  return result;
}

void part2(data pInput) {
  bfs(pInput);
  std::cout << "part 2: " << count_skips_all(pInput, 20) << std::endl;
}