#include "Sudoku.h"
#include <iostream>

Sudoku::Sudoku() : offsetX(0), offsetY(0)
{
    // Inicializa el tablero con ceros (vac�o)
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            tablero[i][j] = 0;

    if (!font.loadFromFile("Letra/letra1.ttf"))
    {
        std::cerr << "No se pudo cargar la fuente" << std::endl;
    }

    inicializar();
}

void Sudoku::inicializar()
{
    int cellSize = 40;
    int gridSize = 9 * cellSize;

    offsetX = (960 - gridSize) / 2;
    offsetY = (720 - gridSize) / 2;

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            grid[i][j].setSize(sf::Vector2f(cellSize, cellSize));
            grid[i][j].setOutlineThickness(1);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setPosition(j * cellSize + offsetX, i * cellSize + offsetY);
            grid[i][j].setFillColor(sf::Color::White);

            numbers[i][j].setFont(font);
            numbers[i][j].setCharacterSize(24);
            numbers[i][j].setFillColor(sf::Color::Black);
            numbers[i][j].setPosition(j * cellSize + 10 + offsetX, i * cellSize + 5 + offsetY);
        }
    }
}

void Sudoku::dibujar(sf::RenderWindow &ventana)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            ventana.draw(grid[i][j]);

            if (tablero[i][j] != 0)
            {
                numbers[i][j].setString(std::to_string(tablero[i][j]));
                ventana.draw(numbers[i][j]);
            }
        }
    }

    sf::RectangleShape thickLine;
    thickLine.setFillColor(sf::Color::Black);

    for (int i = 0; i <= 9; i += 3)
    {
        thickLine.setSize(sf::Vector2f(9 * 40, 3));
        thickLine.setPosition(offsetX, i * 40 + offsetY - 1.5);
        ventana.draw(thickLine);
    }

    for (int j = 0; j <= 9; j += 3)
    {
        thickLine.setSize(sf::Vector2f(3, 9 * 40));
        thickLine.setPosition(j * 40 + offsetX - 1.5, offsetY);
        ventana.draw(thickLine);
    }
}

bool Sudoku::esSeguro(int num, int i, int j)
{
    for (int k = 0; k < 9; k++)
    {
        if (tablero[k][j] == num) return false;
        if (tablero[i][k] == num) return false;

        int ki = 3 * (i / 3) + k % 3;
        int kj = 3 * (j / 3) + k / 3;
        if (tablero[ki][kj] == num) return false;
    }
    return true;
}
bool Sudoku::llenar(int i, int j)
{
    if (j == 9)
    {
        if (i == 8) return true;
        i++;
        j = 0;
    }

    if (tablero[i][j] != 0) return llenar(i, j + 1);

    for (int num = 1; num <= 9; num++)
        if (esSeguro(num, i, j))
        {
            tablero[i][j] = num;
            if (llenar(i, j + 1)) return true;
            tablero[i][j] = 0;
        }

    return false;
}

bool Sudoku::esValido(int fila, int columna, int numero) {
    for (int i = 0; i < 9; i++) {
        if (tablero[fila][i] == numero) {
            return false;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (tablero[i][columna] == numero) {
            return false;
        }
    }

    int inicioFila = fila - fila % 3;
    int inicioColumna = columna - columna % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tablero[i + inicioFila][j + inicioColumna] == numero) {
                return false;
            }
        }
    }

    return true;
}

void Sudoku::generarSudokuAleatorio(int numCeldasLlenas) {
    srand(time(0));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            tablero[i][j] = 0;
        }
    }

    for (int n = 0; n < numCeldasLlenas; n++) {
        int fila, columna, numero;
        do {
            fila = rand() % 9;
            columna = rand() % 9;
            numero = rand() % 9 + 1;
        } while (!esValido(fila, columna, numero) || tablero[fila][columna] != 0);

        tablero[fila][columna] = numero;
    }
}

bool Sudoku::verificarSubTableroCompleto(int inicioFila, int inicioColumna) {
    bool numeros[10] = {false};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = tablero[inicioFila + i][inicioColumna + j];
            if (num == 0) return false;
            if (numeros[num]) return false;
            numeros[num] = true;
        }
    }
    return true;
}

void Sudoku::rellenarSudokuAleatorio() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero[i][j] == 0) {
                std::vector<int> numerosDisponibles;
                for (int num = 1; num <= 9; num++) {
                    if (esValido(i, j, num)) {
                        numerosDisponibles.push_back(num);
                    }
                }

                if (!numerosDisponibles.empty()) {
                    int indiceAleatorio = std::rand() % numerosDisponibles.size();
                    tablero[i][j] = numerosDisponibles[indiceAleatorio];
                }
            }
        }
    }
}
bool Sudoku::resolverSudoku(int fila, int columna) {
    if (fila == 9) return true;

    if (columna == 9) return resolverSudoku(fila + 1, 0);

    if (tablero[fila][columna] != 0) return resolverSudoku(fila, columna + 1);

    for (int num = 1; num <= 9; num++) {
        if (esValido(fila, columna, num)) {
            tablero[fila][columna] = num;
            if (resolverSudoku(fila, columna + 1)) return true;
            tablero[fila][columna] = 0;
        }
    }

    return false;
}

void Sudoku::generarSudokuCompleto() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            tablero[i][j] = 0;
        }
    }
    resolverSudoku(0, 0);
}

bool Sudoku::verificarSubTablero(int inicioFila, int inicioColumna) {
    bool numeros[10] = {false};
    bool completo = true;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = tablero[inicioFila + i][inicioColumna + j];
            if (num == 0) {
                completo = false;
            } else {
                if (numeros[num]) {
                    return false;
                }
                numeros[num] = true;
            }
        }
    }

    return completo;
}
void Sudoku::eliminarNumeros(int celdasAEliminar) {
    srand(time(0));

    int count = 0;
    while (count < celdasAEliminar) {
        int fila = rand() % 9;
        int columna = rand() % 9;

        if (tablero[fila][columna] != 0) {
            tablero[fila][columna] = 0;
            count++;
        }
    }
}

void Sudoku::marcarCeldasFijas() {
    bool esCeldaFija[9][9] = { false };
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero[i][j] != 0) {
                esCeldaFija[i][j] = true;
            }
        }
    }
}
