#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct OrderRule {
  int lhs, rhs;
};

struct GameInfo {
  vector<OrderRule> order_rules;
  vector<vector<int>> updates;
};

// Función para dividir cadenas en números enteros
vector<int> splitToInt(const string& str, char delim) {
  vector<int> result;
  stringstream ss(str);
  string item;
  while (getline(ss, item, delim)) {
    try {
      result.push_back(stoi(item));
    } catch (const invalid_argument&) {
      cerr << "Error: Valor no válido en la entrada: " << item << endl;
      exit(EXIT_FAILURE);
    }
  }
  return result;
}

// Leer archivo de entrada
GameInfo readInputFile(const string& filePath) {
  ifstream file(filePath);
  if (!file.is_open()) {
    cerr << "Error: No se pudo abrir el archivo: " << filePath << endl;
    exit(EXIT_FAILURE);
  }

  GameInfo gameInfo;
  string line;
  bool readingRules = true;
  while (getline(file, line)) {
    if (line.empty()) {
      readingRules = false;
      continue;
    }
    if (readingRules) {
      auto pos = line.find('|');
      if (pos != string::npos) {
        int lhs = stoi(line.substr(0, pos));
        int rhs = stoi(line.substr(pos + 1));
        gameInfo.order_rules.push_back({lhs, rhs});
      }
    } else {
      gameInfo.updates.push_back(splitToInt(line, ','));
    }
  }

  return gameInfo;
}

// Verificar y calcular medianas
void processUpdates(GameInfo& gameInfo) {
  int sumGoodMiddles = 0;
  int sumFixedMiddles = 0;

  for (auto& update : gameInfo.updates) {
    bool isValid = true;

    for (const auto& rule : gameInfo.order_rules) {
      auto lhsIt = find(update.begin(), update.end(), rule.lhs);
      auto rhsIt = find(update.begin(), update.end(), rule.rhs);
      if (lhsIt != update.end() && rhsIt != update.end() && lhsIt > rhsIt) {
        isValid = false;
        swap(*lhsIt, *rhsIt);
      }
    }

    int median = update[(update.size() - 1) / 2];
    if (isValid) {
      sumGoodMiddles += median;
    } else {
      sumFixedMiddles += median;
    }
  }

  cout << "Suma de medianas válidas: " << sumGoodMiddles << endl;
  cout << "Suma de medianas corregidas: " << sumFixedMiddles << endl;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <ruta_del_archivo>" << endl;
    return EXIT_FAILURE;
  }

  string filePath = argv[1];
  GameInfo gameInfo = readInputFile(filePath);
  processUpdates(gameInfo);

  return 0;
}
