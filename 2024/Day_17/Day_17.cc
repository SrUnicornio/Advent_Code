#include <bits/stdc++.h>
using namespace std;

unordered_map<string, long long> registers;
vector<long long> program;
vector<long long> valid;

void readFile(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Cannot open the file: " << filename << endl;
    exit(1);
  }

  string line;
  while (getline(file, line)) {
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    if (line.find("Register") == 0) {
      size_t pos = line.find(":");
      string key = line.substr(9, pos - 9);
      long long value = stoll(line.substr(pos + 1));
      registers[key] = value;
    } else if (line.find("Program") == 0) {
      size_t pos = line.find(":");
      stringstream ss(line.substr(pos + 1));
      long long num;
      while (ss >> num) {
        program.push_back(num);
        if (ss.peek() == ',') ss.ignore();
      }
    }
  }

  file.close();
}

long long resolveOperand(long long operand,
                         const unordered_map<string, long long> &registers) {
  if (operand == 4) return registers.at("A");
  if (operand == 5) return registers.at("B");
  if (operand == 6) return registers.at("C");
  return operand;
}

vector<long long> executeProgram(unordered_map<string, long long> registers,
                                 const vector<long long> &program) {
  long long pos = 0;
  vector<long long> output;

  while (pos >= 0 && pos < program.size()) {
    long long opcode = program[pos];
    long long operand = resolveOperand(program[pos + 1], registers);

    switch (opcode) {
      case 0:  // adv
        registers["A"] /= (1LL << operand);
        pos += 2;
        break;
      case 1:  // bxl
        registers["B"] ^= operand;
        pos += 2;
        break;
      case 2:  // bst
        registers["B"] = operand % 8;
        pos += 2;
        break;
      case 3:  // jnz
        if (registers["A"] > 0) {
          pos = operand;
        } else {
          pos += 2;
        }
        break;
      case 4:  // bxc
        registers["B"] ^= registers["C"];
        pos += 2;
        break;
      case 5:  // out
        output.push_back(operand % 8);
        pos += 2;
        break;
      case 6:  // bdv
        registers["B"] = registers["A"] / (1LL << operand);
        pos += 2;
        break;
      case 7:  // cdv
        registers["C"] = registers["A"] / (1LL << operand);
        pos += 2;
        break;
      default:
        cerr << "Unknown opcode: " << opcode << endl;
        exit(1);
    }
  }

  return output;
}

bool validateProgram(unordered_map<string, long long> registers,
                     const vector<long long> &program, long long A) {
  registers["A"] = A;
  vector<long long> output = executeProgram(registers, program);

  if (output.size() > program.size()) return false;

  if (equal(output.end() - output.size(), output.end(),
            program.end() - output.size())) {
    valid.push_back(A);
    return true;
  }

  return false;
}

void part1() {
  string filename = "input.txt";
  readFile(filename);
  vector<long long> output = executeProgram(registers, program);
  for (size_t i = 0; i < output.size(); ++i) {
    if (i > 0) cout << ",";
    cout << output[i];
  }
  cout << endl;
}

void part2() {
  string filename = "input.txt";
  readFile(filename);
  vector<long long> output = {1};

  for (long long num : output) {
    for (long long i = 0; i < 8; ++i) {
      long long number = i + num;
      registers["A"] = number;
      if (validateProgram(registers, program, number)) {
        output.push_back(number * 8);
      }
    }
  }

  if (!valid.empty()) {
    cout << "Final register: " << valid[0] << endl;
  }
}

int main() {

  part1();
  part2();

  return 0;
}