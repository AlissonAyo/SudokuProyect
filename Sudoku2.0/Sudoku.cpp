#include "Sudoku.h"
#include <iostream>

Sudoku::Sudoku() : offsetX(0), offsetY(0) {
    // Inicializa el tablero con ceros (vac�o)
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            tablero[i][j] = 0;

    // Cargar la fuente (aseg�rate de tener un archivo .ttf en tu proyecto)
    if (!font.loadFromFile("Letra/letra1.ttf")) {
        std::cerr << "No se pudo cargar la fuente" << std::endl;
    }

    inicializar();
}

void Sudoku::inicializar() {
    int cellSize = 40;  // Tama�o de cada celda
    int gridSize = 9 * cellSize;  // Tama�o total de la cuadr�cula (9 celdas)

    // Calcular las posiciones de inicio para centrar la cuadr�cula en la ventana
    offsetX = (960 - gridSize) / 2;  // Posici�n X centrada
    offsetY = (720 - gridSize) / 2;  // Posici�n Y centrada

    // Inicializar las formas de la cuadr�cula
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            grid[i][j].setSize(sf::Vector2f(cellSize, cellSize));
            grid[i][j].setOutlineThickness(1);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setPosition(j * cellSize + offsetX, i * cellSize + offsetY);
            grid[i][j].setFillColor(sf::Color::White);

            // Inicializar los textos de los n�meros
            numbers[i][j].setFont(font);
            numbers[i][j].setCharacterSize(24);
            numbers[i][j].setFillColor(sf::Color::Black);
            numbers[i][j].setPosition(j * cellSize + 10 + offsetX, i * cellSize + 5 + offsetY);
        }
    }
}

void Sudoku::dibujar(sf::RenderWindow &ventana) {
    // Dibujar la cuadr�cula
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            ventana.draw(grid[i][j]);

            // Dibujar los n�meros si la celda no est� vac�a
            if (tablero[i][j] != 0) {
                numbers[i][j].setString(std::to_string(tablero[i][j]));
                ventana.draw(numbers[i][j]);
            }
        }
    }

    // Dibujar las l�neas gruesas cada 3 celdas para diferenciar los subcuadros
    sf::RectangleShape thickLine;
    thickLine.setFillColor(sf::Color::Black);

    // L�neas horizontales
    for (int i = 0; i <= 9; i += 3) {
        thickLine.setSize(sf::Vector2f(9 * 40, 3));
        thickLine.setPosition(offsetX, i * 40 + offsetY - 1.5);
        ventana.draw(thickLine);
    }

    // L�neas verticales
    for (int j = 0; j <= 9; j += 3) {
        thickLine.setSize(sf::Vector2f(3, 9 * 40));
        thickLine.setPosition(j * 40 + offsetX - 1.5, offsetY);
        ventana.draw(thickLine);
    }
}
