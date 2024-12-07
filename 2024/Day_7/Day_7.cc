#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

// Helper function to split a line into target value and numbers
std::pair<long, std::vector<long>> parseEquation(const std::string& line) {
  std::istringstream iss(line);
  long target;
  char colon;
  if (!(iss >> target >> colon) || colon != ':') {
    throw std::runtime_error("Formato de línea inválido: " + line);
  }

  std::vector<long> numbers;
  long num;
  while (iss >> num) {
    numbers.push_back(num);
  }

  if (numbers.empty()) {
    throw std::runtime_error("No hay números después del objetivo en: " + line);
  }

  return {target, numbers};
}

// Function to evaluate all combinations of operators iteratively
bool isValidEquation(long target, const std::vector<long>& numbers) {
  size_t n = numbers.size() - 1;
  if (numbers.size() == 1) {
    return numbers[0] == target;
  }

  // Total combinations of operators: 2^(numbers.size() - 1)
  size_t totalCombinations = 1 << n;

  for (size_t mask = 0; mask < totalCombinations; ++mask) {
    long result = numbers[0];

    for (size_t i = 0; i < n; ++i) {
      if (mask & (1 << i)) {  // Operator '+'
        result += numbers[i + 1];
      } else {  // Operator '*'
        result *= numbers[i + 1];
      }
    }

    if (result == target) {
      return true;
    }
  }
  return false;
}

// Function to concatenate two numbers
long concatenate(long left, long right) {
  std::string leftStr = std::to_string(left);
  std::string rightStr = std::to_string(right);
  std::string concatenated = leftStr + rightStr;

  // Convert back to long long, handle overflow case
  try {
    return std::stoll(concatenated);
  } catch (...) {
    return -1;  // Return a negative value to indicate invalid concatenation
  }
}

// Function to evaluate expressions iteratively with all operator combinations
bool isValidEquation2(long target, const std::vector<long>& numbers) {
  size_t n = numbers.size() - 1;
  if (numbers.size() == 1) {
    return numbers[0] == target;
  }

  // Total combinations of operators: 3^(numbers.size() - 1)
  size_t totalCombinations = pow(3, n);

  for (size_t mask = 0; mask < totalCombinations; ++mask) {
    long long result = numbers[0];
    size_t currentMask = mask;

    for (size_t i = 0; i < n; ++i) {
      int operatorChoice = currentMask % 3;  // 0: +, 1: *, 2: ||
      currentMask /= 3;

      if (operatorChoice == 0) {
        result += numbers[i + 1];
      } else if (operatorChoice == 1) {
        result *= numbers[i + 1];
      } else if (operatorChoice == 2) {
        long long concatenated = concatenate(result, numbers[i + 1]);
        if (concatenated == -1) {
          result = -1;  // Handle invalid concatenation
          break;
        }
        result = concatenated;
      }

      // Short-circuit evaluation if result exceeds target
      if (result > target) {
        break;
      }
    }

    if (result == target) {
      return true;
    }
  }
  return false;
}

int main() {
  std::ifstream inputFile("input.txt");  // Open the input file
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open the file 'input.txt'." << std::endl;
    return 1;
  }

  std::string line;
  long totalCalibrationResult = 0;
  long totalCalibrationResult2 = 0;

      try {
    // Read file line by line
    while (std::getline(inputFile, line)) {
      if (line.empty()) {
        continue;  // Skip empty lines
      }

      auto [target, numbers] = parseEquation(line);

      // Skip very large equations
      if (numbers.size() > 15) {
        std::cerr << "Skipping line due to excessive size: " << line
                  << std::endl;
        continue;
      }

      if (isValidEquation(target, numbers)) {
        totalCalibrationResult += target;
      }

      if (isValidEquation2(target, numbers)) {
        totalCalibrationResult2 += target;
      }
    }

    inputFile.close();  // Close the file

    // Display the final result
    std::cout << "Total calibration result: " << totalCalibrationResult
              << std::endl;

    std::cout << "Total calibration result2: " << totalCalibrationResult2
              << std::endl;

  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    inputFile.close();
    return 1;
  }

  return 0;
}
