#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

// Divide una cadena en partes según el delimitador especificado
vector<string> splitString(const string& str, char delimiter) {
  vector<string> result;
  string current = "";
  for (char ch : str) {
    if (ch == delimiter) {
      if (!current.empty()) {
        result.push_back(current);
        current = "";
      }
    } else {
      current += ch;
    }
  }
  if (!current.empty()) {
    result.push_back(current);
  }
  return result;
}

// Construye la estructura inicial de archivos y espacios
vector<pair<vector<string>, bool>> buildFileStructure(
    const vector<char>& inputData) {
  vector<pair<vector<string>, bool>> fileStructure;

  for (size_t i = 0; i < inputData.size(); ++i) {
    bool isFile = false;
    vector<string> currentData;

    for (int j = 0; j < inputData[i] - '0'; ++j) {
      if (i % 2 == 0) {
        isFile = true;
        currentData.push_back(to_string(i / 2));
      } else {
        string temp(1, inputData[i]);
        vector<string> tempData = {"." + temp};
        fileStructure.push_back(make_pair(tempData, false));
      }
    }

    if (isFile) {
      fileStructure.push_back(make_pair(currentData, false));
    }
  }

  return fileStructure;
}

// Ajusta la estructura de archivos para distribuir los datos en espacios
// disponibles
void distributeDataInSpaces(vector<pair<vector<string>, bool>>& fileStructure) {
  int index = fileStructure.size() - 1;
  while (index >= 0) {
    if (fileStructure[index].first[0][0] != '.') {
      for (int j = 0; j < index; ++j) {
        if (fileStructure[j].first[0][0] == '.' &&
            (fileStructure[j].first[0][1] - '0') >=
                fileStructure[index].first.size() &&
            !fileStructure[index].second) {
          int spaceSize = fileStructure[j].first[0][1] - '0';
          vector<string> replacement;
          int count = 0;

          for (size_t k = 0; k < fileStructure[index].first.size(); ++k) {
            vector<string> temp;
            temp.push_back(fileStructure[index].first[k]);
            fileStructure[j + k] = make_pair(temp, true);
            ++count;

            for (size_t l = k + 1; l < spaceSize; ++l) {
              fileStructure[j + l].first[0][1] = (spaceSize - count) + '0';
            }
          }

          for (size_t k = 0; k < fileStructure[index].first.size(); ++k) {
            replacement.push_back("." +
                                  to_string(fileStructure[index].first.size()));
          }
          fileStructure[index] = make_pair(replacement, false);
          break;
        }
      }
    }
    --index;
  }
}

// Calcula el puntaje basado en la estructura final
long long calculateScoreFromStructure(
    const vector<pair<vector<string>, bool>>& fileStructure) {
  long long score = 0;
  long long count = 0;

  for (const auto& entry : fileStructure) {
    if (entry.first[0][0] != '.') {
      for (const auto& element : entry.first) {
        score += (stoll(element) * count);
        ++count;
      }
    } else {
      int spaceCount = 0;
      for (const auto& element : entry.first) {
        if (element.find(".") != string::npos) {
          ++spaceCount;
        }
      }
      count += spaceCount;
    }
  }

  return score;
}

// Calcula el puntaje para la parte 1
long long calculatePart1Score(const vector<char>& inputData) {
  long long score = 0;
  vector<string> fileData;

  for (size_t i = 0; i < inputData.size(); ++i) {
    for (int j = 0; j < inputData[i] - '0'; ++j) {
      if (i % 2 == 0) {
        fileData.push_back(to_string(i / 2));
      } else {
        fileData.push_back(".");
      }
    }
  }

  size_t index = 0;
  while (index < fileData.size()) {
    if (fileData[index] == ".") {
      if (fileData.back() != ".") {
        fileData[index] = fileData.back();
      } else {
        --index;
      }
      fileData.pop_back();
    }
    ++index;
  }

  for (size_t i = 0; i < fileData.size(); ++i) {
    score += (i * stoi(fileData[i]));
  }

  return score;
}

// Calcula el puntaje para la parte 2
long long calculatePart2Score(const vector<char>& inputData) {
  vector<pair<vector<string>, bool>> fileStructure =
      buildFileStructure(inputData);
  distributeDataInSpaces(fileStructure);
  return calculateScoreFromStructure(fileStructure);
}

int main() {
  vector<char> inputData;

  // Leer datos del archivo de entrada
  ifstream inputFile("input.txt");
  if (inputFile.is_open()) {
    string line;
    while (getline(inputFile, line)) {
      for (char ch : line) {
        inputData.push_back(ch);
      }
    }
  } else {
    cerr << "No se pudo leer el archivo de entrada\n";
    return 1;
  }

  // Calcular y mostrar los resultados
  cout << "Parte 1: " << calculatePart1Score(inputData) << endl;
  cout << "Parte 2: " << calculatePart2Score(inputData) << endl;

  return 0;
}
