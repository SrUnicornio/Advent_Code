#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

// Estructura simple para representar posiciones en 2D
struct Vec2d {
  int x, y;
};

// Operador de igualdad para Vec2d
bool operator==(const Vec2d& left, const Vec2d& right) {
  return left.x == right.x && left.y == right.y;
}

// Hash personalizado para Vec2d
template <>
struct std::hash<Vec2d> {
  std::size_t operator()(const Vec2d& vec) const {
    return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
  }
};

// Función que simula el movimiento del guardia y calcula posiciones visitadas
int simulateGuardPath(const std::vector<std::string>& map, Vec2d guardPos,
                      Vec2d guardDir) {
  // Conjunto para almacenar posiciones visitadas
  std::unordered_set<Vec2d> positionsVisited;

  // Simulación del movimiento del guardia
  while (true) {
    positionsVisited.insert(guardPos);

    // Verifica si el guardia está saliendo del mapa
    int newX = guardPos.x + guardDir.x;
    int newY = guardPos.y + guardDir.y;
    if (newX < 0 || newX >= map[0].size() || newY < 0 || newY >= map.size()) {
      break;
    }

    // Si hay un obstáculo, gira a la derecha
    if (map[newY][newX] == '#') {
      guardDir = {-guardDir.y, guardDir.x};  // Giro a la derecha
    } else {
      // De lo contrario, avanza
      guardPos.x = newX;
      guardPos.y = newY;
    }
  }

  // Retorna el número de posiciones visitadas
  return positionsVisited.size();
}

// Función para leer el mapa y encontrar la posición inicial del guardia
std::pair<std::vector<std::string>, Vec2d> readMapFromFile(
    const std::string& filename) {
  std::ifstream ifs{filename};
  if (!ifs.is_open()) {
    throw std::runtime_error("Error al abrir el archivo");
  }

  std::vector<std::string> map;
  Vec2d guardPos = {};
  int row = 0;
  for (std::string line; std::getline(ifs, line); ++row) {
    for (int col = 0; col < line.size(); ++col) {
      if (line[col] == '^') {
        guardPos = {col, row};
      }
    }
    map.push_back(line);
  }

  return {map, guardPos};
}

// Función que verifica si el guardia está en los bordes del mapa o ha dado más
// de 10000 pasos
bool isOutOfBoundsOrTooManySteps(int i, int j,
                                 const std::vector<std::string>& matrix,
                                 int steps) {
  return (i == 0 || j == 0 || i == matrix[0].size() - 1 ||
          j == matrix.size() - 1 || steps >= 10000);
}

// Función que mueve al guardia en una dirección dada
bool moveInDirection(int& i, int& j, int di, int dj,
                     const std::vector<std::string>& matrix, int& steps) {
  while (!isOutOfBoundsOrTooManySteps(i, j, matrix, steps) &&
         matrix[j + dj][i + di] != '#') {
    i += di;
    j += dj;
    ++steps;
    if (isOutOfBoundsOrTooManySteps(i, j, matrix, steps)) {
      return false;
    }
  }
  return true;
}

// Función que simula el movimiento del guardia y calcula las posiciones
// visitadas
int simulateGuardPath(const std::vector<std::string>& matrix, int initial_i,
                      int initial_j) {
  int i = initial_i, j = initial_j, steps = 0;

  while (!isOutOfBoundsOrTooManySteps(i, j, matrix, steps)) {
    // Movimiento hacia arriba
    if (!moveInDirection(i, j, 0, -1, matrix, steps)) break;

    // Movimiento hacia la derecha
    if (!moveInDirection(i, j, 1, 0, matrix, steps)) break;

    // Movimiento hacia abajo
    if (!moveInDirection(i, j, 0, 1, matrix, steps)) break;

    // Movimiento hacia la izquierda
    if (!moveInDirection(i, j, -1, 0, matrix, steps)) break;
  }

  return steps;
}

int guradLoop(const std::vector<std::string> map, const Vec2d guardPos) {
  // Dirección inicial del guardia (hacia arriba)
  int initial_i = guardPos.x;
  int initial_j = guardPos.y;
  int result = 0;
  std::vector<std::string> matrix_copy;
  for (int j = 0; j < map.size(); j++) {
    for (int i = 0; i < map[0].size(); i++) {
      matrix_copy = map;  // Copiar el mapa original
      if (matrix_copy[j][i] == '.') {
        matrix_copy[j][i] = '#';  // Poner un obstáculo en la posición vacía
        if (simulateGuardPath(matrix_copy, initial_i, initial_j) == 10000) {
          result++;  // Contar las posiciones válidas
        }
      }
    }
  }
  return result;
}

int main() {
  try {
    // Leer el mapa desde el archivo y encontrar la posición inicial del guardia
    auto [map, guardPos] = readMapFromFile("input.txt");

    // Dirección inicial del guardia (hacia arriba)
    Vec2d guardDir = {0, -1};

    // Simular el movimiento del guardia
    int positionsVisitedByGuard = simulateGuardPath(map, guardPos, guardDir);

    // Mostrar el resultado
    std::cout << "Distinct positions visited by the guard: "
              << positionsVisitedByGuard << std::endl;

    // Para verificar las posiciones posibles para convertir puntos vacíos a
    // obstáculos
    int result = guradLoop(map, guardPos);
    std::cout << "Total valid positions after modifying map: " << result
              << std::endl;

  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return -1;
  }

  return 0;
}