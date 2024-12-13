#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Verifica si una posición está dentro de los límites de la matriz
bool isWithinBounds(int row, int col, int numRows, int numCols) {
  return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

// DFS para contar las posiciones accesibles de altura 9 para la Parte 1
pair<int, set<pair<int, int>>> depthFirstSearchPart1(
    int row, int col, const vector<vector<int>>& heightMap,
    set<pair<int, int>>& visited) {
  static const vector<pair<int, int>> directions = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  if (!isWithinBounds(row, col, heightMap.size(), heightMap[0].size()) ||
      visited.count({row, col})) {
    return {0, visited};
  }

  visited.insert({row, col});
  if (heightMap[row][col] == 9) {
    return {1, visited};
  }
  
  // Recorre las direcciones adyacentes para encontrar la siguiente altura en el sendero
  int count = 0;
  for (const auto& dir : directions) {
    int newRow = row + dir.first;
    int newCol = col + dir.second;

    if (isWithinBounds(newRow, newCol, heightMap.size(), heightMap[0].size()) &&
        heightMap[newRow][newCol] == heightMap[row][col] + 1) {
      auto result = depthFirstSearchPart1(newRow, newCol, heightMap, visited);
      count += result.first;
      visited = result.second;
    }
  }

  return {count, visited};
}

// Resuelve la Parte 1 iterando sobre el mapa de alturas y contando los senderos
int solvePart1(const vector<vector<int>>& heightMap) {
  int totalScore = 0;

  for (int row = 0; row < heightMap.size(); ++row) {
    for (int col = 0; col < heightMap[row].size(); ++col) {
      if (heightMap[row][col] == 0) {  // Trailhead found.
        set<pair<int, int>> visited;
        auto result = depthFirstSearchPart1(row, col, heightMap, visited);
        totalScore += result.first;
      }
    }
  }

  return totalScore;
}

// DFS para contar las posiciones accesibles de altura 9 para la Parte 2
int depthFirstSearchPart2(int row, int col,
                          const vector<vector<int>>& heightMap) {
  static const vector<pair<int, int>> directions = {
      {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  if (!isWithinBounds(row, col, heightMap.size(), heightMap[0].size())) {
    return 0;
  }

  if (heightMap[row][col] == 9) {
    return 1;
  }

  int count = 0;
  // Recorre las direcciones adyacentes para encontrar la siguiente altura en el sendero
  for (const auto& dir : directions) {
    int newRow = row + dir.first;
    int newCol = col + dir.second;

    if (isWithinBounds(newRow, newCol, heightMap.size(), heightMap[0].size()) &&
        heightMap[newRow][newCol] == heightMap[row][col] + 1) {
      count += depthFirstSearchPart2(newRow, newCol, heightMap);
    }
  }

  return count;
}

// Resuelve la Parte 2 iterando sobre el mapa de alturas y contando los senderos
int solvePart2(const vector<vector<int>>& heightMap) {
  int totalScore = 0;

  for (int row = 0; row < heightMap.size(); ++row) {
    for (int col = 0; col < heightMap[row].size(); ++col) {
      if (heightMap[row][col] == 0) {  // Trailhead found.
        totalScore += depthFirstSearchPart2(row, col, heightMap);
      }
    }
  }

  return totalScore;
}

int main() {
  vector<vector<int>> heightMap;
  ifstream inputFile("input.txt");

  if (inputFile.is_open()) {
    string line;
    while (getline(inputFile, line)) {
      vector<int> row;
      for (char ch : line) {
        row.push_back(ch - '0');  // Convert character to integer.
      }
      heightMap.push_back(row);
    }
  } else {
    cerr << "Error: Unable to open input file." << endl;
    return 1;
  }

  cout << "Part 1: " << solvePart1(heightMap) << endl;
  cout << "Part 2: " << solvePart2(heightMap) << endl;

  return 0;
}
