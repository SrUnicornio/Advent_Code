#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

// Función para procesar la memoria y calcular la suma de multiplicaciones
// válidas
int calculateSumOfMultiplications(const string& memory) {
  int sum = 0;

  // Expresión regular para capturar instrucciones válidas de la forma mul(X,Y)
  regex validMulRegex(R"(mul\((\d{1,3}),(\d{1,3})\))");
  smatch match;

  // Busca todas las coincidencias en la memoria corrupta
  string::const_iterator searchStart(memory.cbegin());
  while (regex_search(searchStart, memory.cend(), match, validMulRegex)) {
    // Extraer los dos números de la instrucción válida
    int x = stoi(match[1]);
    int y = stoi(match[2]);

    // Calcular el producto y añadirlo a la suma
    sum += x * y;

    // Continuar buscando desde el final de la coincidencia actual
    searchStart = match.suffix().first;
  }

  return sum;
}

// Función para procesar la memoria y calcular la suma de multiplicaciones
// válidas
int calculateSumWithConditions(const string& memory) {
  int sum = 0;
  bool enabled = true;  // Estado inicial: habilitado

  // Expresión regular para capturar instrucciones válidas
  regex instructionRegex(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");
  smatch match;

  // Busca todas las coincidencias en la memoria corrupta
  string::const_iterator searchStart(memory.cbegin());
  while (regex_search(searchStart, memory.cend(), match, instructionRegex)) {
    string matched = match[0];

    if (matched == "do()") {
      enabled = true;  // Habilitar multiplicaciones
    } else if (matched == "don't()") {
      enabled = false;  // Deshabilitar multiplicaciones
    } else if (matched.substr(0, 4) == "mul(" && enabled) {
      // Extraer los dos números de la instrucción válida
      int x = stoi(match[1]);
      int y = stoi(match[2]);

      // Calcular el producto y añadirlo a la suma si está habilitado
      sum += x * y;
    }

    // Continuar buscando desde el final de la coincidencia actual
    searchStart = match.suffix().first;
  }

  return sum;
}


int main() {
  string fileName = "input.txt";  // Archivo con la memoria corrupta
  ifstream file(fileName);

  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    return 1;
  }

  string memory((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  file.close();

  int result = calculateSumOfMultiplications(memory);
  cout << "La suma de las multiplicaciones válidas es: " << result << endl;

  result = calculateSumWithConditions(memory);
  cout << "La suma de las multiplicaciones habilitadas es: " << result << endl;

  return 0;
}
