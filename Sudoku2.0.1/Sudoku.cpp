#include "Sudoku.h"
#include <iostream>

Sudoku::Sudoku() : offsetX(0), offsetY(0) {
    // Inicializa el tablero con ceros (vacío)
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            tablero[i][j] = 0;

    if (!font.loadFromFile("Letra/letra1.ttf")) {
        std::cerr << "No se pudo cargar la fuente" << std::endl;
    }

    inicializar();
}

void Sudoku::inicializar() {
    int cellSize = 40;
    int gridSize = 9 * cellSize;

    offsetX = (960 - gridSize) / 2;
    offsetY = (720 - gridSize) / 2;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
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

void Sudoku::dibujar(sf::RenderWindow &ventana) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            ventana.draw(grid[i][j]);

            if (tablero[i][j] != 0) {
                numbers[i][j].setString(std::to_string(tablero[i][j]));
                ventana.draw(numbers[i][j]);
            }
        }
    }

    sf::RectangleShape thickLine;
    thickLine.setFillColor(sf::Color::Black);

    for (int i = 0; i <= 9; i += 3) {
        thickLine.setSize(sf::Vector2f(9 * 40, 3));
        thickLine.setPosition(offsetX, i * 40 + offsetY - 1.5);
        ventana.draw(thickLine);
    }

    for (int j = 0; j <= 9; j += 3) {
        thickLine.setSize(sf::Vector2f(3, 9 * 40));
        thickLine.setPosition(j * 40 + offsetX - 1.5, offsetY);
        ventana.draw(thickLine);
    }
}
