#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

// Función para leer las columnas de un archivo y separarlas en dos listas
void readColumnsFromFile(const string& fileName, vector<int>& leftList,
                         vector<int>& rightList) {
  ifstream file(fileName);
  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    exit(1);
  }

  int leftValue, rightValue;
  while (file >> leftValue >> rightValue) {
    leftList.push_back(leftValue);
    rightList.push_back(rightValue);
  }
  file.close();
}


int calculateSimilarityScore(const vector<int>& leftList,
                             const vector<int>& rightList) {
  // Crear un mapa para contar la frecuencia de cada número en la lista derecha
  unordered_map<int, int> frequencyMap;
  for (int num : rightList) {
    frequencyMap[num]++;
  }

  // Calcular el puntaje de similitud
  int similarityScore = 0;
  for (int num : leftList) {
    similarityScore += num * frequencyMap[num];
  }

  return similarityScore;
}

// Función para calcular la distancia total entre dos listas
int calculateTotalDistance(vector<int>& leftList, vector<int>& rightList) {
  // Ordenar ambas listas en orden ascendente
  sort(leftList.begin(), leftList.end());
  sort(rightList.begin(), rightList.end());

  // Calcular la distancia total
  int totalDistance = 0;
  for (size_t i = 0; i < leftList.size(); ++i) {
    totalDistance += abs(leftList[i] - rightList[i]);
  }

  return totalDistance;
}

int main() {
  string fileName = "input.txt";  // Nombre del archivo de entrada
  vector<int> leftList, rightList;

  // Leer las columnas del archivo
  readColumnsFromFile(fileName, leftList, rightList);

  // Verificar que ambas listas tengan el mismo tamaño
  if (leftList.size() != rightList.size()) {
    cerr << "Error: Las listas deben tener el mismo tamaño." << endl;
    return 1;
  }

  // Calcular la distancia total
  int totalDistance = calculateTotalDistance(leftList, rightList);

  // Mostrar el resultado
  cout << "La distancia total entre las listas es: " << totalDistance << endl;

  // Calcular el puntaje de similitud
  int similarityScore = calculateSimilarityScore(leftList, rightList);

  // Mostrar el resultado
  cout << "El puntaje de similitud entre las listas es: " << similarityScore
       << endl;

  return 0;
}
