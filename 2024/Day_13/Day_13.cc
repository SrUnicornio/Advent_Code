#include <bits/stdc++.h>

using namespace std;

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

long long part1(vector<vector<pair<int, int>>>& a) {
  long long s = 0;
  for (auto v : a) {
    long long xc1 = v[0].first;
    long long xc2 = v[1].first;
    long long yc1 = v[0].second;
    long long yc2 = v[1].second;
    long long targetx = v[2].first;
    long long targety = v[2].second;

    long long a_press =
        (targety * xc2 - targetx * yc2) / (yc1 * xc2 - yc2 * xc1);
    long long b_press = (targetx - a_press * xc1) / xc2;

    if (a_press <= 100 && b_press <= 100 &&
        (a_press * xc1 + b_press * xc2 == targetx) &&
        (a_press * yc1 + b_press * yc2 == targety)) {
      s += (3 * a_press + b_press);
    }
  }
  return s;
}

long long part2(vector<vector<pair<int, int>>>& a) {
  long long s = 0;
  for (auto v : a) {
    long long xc1 = v[0].first;
    long long xc2 = v[1].first;
    long long yc1 = v[0].second;
    long long yc2 = v[1].second;
    long long targetx = v[2].first + pow(10, 13);
    long long targety = v[2].second + pow(10, 13);

    long long a_press =
        (targety * xc2 - targetx * yc2) / (yc1 * xc2 - yc2 * xc1);
    long long b_press = (targetx - a_press * xc1) / xc2;

    if ((a_press * xc1 + b_press * xc2 == targetx) &&
        (a_press * yc1 + b_press * yc2 == targety)) {
      s += (3 * a_press + b_press);
    }
  }
  return s;
}

vector<vector<pair<int, int>>> readFile(string filename) {
  vector<vector<pair<int, int>>> d;
  vector<pair<int, int>> a;

  ifstream file(filename);
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      if (line == "") {
        d.push_back(a);
        a.clear();
      } else if (line.find("Button") != string::npos) {
        vector<string> splitLine = splitString(line, ' ');
        int x = stoi(splitString(splitLine[2], '+')[1]);
        int y = stoi(splitString(splitLine[3], '+')[1]);
        a.push_back(make_pair(x, y));
      } else {
        vector<string> splitLine = splitString(line, ' ');
        string xstr = splitString(splitLine[1], '=')[1];
        xstr.pop_back();
        int targetx = stoi(xstr);
        int targety = stoi(splitString(splitLine[2], '=')[1]);
        a.push_back(make_pair(targetx, targety));
      }
    }
    d.push_back(a);
  } else {
    cout << "Unable to read from input file\n";
    return d;
  }
  return d;
}

int main() {
  vector<vector<pair<int, int>>> d;

  d = readFile("input.txt");

  cout << "Part 1: " << part1(d) << endl;
  cout << "Part 2: " << part2(d) << endl;

  return 0;
}