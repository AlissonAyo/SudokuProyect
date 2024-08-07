#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 9;

void printBoard(vector<vector<int>>& board) {
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (col == 3 || col == 6)
                cout << " | ";
            cout << board[row][col] << " ";
        }
        if (row == 2 || row == 5) {
            cout << endl;
            for (int i = 0; i < N; ++i)
                cout << "---";
        }
        cout << endl;
    }
}

bool findEmptyCell(vector<vector<int>>& board, int& row, int& col) {
    for (row = 0; row < N; ++row)
        for (col = 0; col < N; ++col)
            if (board[row][col] == 0)
                return true;
    return false;
}

bool isSafe(vector<vector<int>>& board, int row, int col, int num) {
    for (int x = 0; x < N; ++x)
        if (board[row][x] == num || board[x][col] == num)
            return false;
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i + startRow][j + startCol] == num)
                return false;
    return true;
}

bool solveSudoku(vector<vector<int>>& board) {
    int row, col;
    if (!findEmptyCell(board, row, col))
        return true;
    for (int num = 1; num <= 9; ++num) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

bool isValidBoard(vector<vector<int>>& board) {
    for (int row = 0; row < N; ++row)
        for (int col = 0; col < N; ++col)
            if (board[row][col] != 0 && !isSafe(board, row, col, board[row][col]))
                return false;
    return true;
}

void removeNumbers(vector<vector<int>>& board, int numRemove) {
    srand(time(0));
    while (numRemove > 0) {
        int row = rand() % N;
        int col = rand() % N;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            --numRemove;
        }
    }
}

void generateSudoku(vector<vector<int>>& board) {
    for (int i = 0; i < N; i += 3) {
        for (int j = 1; j <= 9; ++j) {
            int row = i + rand() % 3;
            int col = i + rand() % 3;
            while (board[row][col] != 0) {
                row = i + rand() % 3;
                col = i + rand() % 3;
            }
            board[row][col] = j;
        }
    }
    solveSudoku(board);
    removeNumbers(board, 40);
}

int main() {
    vector<vector<int>> board(N, vector<int>(N, 0));
    generateSudoku(board);
    cout << "Tablero de Sudoku generado:\n";
    printBoard(board);

    if (solveSudoku(board)) {
        cout << "\nSolucion del Sudoku:\n";
        printBoard(board);
    } else {
        cout << "No se encontro solucion para el Sudoku.\n";
    }

    return 0;
}
