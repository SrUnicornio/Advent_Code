#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Función que lee el mapa desde el archivo de entrada.
vector<vector<char>> readMapFromFile(const string& input_filename) {
  ifstream inputFile(input_filename);
  vector<vector<char>> map;
  string line;

  if (!inputFile.is_open()) {
    cerr << "Error abriendo el archivo." << endl;
    return map;
  }

  while (getline(inputFile, line)) {
    vector<char> row(line.begin(), line.end());
    map.push_back(row);
  }

  inputFile.close();
  return map;
}

// Función para almacenar las posiciones de las antenas por frecuencia.
unordered_map<char, vector<pair<int, int>>> getAntennas(
    const vector<vector<char>>& map) {
  unordered_map<char, vector<pair<int, int>>> antennas;

  for (size_t i = 0; i < map.size(); ++i) {
    for (size_t j = 0; j < map[i].size(); ++j) {
      if (isalnum(map[i][j])) {
        antennas[map[i][j]].push_back({i, j});
      }
    }
  }

  return antennas;
}

// Calcula la diferencia entre las posiciones de dos antenas.
pair<int, int> calculateDifference(pair<int, int> first_antenna,
                                   pair<int, int> second_antenna) {
  int i_diff = second_antenna.first - first_antenna.first;
  int j_diff = second_antenna.second - first_antenna.second;
  return {i_diff, j_diff};
}

// Verifica si un par de antinodos está dentro de los límites del mapa.
bool isValidAntinode(pair<int, int> antinode, const vector<vector<char>>& map) {
  return antinode.first >= 0 && antinode.first < map.size() &&
         antinode.second >= 0 && antinode.second < map[0].size();
}

// Verifica si un punto está dentro de los límites del mapa.
bool inBounds(const pair<int, int>& point, const vector<vector<char>>& map) {
  return point.first >= 0 && static_cast<size_t>(point.first) < map.size() &&
         point.second >= 0 && static_cast<size_t>(point.second) < map[0].size();
}

// Función unificada para generar antinodos. Usa un parámetro `usePart1` para
// elegir la lógica.
vector<pair<int, int>> getAntinodes(pair<int, int> first_antenna,
                                    pair<int, int> second_antenna,
                                    pair<int, int> diff,
                                    const vector<vector<char>>& map,
                                    bool usePart1) {
  vector<pair<int, int>> antinodes;

  if (usePart1) {
    // Lógica de la Parte 1: Un solo par de antinodos por cada par de antenas.
    pair<int, int> first_antinode = {(diff.first * -1) + first_antenna.first,
                                     (diff.second * -1) + first_antenna.second};
    pair<int, int> second_antinode = {diff.first + second_antenna.first,
                                      diff.second + second_antenna.second};

    if (isValidAntinode(first_antinode, map)) {
      antinodes.push_back(first_antinode);
    }

    if (isValidAntinode(second_antinode, map)) {
      antinodes.push_back(second_antinode);
    }
  } else {
    // Lógica de la Parte 2: Genera antinodos mientras estén dentro de los
    // límites.
    pair<int, int> points[3] = {first_antenna, second_antenna, first_antenna};

    while (true) {
      // Actualiza las posiciones de los puntos en las direcciones calculadas.
      points[0] = {(diff.first * -1) + points[0].first,
                   (diff.second * -1) + points[0].second};
      points[1] = {diff.first + points[1].first,
                   diff.second + points[1].second};
      points[2] = {diff.first + points[2].first,
                   diff.second + points[2].second};

      bool allOutOfBounds = true;
      for (const auto& point : points) {
        if (inBounds(point, map)) {
          antinodes.push_back(point);
          allOutOfBounds = false;
        }
      }

      // Si todos los puntos están fuera de los límites, terminamos.
      if (allOutOfBounds) {
        break;
      }
    }
  }

  return antinodes;
}

// Función principal para calcular los antinodos.
set<pair<int, int>> calculateAntinodes(
    const unordered_map<char, vector<pair<int, int>>>& antennas,
    const vector<vector<char>>& map, bool part1) {
  set<pair<int, int>> all_antinodes;

  // Iteramos sobre cada antena y sus posiciones.
  for (const auto& [antenna, positions] : antennas) {
    if (!part1) {
      // Si hay al menos dos antenas de la misma frecuencia, todas las
      // posiciones son antinodos
      if (positions.size() > 1) {
        for (const auto& pos : positions) {
          all_antinodes.insert(pos);  // La propia antena también es un antinode
        }
      }
    }
    for (size_t i = 0; i < positions.size(); ++i) {
      for (size_t j = 0; j < positions.size(); ++j) {
        if (positions[i] == positions[j]) {
          continue;
        }

        // Calculamos la diferencia entre las posiciones de las antenas.
        pair<int, int> diff = calculateDifference(positions[i], positions[j]);

        // Obtenemos los antinodos generados por estas antenas.
        vector<pair<int, int>> antinodes =
            getAntinodes(positions[i], positions[j], diff, map, part1);

        // Insertamos los antinodos únicos en el conjunto.
        all_antinodes.insert(antinodes.begin(), antinodes.end());
      }
    }
  }
  return all_antinodes;
}

// Función principal.
int main() {
  string input_filename = "input.txt";

  // Leer el mapa desde el archivo.
  vector<vector<char>> map = readMapFromFile(input_filename);

  // Obtener las antenas del mapa.
  unordered_map<char, vector<pair<int, int>>> antennas = getAntennas(map);

  // Calcular todos los antinodos únicos.
  set<pair<int, int>> all_antinodes = calculateAntinodes(antennas, map, true);

  // Mostrar el total de ubicaciones únicas de antinodos.
  cout << "Total unique antinode locations: " << all_antinodes.size() << endl;

  // Calcular todos los antinodos únicos.
  set<pair<int, int>> all_antinodes2 = calculateAntinodes(antennas, map, false);

  // Mostrar el total de ubicaciones únicas de antinodos.
  cout << "Total unique antinode locations: " << all_antinodes2.size() << endl;
  return 0;
}
