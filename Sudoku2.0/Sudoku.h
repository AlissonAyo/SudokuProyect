#ifndef SUDOKU_H
#define SUDOKU_H

#include <SFML/Graphics.hpp>

class Sudoku
{
private:
    sf::RectangleShape grid[9][9];  // Arreglo 2D para representar la cuadr�cula de Sudoku
    sf::Font font;  // Fuente para dibujar los n�meros
    sf::Text numbers[9][9];  // Texto para mostrar los n�meros en la cuadr�cula
    int tablero[9][9];  // Arreglo para almacenar el Sudoku actual
    int offsetX, offsetY;  // Variables para almacenar el desplazamiento para centrar la cuadr�cula

public:
    Sudoku();  // Constructor
    void inicializar();  // Inicializar la cuadr�cula
    void dibujar(sf::RenderWindow &ventana);  // Dibujar la cuadr�cula y n�meros
};


#endif

