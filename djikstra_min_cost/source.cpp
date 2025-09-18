#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <stack>

using namespace std;

int minCostPathWithTrace(const vector<vector<int>>& grid, vector<pair<int, int>>& path) {
    int rows = grid.size();
    int cols = grid[0].size();

    vector<pair<int, int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    // Start from every cell in the top row
    for (int col = 0; col < cols; ++col) {
        pq.emplace(0, 0, col);
        dist[0][col] = 0;
    }

    pair<int, int> end_cell = {-1, -1};

    while (!pq.empty()) {
        auto [cost, r, c] = pq.top();
        pq.pop();

        if (r == rows - 1) {
            end_cell = {r, c};
            break;
        }

        for (auto [dr, dc] : directions) {
            int nr = r + dr;
            int nc = c + dc;

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                int moveCost = max(0, grid[nr][nc] - grid[r][c]);
                int newCost = cost + moveCost;

                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;
                    parent[nr][nc] = {r, c};
                    pq.emplace(newCost, nr, nc);
                }
            }
        }
    }

    // Reconstruct the path
    if (end_cell.first != -1) {
        int r = end_cell.first, c = end_cell.second;
        while (r != -1 && c != -1) {
            path.emplace_back(r, c);
            tie(r, c) = parent[r][c];
        }
        reverse(path.begin(), path.end());
        return dist[end_cell.first][end_cell.second];
    }

    return -1;
}

int main() {
    vector<vector<int>> grid = {
        {2, 1, 2, 1, 1},
        {1, 1, 1, 2, 2},
        {1, 2, 2, 1, 1},
        {1, 2, 1, 2, 2},
        {2, 2, 2, 3, 1}
    };

    vector<pair<int, int>> path;
    int cost = minCostPathWithTrace(grid, path);

    cout << "Minimum cost to reach bottom row: " << cost << endl;
    cout << "Path:" << endl;
    for (auto [r, c] : path) {
        cout << "(" << r << ", " << c << ") -> ";
    }
    cout << "END" << endl;

    return 0;
}
