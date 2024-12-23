#include <bits/stdc++.h>

using namespace std;

vector<string> splitString(const string &str, char splitter) {
  vector<string> vec = vector<string>();

  int last = 0;
  int size = str.size();
  int i;
  for (i = 0; i < size; i++) {
    if (str[i] == splitter) {
      vec.push_back(str.substr(last, i - last));
      last = i + 1;
    }
  }
  string leftover = str.substr(last, i - last);
  if (!leftover.empty()) vec.push_back(str.substr(last, i - last));

  return vec;
}

string readInput(char *argv[]) {
  if (argv[1] == nullptr) {
    cerr << "Error: Please provide the path to the inputs folder" << '\n';
    exit(EXIT_FAILURE);
  }

  ifstream inputFile(argv[1]);
  if (!inputFile.is_open()) {
    cerr << "Error: input file not found." << '\n';
    exit(EXIT_FAILURE);
  }

  string fileContent = string((istreambuf_iterator<char>(inputFile)),
                              istreambuf_iterator<char>());

  while (fileContent.back() == '\n') fileContent.erase(fileContent.size() - 1);
  fileContent.push_back('\n');

  inputFile.close();
  return fileContent;
}

vector<int64_t> parseInput(const string &input) {
  vector<int64_t> result;
  vector<string> lines = splitString(input, '\n');
  for (const string &line : lines) {
    result.push_back(stoi(line));
  }
  return result;
}

int64_t getSecretNumber(int64_t secret) {
  int64_t multiplied = secret * 64;
  // Mix bitXOR
  secret ^= multiplied;
  // Prune
  secret %= 16777216;

  int64_t divided = secret / 32;
  // Mix bitXOR
  secret ^= divided;
  // Prune
  secret %= 16777216;

  multiplied = secret * 2048;
  // Mix bitXOR
  secret ^= multiplied;
  // Prune
  secret %= 16777216;

  return secret;
}

int64_t solution_1(const string &input) {
  vector<int64_t> numbers = parseInput(input);
  int64_t result = 0;
  for (int64_t number : numbers) {
    for (int i = 0; i < 2000; i++) number = getSecretNumber(number);

    result += number;
  }

  return result;
}

int64_t sumPartern(map<int, int> buyerSells) {
  int64_t totalPatern = 0;
  for (auto [_, sell] : buyerSells) {
    totalPatern += sell;
  }
  return totalPatern;
}

int64_t solution_2(const string &input) {
  vector<int64_t> numbers = parseInput(input);
  map<tuple<int, int, int, int>, map<int, int>> parternBuyerSells;

  for (int buyer = 0; buyer < numbers.size(); buyer++) {
    int number = numbers[buyer];
    vector<int> bananas;
    vector<int> changes;
    bananas.push_back(number % 10);
    for (int i = 0; i < 2000; i++) {
      number = getSecretNumber(number);
      bananas.push_back(number % 10);
      changes.push_back(bananas[i + 1] - bananas[i]);
    }
    for (int c = 0; c < changes.size() - 3; c++) {
      tuple<int, int, int, int> patern = {changes[c], changes[c + 1],
                                          changes[c + 2], changes[c + 3]};
      if (parternBuyerSells.find(patern) == parternBuyerSells.end() ||
          parternBuyerSells[patern].find(buyer) ==
              parternBuyerSells[patern].end()) {
        parternBuyerSells[patern][buyer] = bananas[c + 4];
      }
    }
  }

  int64_t bestSum = 0;
  for (auto [patern, buyerSells] : parternBuyerSells) {
    int64_t totalPatern = sumPartern(buyerSells);
    if (totalPatern > bestSum) {
      bestSum = totalPatern;
    }
  }

  return bestSum;
}

int main(int argc, char *argv[]) {
  string input = readInput(argv);

  cout << "Part 1: " << solution_1(input) << '\n';
  cout << "Part 2: " << solution_2(input) << '\n';

  return 0;
}