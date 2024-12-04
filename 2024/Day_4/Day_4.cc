#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Función para verificar si se encuentra la palabra en una dirección dada
bool isWordFound(const vector<string>& grid, int x, int y, int dx, int dy,
                 const string& word) {
  int n = grid.size();
  int m = grid[0].size();
  int wordLength = word.size();

  for (int i = 0; i < wordLength; ++i) {
    int nx = x + i * dx;
    int ny = y + i * dy;

    // Verificar límites
    if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] != word[i]) {
      return false;
    }
  }
  return true;
}

// Función para contar todas las ocurrencias de la palabra
int countWordOccurrences(const vector<string>& grid, const string& word) {
  int n = grid.size();
  int m = grid[0].size();
  int count = 0;

  // Todas las 8 direcciones posibles (dx, dy)
  vector<pair<int, int>> directions = {
      {0, 1},    // Horizontal (derecha)
      {0, -1},   // Horizontal (izquierda)
      {1, 0},    // Vertical (abajo)
      {-1, 0},   // Vertical (arriba)
      {1, 1},    // Diagonal (abajo-derecha)
      {-1, -1},  // Diagonal (arriba-izquierda)
      {1, -1},   // Diagonal (abajo-izquierda)
      {-1, 1}    // Diagonal (arriba-derecha)
  };

  // Iterar por cada celda en la cuadrícula
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      // Verificar todas las direcciones
      for (auto [dx, dy] : directions) {
        if (isWordFound(grid, i, j, dx, dy, word)) {
          count++;
        }
      }
    }
  }

  return count;
}

// Función para verificar si una posición contiene "MAS" en una dirección
// diagonal
bool isMAS(const vector<string>& grid, int x, int y, int dx, int dy) {
  int n = grid.size();
  int m = grid[0].size();

  string mas = "MAS";
  string reversedMas = "SAM";

  string diagonal;
  for (int i = 0; i < 3; ++i) {
    int nx = x + i * dx;
    int ny = y + i * dy;

    // Verificar límites
    if (nx < 0 || nx >= n || ny < 0 || ny >= m) {
      return false;
    }
    diagonal += grid[nx][ny];
  }

  // Verificar si coincide con "MAS" o "SAM"
  return (diagonal == mas || diagonal == reversedMas);
}

// Función para contar todas las ocurrencias de X-MAS
int countXMAS(const vector<string>& grid) {
  int n = grid.size();
  int m = grid[0].size();
  int count = 0;

  // Iterar por cada celda que puede ser el centro de una "X"
  for (int i = 1; i < n - 1; ++i) {
    for (int j = 1; j < m - 1; ++j) {
      // Verificar las dos diagonales que forman una "X"
      if (isMAS(grid, i - 1, j - 1, 1, 1) && isMAS(grid, i + 1, j - 1, -1, 1)) {
        count++;
      }
    }
  }

  return count;
}

int main() {
  string fileName = "input.txt";  // Nombre del archivo de entrada
  ifstream file(fileName);

  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    return 1;
  }

  vector<string> grid;
  string line;

  // Leer la cuadrícula desde el archivo
  while (getline(file, line)) {
    grid.push_back(line);
  }
  file.close();

  string word = "XMAS";  // Palabra a buscar
  int result = countWordOccurrences(grid, word);

  cout << "La palabra " << word << " aparece " << result
       << " veces en la cuadrícula." << endl;

  result = countXMAS(grid);

  cout << "El patrón X-MAS aparece " << result << " veces en la cuadrícula."
       << endl;
  return 0;
}


