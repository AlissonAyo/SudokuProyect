#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
