#include <bits/stdc++.h>

using namespace std;

struct pair_hash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    auto h1 = hash<T1>{}(p.first);
    auto h2 = hash<T2>{}(p.second);
    return h1 ^ h2;
  }
};

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

bool inBounds(int r, int c, int aRow, int aCol) {
  return r >= 0 && r < aRow && c >= 0 && c < aCol;
}

pair<int, int> dfs1(char ochar, int r, int c, vector<vector<char>> a,
                    unordered_set<pair<int, int>, pair_hash>& explored) {
  pair<int, int> dirs[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  pair<int, int> pos = make_pair(r, c);
  pair<int, int> res = make_pair(0, 0);
  if (explored.find(pos) != explored.end() ||
      !inBounds(r, c, a.size(), a[0].size())) {
    return res;
  }
  if (a[r][c] != ochar) {
    return res;
  }
  explored.insert(pos);
  res.first++;
  for (auto dir : dirs) {
    int nr = r + dir.first;
    int nc = c + dir.second;
    if (!inBounds(nr, nc, a.size(), a[0].size()) || a[nr][nc] != ochar) {
      res.second++;
    } else {
      pair<int, int> nres = dfs1(ochar, nr, nc, a, explored);
      res.first += nres.first;
      res.second += nres.second;
    }
  }
  return res;
}

long long part1(vector<vector<char>>& a) {
  long long s = 0;
  unordered_set<pair<int, int>, pair_hash> explored;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      pair<int, int> p = make_pair(i, j);
      if (explored.find(p) == explored.end()) {
        pair<int, int> res = dfs1(a[i][j], i, j, a, explored);
        s += (res.first * res.second);
      }
    }
  }
  return s;
}

int dfs2(char ochar, int r, int c, vector<vector<char>> a,
         unordered_set<pair<int, int>, pair_hash>& explored,
         unordered_set<pair<int, int>, pair_hash>& area) {
  pair<int, int> dirs[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  pair<int, int> pos = make_pair(r, c);
  int res = 0;
  if (explored.find(pos) != explored.end() ||
      !inBounds(r, c, a.size(), a[0].size())) {
    return res;
  }
  if (a[r][c] != ochar) {
    return res;
  }
  explored.insert(pos);
  area.insert(pos);
  res++;
  for (auto dir : dirs) {
    int nr = r + dir.first;
    int nc = c + dir.second;
    if (inBounds(nr, nc, a.size(), a[0].size()) && a[nr][nc] == ochar) {
      res += dfs2(ochar, nr, nc, a, explored, area);
    }
  }
  return res;
}

int calculate_sides(char ochar,
                    unordered_set<pair<int, int>, pair_hash>& perimeter,
                    vector<vector<char>>& a) {
  int res = 0;
  for (auto p : perimeter) {
    int r = p.first;
    int c = p.second;

    bool left =
        !inBounds(r, c - 1, a.size(), a[0].size()) || a[r][c - 1] != ochar;
    bool right =
        !inBounds(r, c + 1, a.size(), a[0].size()) || a[r][c + 1] != ochar;
    bool up =
        !inBounds(r - 1, c, a.size(), a[0].size()) || a[r - 1][c] != ochar;
    bool down =
        !inBounds(r + 1, c, a.size(), a[0].size()) || a[r + 1][c] != ochar;

    bool upleft = !inBounds(r - 1, c - 1, a.size(), a[0].size()) ||
                  a[r - 1][c - 1] != ochar;
    bool upright = !inBounds(r - 1, c + 1, a.size(), a[0].size()) ||
                   a[r - 1][c + 1] != ochar;
    bool downleft = !inBounds(r + 1, c - 1, a.size(), a[0].size()) ||
                    a[r + 1][c - 1] != ochar;
    bool downright = !inBounds(r + 1, c + 1, a.size(), a[0].size()) ||
                     a[r + 1][c + 1] != ochar;

    if (left && up && !upleft) {
      res++;
    }
    if (right && up && !upright) {
      res++;
    }
    if (left && down && !downleft) {
      res++;
    }
    if (right && down && !downright) {
      res++;
    }

    if (!left && !up && upleft) {
      res++;
    }
    if (!right && !up && upright) {
      res++;
    }
    if (!left && !down && downleft) {
      res++;
    }
    if (!right && !down && downright) {
      res++;
    }

    if (left && up && upleft) {
      res++;
    }
    if (right && up && upright) {
      res++;
    }
    if (left && down && downleft) {
      res++;
    }
    if (right && down && downright) {
      res++;
    }
  }

  return res;
}

long long part2(vector<vector<char>>& a) {
  long long s = 0;
  unordered_set<pair<int, int>, pair_hash> explored;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      pair<int, int> p = make_pair(i, j);
      if (explored.find(p) == explored.end()) {
        unordered_set<pair<int, int>, pair_hash> area;
        s += (dfs2(a[i][j], i, j, a, explored, area) *
              calculate_sides(a[i][j], area, a));
      }
    }
  }
  return s;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  vector<vector<char>> d;

  ifstream file("input.txt");
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      vector<char> a;
      for (auto x : line) {
        a.push_back(x);
      }
      d.push_back(a);
    }
  } else {
    cout << "Unable to read from input file\n";
    return 1;
  }

  cout << "Part 1: " << part1(d) << endl;
  cout << "Part 2: " << part2(d) << endl;

  return 0;
}