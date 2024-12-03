#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Función para calcular el piso
int calculateFloor(const string& instructions) {
  int floor = 0;

  for (char c : instructions) {
    if (c == '(') {
      floor++;  // Subir un piso
    } else if (c == ')') {
      floor--;  // Bajar un piso
    }
  }

  return floor;
}

// Función para encontrar la posición del primer carácter que lleva a Santa al
// sótano
int findBasementPosition(const string& instructions) {
  int floor = 0;

  for (size_t i = 0; i < instructions.size(); ++i) {
    char c = instructions[i];
    if (c == '(') {
      floor++;  // Subir un piso
    } else if (c == ')') {
      floor--;  // Bajar un piso
    }

    // Verificar si Santa entra al sótano
    if (floor == -1) {
      return i + 1;  // La posición es índice + 1
    }
  }

  return -1;  // Nunca entra al sótano
}

int main() {
  string fileName = "input.txt";  // Archivo con las instrucciones
  ifstream file(fileName);

  if (!file) {
    cerr << "Error al abrir el archivo: " << fileName << endl;
    return 1;
  }

  string instructions;
  file >> instructions;  // Leer todo el contenido como una sola línea
  file.close();

  int finalFloor = calculateFloor(instructions);
  cout << "Santa llega al piso: " << finalFloor << endl;

  string instructions2;
  file >> instructions2;  // Leer todo el contenido como una sola línea
  file.close();

  int basementPosition = findBasementPosition(instructions2);

  if (basementPosition != -1) {
    cout << "La posición del primer carácter que lleva al sótano es: "
         << basementPosition << endl;
  } else {
    cout << "Santa nunca entra al sótano." << endl;
  }

  return 0;
}
