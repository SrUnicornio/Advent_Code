#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Función para verificar si un reporte es seguro
bool isSafeReport(const vector<int>& levels) {
  bool isIncreasing = true, isDecreasing = true;

  for (size_t i = 1; i < levels.size(); ++i) {
    int diff = levels[i] - levels[i - 1];

    // Verificar si la diferencia está entre 1 y 3 (inclusive)
    if (abs(diff) < 1 || abs(diff) > 3) {
      return false;
    }

    // Determinar si la secuencia es estrictamente creciente o decreciente
    if (diff > 0) {
      isDecreasing = false;
    } else if (diff < 0) {
      isIncreasing = false;
    }
  }

  // La secuencia es segura si es completamente creciente o decreciente
  return isIncreasing || isDecreasing;
}

// Función para procesar un archivo y contar los reportes seguros
int countSafeReports(const string& fileName) {
  ifstream file(fileName);
  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    return -1;
  }

  string line;
  int safeCount = 0;

  // Leer cada línea (reporte) del archivo
  while (getline(file, line)) {
    stringstream ss(line);
    vector<int> levels;
    int level;

    // Convertir la línea en un vector de enteros
    while (ss >> level) {
      levels.push_back(level);
    }

    // Verificar si el reporte es seguro
    if (isSafeReport(levels)) {
      safeCount++;
    }
  }

  file.close();
  return safeCount;
}

// Función para verificar si un reporte es seguro
bool isSafeReport2(const vector<int>& levels) {
  bool isIncreasing = true, isDecreasing = true;

  for (size_t i = 1; i < levels.size(); ++i) {
    int diff = levels[i] - levels[i - 1];

    // Verificar si la diferencia está entre 1 y 3 (inclusive)
    if (abs(diff) < 1 || abs(diff) > 3) {
      return false;
    }

    // Determinar si la secuencia es estrictamente creciente o decreciente
    if (diff > 0) {
      isDecreasing = false;
    } else if (diff < 0) {
      isIncreasing = false;
    }
  }

  // La secuencia es segura si es completamente creciente o decreciente
  return isIncreasing || isDecreasing;
}

// Función para verificar si un reporte puede ser seguro eliminando un nivel
bool isSafeWithDampener(const vector<int>& levels) {
  // Si ya es seguro, no se necesita modificar
  if (isSafeReport2(levels)) {
    return true;
  }

  // Probar eliminando cada nivel, uno por uno
  for (size_t i = 0; i < levels.size(); ++i) {
    vector<int> modifiedLevels = levels;
    modifiedLevels.erase(modifiedLevels.begin() +
                         i);  // Eliminar el nivel en la posición i

    // Verificar si la secuencia modificada es segura
    if (isSafeReport2(modifiedLevels)) {
      return true;
    }
  }

  return false;  // No es seguro incluso con el Dampener
}

// Función para procesar un archivo y contar los reportes seguros
int countSafeReportsWithDampener(const string& fileName) {
  ifstream file(fileName);
  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    return -1;
  }

  string line;
  int safeCount = 0;

  // Leer cada línea (reporte) del archivo
  while (getline(file, line)) {
    stringstream ss(line);
    vector<int> levels;
    int level;

    // Convertir la línea en un vector de enteros
    while (ss >> level) {
      levels.push_back(level);
    }

    // Verificar si el reporte es seguro con el Dampener
    if (isSafeWithDampener(levels)) {
      safeCount++;
    }
  }

  file.close();
  return safeCount;
}

int main() {
  string fileName = "input.txt";  // Nombre del archivo de entrada

  // Contar reportes seguros
  int safeReports = countSafeReports(fileName);

  if (safeReports != -1) {
    cout << "El número de reportes seguros es: " << safeReports << endl;
  }

  // Contar reportes seguros
  int safeReports2 = countSafeReportsWithDampener(fileName);

  if (safeReports2 != -1) {
    cout << "El número de reportes seguros con el Dampener es: " << safeReports2
         << endl;
  }

  return 0;
}
