#ifndef SUDOKU_H
#define SUDOKU_H

#include <SFML/Graphics.hpp>

class Sudoku
{
private:
    sf::RectangleShape grid[9][9];  // Arreglo 2D para representar la cuadrícula de Sudoku
    sf::Font font;  // Fuente para dibujar los números
    sf::Text numbers[9][9];  // Texto para mostrar los números en la cuadrícula
    int tablero[9][9];  // Arreglo para almacenar el Sudoku actual
    int offsetX, offsetY;  // Variables para almacenar el desplazamiento para centrar la cuadrícula

public:
    Sudoku();  // Constructor
    void inicializar();  // Inicializar la cuadrícula
    void dibujar(sf::RenderWindow &ventana);  // Dibujar la cuadrícula y números
};


#endif

