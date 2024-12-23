#include <bits/stdc++.h>

using namespace std;

struct Colors {
  char blanco{'w'};
  char azul{'u'};
  char negro{'b'};
  char rojo{'r'};
  char verde{'r'};
};

std::pair<std::vector<std::string>, std::vector<std::string>> readFile(
    const string &filename) {
  std::ifstream file_process{filename};

  if (!file_process.is_open()) {
    std::cerr << "Cannot open the File: " << filename << std::endl;
    return {};
  }
  std::vector<std::string> pattern;
  std::vector<std::string> design;
  std::string line{""};
  std::getline(file_process, line);

  // eliiminar todas las , dento de line
  line.erase(std::remove(line.begin(), line.end(), ','), line.end());

  // Separar la línea en palabras y agregarlas al vector
  std::istringstream iss(line);
  std::string token;
  while (iss >> token) {
    pattern.push_back(token);
  }

  while (getline(file_process, line)) {
    if (!line.empty()) {
      design.push_back(line);
    }
  }

  return std::make_pair(pattern, design);
}

std::string remove_pattern(const std::string &design,
                           const std::string &pattern) {
  std::string result = design;
  size_t pos = result.find(pattern);
  if (pos != std::string::npos) {
    result.replace(pos, pattern.length(), " ");
  }
  return result;
}
// Función para verificar si un diseño es posible utilizando los patrones
// disponibles
bool is_valid_design(const string &design, const vector<string> &towels) {
  queue<string> queue;  // Cola para realizar la búsqueda en amplitud
  unordered_set<string>
      seen;  // Conjunto para evitar procesar el mismo estado varias veces

  queue.push(design);
  seen.insert(design);

  while (!queue.empty()) {
    string current = queue.front();
    queue.pop();

    for (const string &towel : towels) {
      if (current.find(towel) == 0) {  // Si el diseño comienza con este patrón
        string new_design = current.substr(towel.size());
        if (new_design
                .empty()) {  // Si el diseño restante está vacío, es válido
          return true;
        }
        if (seen.find(new_design) ==
            seen.end()) {  // Si no se ha procesado este estado
          seen.insert(new_design);
          queue.push(new_design);
        }
      }
    }
  }
  return false;
}

// Función principal para contar cuántos diseños son posibles
int part1(const vector<string> &towels, const vector<string> &designs) {
  int result = 0;
  for (const string &design : designs) {
    if (is_valid_design(design, towels)) {
      result++;
    }
  }
  return result;
}

// Función para calcular cuántas formas hay de construir un diseño con los
// patrones disponibles
unsigned long countWays(const string &design, const vector<string> &patterns,
                        unordered_map<string, long> &cache) {
  // Si el diseño está vacío, hay exactamente una forma de construirlo (caso
  // base)
  if (design.empty()) {
    return 1;
  }

  // Si ya hemos calculado el resultado para este diseño, devolverlo del caché
  if (cache.find(design) != cache.end()) {
    return cache[design];
  }

  unsigned long count = 0;

  // Intentar construir el diseño utilizando los patrones disponibles
  for (const string &pattern : patterns) {
    if (design.find(pattern) == 0) {  // Si el diseño comienza con este patrón
      count += countWays(design.substr(pattern.size()), patterns, cache);
    }
  }

  // Almacenar el resultado en el caché antes de devolverlo
  cache[design] = count;
  return count;
}

int main() {
  std::string file{"input.txt"};

  auto [pattern, designs] = readFile(file);

  int count = part1(pattern, designs);

  unsigned long totalWays;
  unordered_map<string, long> cache;  // Caché para memoización

  for (const string &design : designs) {
    if (!is_valid_design(design, pattern)) {
      continue;
    }
    totalWays += countWays(design, pattern, cache);
  }
  std::cout << 950763269786650 - 950763269786691 << std::endl;
  std::cout << "Total de diseños válidos: " << count << std::endl;
  std::cout << "Total de formas de construir los diseños: " << totalWays
            << std::endl;
  return 0;
}