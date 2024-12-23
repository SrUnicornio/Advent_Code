import heapq

def parse_maze(input_lines):
    maze = []
    start = end = None
    for r, line in enumerate(input_lines):
        row = list(line.strip())
        maze.append(row)
        for c, cell in enumerate(row):
            if cell == "S":
                start = (r, c)
            elif cell == "E":
                end = (r, c)
    return maze, start, end

def reindeer_maze(maze, start, end):
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]  # North, East, South, West
    rows, cols = len(maze), len(maze[0])
    visited = set()
    pq = [(0, start[0], start[1], 1)]  # (cost, x, y, direction_index)
    
    while pq:
        cost, x, y, direction = heapq.heappop(pq)
        
        # If we reach the end, return the cost
        if (x, y) == end:
            return cost
        
        # Avoid revisiting the same state
        if (x, y, direction) in visited:
            continue
        visited.add((x, y, direction))
        
        # Move forward
        dx, dy = directions[direction]
        nx, ny = x + dx, y + dy
        if 0 <= nx < rows and 0 <= ny < cols and maze[nx][ny] != "#":
            heapq.heappush(pq, (cost + 1, nx, ny, direction))
        
        # Rotate clockwise and counterclockwise
        for turn_cost, new_direction in [(1000, (direction + 1) % 4), (1000, (direction - 1) % 4)]:
            heapq.heappush(pq, (cost + turn_cost, x, y, new_direction))

    return -1  # If no path is found (shouldn't happen for valid input)

def main():
    # Leer el archivo de entrada
    input_file = "input.txt"  # Cambia esto al nombre de tu archivo
    with open(input_file, "r") as file:
        input_lines = file.readlines()
    
    maze, start, end = parse_maze(input_lines)
    result = reindeer_maze(maze, start, end)
    print("Lowest score:", result)

# Ejecutar el programa principal
if __name__ == "__main__":
    main()
