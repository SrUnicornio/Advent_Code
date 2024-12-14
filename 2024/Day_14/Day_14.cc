#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct Robot {
  int x, y;    // Current position
  int vx, vy;  // Velocity
};

// Define the grid dimensions
const int GRID_WIDTH = 101;
const int GRID_HEIGHT = 103;

// Function to parse input lines into positions and velocities
Robot parseRobot(const std::string& line) {
  Robot robot;
  sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.x, &robot.y, &robot.vx, &robot.vy);
  return robot;
}

// Function to simulate robot motion for a given number of seconds
std::pair<int, int> simulatePosition(std::pair<int, int> position,
                                     std::pair<int, int> velocity, int seconds) {
  int x = (position.first + velocity.first * seconds) % GRID_WIDTH;
  int y = (position.second + velocity.second * seconds) % GRID_HEIGHT;

  // Handle wrapping for negative positions
  if (x < 0) x += GRID_WIDTH;
  if (y < 0) y += GRID_HEIGHT;

  return {x, y};
}

// Function to calculate the safety factor
int calculateSafetyFactor(
    const std::vector<Robot>& robots, int seconds) {
  // Quadrant counts
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

  for (const auto& robot : robots) {
    // Simulate the position of the robot after the given number of seconds
    std::pair<int, int> position = {robot.x, robot.y};
    std::pair<int, int> velocity = {robot.vx, robot.vy};
    auto [x, y] = simulatePosition(position, velocity, seconds);

    // Determine the quadrant
    if (x > GRID_WIDTH / 2 && y < GRID_HEIGHT / 2) {
      q1++;  // Top-right quadrant
    } else if (x < GRID_WIDTH / 2 && y < GRID_HEIGHT / 2) {
      q2++;  // Top-left quadrant
    } else if (x < GRID_WIDTH / 2 && y > GRID_HEIGHT / 2) {
      q3++;  // Bottom-left quadrant
    } else if (x > GRID_WIDTH / 2 && y > GRID_HEIGHT / 2) {
      q4++;  // Bottom-right quadrant
    }
  }

  // Calculate and return the safety factor
  return q1 * q2 * q3 * q4;
}

int main() {
  std::vector<Robot> robots;
  std::string line;

  // Read input from a file
  std::ifstream inputFile("input.txt");
  if (!inputFile.is_open()) {
    std::cerr << "Unable to open input file." << std::endl;
    return 1;
  }

  while (getline(inputFile, line)) {
    robots.push_back(parseRobot(line));
  }

  inputFile.close();

  // Simulate for 100 seconds and calculate the safety factor
  int safetyFactor = calculateSafetyFactor(robots, 100);

  // Output the result
  std::cout << "Safety Factor: " << safetyFactor << std::endl;

  return 0;
}
