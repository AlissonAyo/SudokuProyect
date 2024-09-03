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
    bool esSeguro(int num, int i, int j);
    bool llenar(int i, int j);
    bool esValido(int fila, int columna, int numero);
    void generarSudokuAleatorio(int numCeldasLlenas);
    bool verificarSubTableroCompleto(int inicioFila, int inicioColumna);
    void rellenarSudokuAleatorio();
    bool resolverSudoku(int fila, int columna);
    void generarSudokuCompleto();
    bool verificarSubTablero(int inicioFila, int inicioColumna);
    void eliminarNumeros(int celdasAEliminar);
    void marcarCeldasFijas();
};
#endif // SUDOKU_H
