#ifndef SUDOKU_H
#define SUDOKU_H
#include <SFML/Graphics.hpp>

class Sudoku {
public:
    sf::RectangleShape grid[9][9];
    sf::Font font;
    sf::Text numbers[9][9];
    int tablero[9][9];
    int offsetX, offsetY;
public:
    Sudoku();
    void inicializar();
    void dibujar(sf::RenderWindow &ventana);
};
#endif // SUDOKU_H
