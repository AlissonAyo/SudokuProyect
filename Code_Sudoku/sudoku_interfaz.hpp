#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Sudoku {
private:
    int dificultad;
    int casillas[9][9];
public:
    Sudoku(int dificultad) : dificultad(dificultad) {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                casillas[i][j] = 0;
            }
    }

    void setCasilla(int num, int i, int j) {
        if (num > 0 && num <= 9)
            if (i >= 0 && i < 9 && j >= 0 && j < 9)
                casillas[i][j] = num;
            else std::cout << "Posicion fuera de rango." << std::endl;
        else std::cout << "Numero no valido" << std::endl;
    }

    bool esSeguro(int num, int i, int j) {
        for (int k = 0; k < 9; k++) {
            if (casillas[k][j] == num) return false;
            if (casillas[i][k] == num) return false;

            int ki = 3 * (i / 3) + k % 3;
            int kj = 3 * (j / 3) + k / 3;
            if (casillas[ki][kj] == num) return false;

        }
        return true;
    }

    bool llenar(int i, int j) {
        if (j == 9) {
            if (i == 8) return true;
            i++;
            j = 0;
        }

        if (casillas[i][j] != 0) return llenar(i, j + 1);

        for (int num = 1; num <= 9; num++)
            if (esSeguro(num, i, j)) {
                casillas[i][j] = num;
                if (llenar(i, j + 1)) return true;
                casillas[i][j] = 0;
            }

        return false;
    }

    void mostrarConsola() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << casillas[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void graficarSudoku(sf::RenderWindow &window, sf::Text &text, int x, int y, int casillaTamano, int bordeTamano, sf::Color color1, sf::Color color2, sf::Color color3) {
        int fondoTamano = 9 * casillaTamano + 10 * bordeTamano;
        int grupoTamano = 3 * casillaTamano + 2 * bordeTamano;

        sf::RectangleShape fondo(sf::Vector2f(fondoTamano, fondoTamano));
        fondo.setFillColor(color1);
        fondo.setPosition(sf::Vector2f(x, y));

        sf::RectangleShape grupo(sf::Vector2f(grupoTamano, grupoTamano));
        grupo.setFillColor(color2);

        sf::RectangleShape casilla(sf::Vector2f(casillaTamano, casillaTamano));
        casilla.setFillColor(color3);

        window.draw(fondo);

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                int pi = x + bordeTamano * (i+1) + grupoTamano * i;
                int pj = y + bordeTamano * (j+1) + grupoTamano * j;

                grupo.setPosition(sf::Vector2f(pi, pj));
                window.draw(grupo);
            }
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                std::stringstream ss;
                ss << casillas[i][j];
                std::string numeroCasilla = ss.str();

                int pi = x + bordeTamano * (i+1) + casillaTamano * i;
                int pj = y + bordeTamano * (j+1) + casillaTamano * j;
                int textBorde = (casillaTamano - text.getCharacterSize()) / 2;

                casilla.setPosition(sf::Vector2f(pi, pj));
                window.draw(casilla);
                text.setPosition(sf::Vector2f(pi + textBorde * 2, pj + textBorde / 2));
                text.setString(numeroCasilla);
                window.draw(text);
            }
    }
};

void graficarSudoku(sf::RenderWindow &window, int x, int y, int casillaTamano, int bordeTamano, sf::Color color1, sf::Color color2, sf::Color color3) {
    int fondoTamano = 9 * casillaTamano + 10 * bordeTamano;
    int grupoTamano = 3 * casillaTamano + 2 * bordeTamano;

    sf::RectangleShape fondo(sf::Vector2f(fondoTamano, fondoTamano));
    fondo.setFillColor(color1);
    fondo.setPosition(sf::Vector2f(x, y));

    sf::RectangleShape grupo(sf::Vector2f(grupoTamano, grupoTamano));
    grupo.setFillColor(color2);

    sf::RectangleShape casilla(sf::Vector2f(casillaTamano, casillaTamano));
    casilla.setFillColor(color3);

    window.draw(fondo);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            grupo.setPosition(sf::Vector2f(x + bordeTamano * (i+1) + grupoTamano * i, y + bordeTamano * (j+1) + grupoTamano * j));
            window.draw(grupo);
        }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            casilla.setPosition(sf::Vector2f(x + bordeTamano * (i+1) + casillaTamano * i, y + bordeTamano * (j+1) + casillaTamano * j));
            window.draw(casilla);
        }
}
