#include <windows.h>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

const int N = 9;
const int cellSize = 60;
const int gridSize = cellSize * N;
const int windowSize = gridSize + 50;
vector<vector<int>> board(N, vector<int>(N, 0));

POINT selectedCell = {-1, -1};  // Para almacenar la celda seleccionada
HWND hwndMain, hwndDifficulty;
HWND hwndEasy, hwndNormal, hwndHard;
HWND hwndTimer;
clock_t startTime;
bool gameStarted = false;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DifficultyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void drawBoard(HDC hdc);
void generateSudoku(vector<vector<int>>& board, int emptyCells);
bool isSafe(const vector<vector<int>>& board, int row, int col, int num);
bool solveSudoku(vector<vector<int>>& board);

void createWelcomeWindow(HINSTANCE hInstance);
void createDifficultyWindow(HINSTANCE hInstance);
void startGame(HWND hwnd);
void updateTimer();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    createWelcomeWindow(hInstance);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void createWelcomeWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "WelcomeWindowClass";
    WNDCLASSA wc = { };

    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    hwndMain = CreateWindowExA(
        0,
        CLASS_NAME,
        "Bienvenido al Sudoku",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwndMain == NULL) {
        return;
    }

    CreateWindowA(
        "BUTTON",
        "INICIAR",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100,
        70,
        100,
        30,
        hwndMain,
        (HMENU)1,
        hInstance,
        NULL
    );

    ShowWindow(hwndMain, SW_SHOW);
}

void createDifficultyWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "DifficultyWindowClass";
    WNDCLASSA wc = { };

    wc.lpfnWndProc = DifficultyWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    hwndDifficulty = CreateWindowExA(
        0,
        CLASS_NAME,
        "Selecciona la Dificultad",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwndDifficulty == NULL) {
        return;
    }

    hwndEasy = CreateWindowA(
        "BUTTON",
        "FÁCIL",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100,
        30,
        100,
        30,
        hwndDifficulty,
        (HMENU)2,
        hInstance,
        NULL
    );

    hwndNormal = CreateWindowA(
        "BUTTON",
        "NORMAL",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100,
        70,
        100,
        30,
        hwndDifficulty,
        (HMENU)3,
        hInstance,
        NULL
    );

    hwndHard = CreateWindowA(
        "BUTTON",
        "DIFÍCIL",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100,
        110,
        100,
        30,
        hwndDifficulty,
        (HMENU)4,
        hInstance,
        NULL
    );

    ShowWindow(hwndDifficulty, SW_SHOW);
}

void startGame(HWND hwnd) {
    if (hwnd == hwndEasy) {
        generateSudoku(board, 40);
    } else if (hwnd == hwndNormal) {
        generateSudoku(board, 50);
    } else if (hwnd == hwndHard) {
        generateSudoku(board, 60);
    }

    gameStarted = true;
    startTime = clock();

    // Crear una ventana para el Sudoku
    const char CLASS_NAME[] = "SudokuWindowClass";
    WNDCLASSA wc = { };

    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    hwndMain = CreateWindowExA(
        0,
        CLASS_NAME,
        "Sudoku",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, windowSize + 100, windowSize + 100,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    if (hwndMain == NULL) {
        return;
    }

    // Crear un control de texto para el cronómetro
    hwndTimer = CreateWindowA(
        "STATIC",
        "Tiempo: 0",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        25, windowSize + 30, 100, 30,
        hwndMain,
        NULL,
        wc.hInstance,
        NULL
    );

    ShowWindow(hwndMain, SW_SHOW);

    SetTimer(hwndMain, 1, 1000, NULL);  // Iniciar un temporizador que se actualiza cada segundo
}

void updateTimer() {
    if (gameStarted) {
        clock_t currentTime = clock();
        int secondsElapsed = int(double(currentTime - startTime) / CLOCKS_PER_SEC);
        ostringstream oss;
        oss << "Tiempo: " << secondsElapsed << " s";
        SetWindowTextA(hwndTimer, oss.str().c_str());
    }
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) {  // Botón INICIAR
                createDifficultyWindow((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE));
                ShowWindow(hwndMain, SW_HIDE);
            }
            break;
        }
        case WM_TIMER: {
            updateTimer();
            break;
        }
        case WM_PAINT: {
            if (gameStarted) {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                drawBoard(hdc);
                EndPaint(hwnd, &ps);
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            if (gameStarted) {
                int xPos = LOWORD(lParam) - 25;
                int yPos = HIWORD(lParam) - 25;
                if (xPos >= 0 && yPos >= 0 && xPos < gridSize && yPos < gridSize) {
                    selectedCell.x = xPos / cellSize;
                    selectedCell.y = yPos / cellSize;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            break;
        }
        case WM_KEYDOWN: {
            if (gameStarted && selectedCell.x != -1 && selectedCell.y != -1 && wParam >= '1' && wParam <= '9') {
                int num = wParam - '0';
                if (isSafe(board, selectedCell.y, selectedCell.x, num)) {
                    board[selectedCell.y][selectedCell.x] = num;
                    InvalidateRect(hwnd, NULL, TRUE);
                    bool completed = true;
                    for (int row = 0; row < N; ++row) {
                        for (int col = 0; col < N; ++col) {
                            if (board[row][col] == 0) {
                                completed = false;
                                break;
                            }
                        }
                    }
                    if (completed) {
                        gameStarted = false;
                        KillTimer(hwnd, 1);  // Detener el temporizador
                        clock_t endTime = clock();
                        int timeTaken = int(double(endTime - startTime) / CLOCKS_PER_SEC);
                        string message = "¡Felicidades! Has completado el Sudoku en " + to_string(timeTaken) + " segundos.";
                        MessageBoxA(hwnd, message.c_str(), "Juego Completado", MB_OK);
                    }
                } else {
                    MessageBoxA(hwnd, "Valor incorrecto. Intenta de nuevo.", "Error", MB_OK);
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK DifficultyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: {
            if (LOWORD(wParam) == 2 || LOWORD(wParam) == 3 || LOWORD(wParam) == 4) {
                startGame((HWND)lParam);
                ShowWindow(hwndDifficulty, SW_HIDE);
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void drawBoard(HDC hdc) {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN thickPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
    HBRUSH highlightBrush = CreateSolidBrush(RGB(173, 216, 230));
    SelectObject(hdc, pen);

    for (int i = 0; i <= N; ++i) {
        if (i % 3 == 0)
            SelectObject(hdc, thickPen);
        else
            SelectObject(hdc, pen);

        MoveToEx(hdc, 25, 25 + i * cellSize, NULL);
        LineTo(hdc, 25 + gridSize, 25 + i * cellSize);
        MoveToEx(hdc, 25 + i * cellSize, 25, NULL);
        LineTo(hdc, 25 + i * cellSize, 25 + gridSize);
    }

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
                if (selectedCell.x == col && selectedCell.y == row) {
                    RECT rect = {25 + col * cellSize + 2, 25 + row * cellSize + 2, 25 + (col + 1) * cellSize - 2, 25 + (row + 1) * cellSize - 2};
                    FillRect(hdc, &rect, highlightBrush);
                }
            } else {
                string num = to_string(board[row][col]);
                TextOutA(hdc, 25 + col * cellSize + 20, 25 + row * cellSize + 20, num.c_str(), num.size());
            }
        }
    }

    DeleteObject(pen);
    DeleteObject(thickPen);
    DeleteObject(highlightBrush);
}

bool isSafe(const vector<vector<int>>& board, int row, int col, int num) {
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
    for (row = 0; row < N; ++row) {
        for (col = 0; col < N; ++col) {
            if (board[row][col] == 0) {
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
        }
    }
    return true;
}

void generateSudoku(vector<vector<int>>& board, int emptyCells) {
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
    for (int i = 0; i < emptyCells; ++i) {
        int row = rand() % N;
        int col = rand() % N;
        board[row][col] = 0;
    }
}
