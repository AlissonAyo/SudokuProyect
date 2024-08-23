#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include "sudoku_interfaz.hpp"

void ventana_Manual();
void ventana_Principal();
void ventana_Juego();
void ventanaSudokuMedio();

void ventana_Dificil() {
    constexpr int tamanoCuadricula = 9;
    constexpr int tamanoCelda = 50;
    constexpr int desplazamientoX = 100;
    constexpr int desplazamientoY = 100;

    sf::RenderWindow ventana_Dificil(sf::VideoMode(800, 600), "Modo difícil");
    sf::Font fuente;
    if (!fuente.loadFromFile("Tipo_Letra1.ttf")) return;

    sf::Text tituloModo("SUDOKU", fuente, 50);
    tituloModo.setFillColor(sf::Color(221, 36, 121));
    tituloModo.setPosition(100, 20);
    tituloModo.setLetterSpacing(1.5f);

    std::array<std::array<int, tamanoCuadricula>, tamanoCuadricula> cuadricula = {};

    // Colocar algunos números para el modo difícil (ejemplo)
    cuadricula[0][1] = 5;
    cuadricula[1][4] = 3;
    cuadricula[3][7] = 6;
    cuadricula[6][2] = 8;

    int vidas = 3;
    int filaSeleccionada = -1;
    int columnaSeleccionada = -1;

    sf::RectangleShape botonPistas(sf::Vector2f(100, 50));
    botonPistas.setFillColor(sf::Color(173, 216, 230));  // Azul claro
    botonPistas.setPosition(550, 50);

    sf::Font fuente2;
    if (!fuente2.loadFromFile("Tipo_Letra3.ttf")) return;

    sf::Text textoPistas("Pistas", fuente2, 20);
    textoPistas.setFillColor(sf::Color::Black);
    textoPistas.setPosition(560, 60);

    // Crear corazones para las vidas
    std::vector<sf::CircleShape> corazones(vidas);
    for (int i = 0; i < vidas; ++i) {
        corazones[i].setRadius(10);
        corazones[i].setPointCount(100);
        corazones[i].setFillColor(sf::Color(255, 0, 0));  // Rojo
        corazones[i].setPosition(560 + i * 30, 20);
    }

    while (ventana_Dificil.isOpen()) {
        sf::Event evento;
        while (ventana_Dificil.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) ventana_Dificil.close();

            // Detectar el clic en la cuadrícula
            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                int x = evento.mouseButton.x;
                int y = evento.mouseButton.y;

                if (x >= desplazamientoX && x < desplazamientoX + tamanoCuadricula * tamanoCelda &&
                    y >= desplazamientoY && y < desplazamientoY + tamanoCuadricula * tamanoCelda) {
                    columnaSeleccionada = (x - desplazamientoX) / tamanoCelda;
                    filaSeleccionada = (y - desplazamientoY) / tamanoCelda;
                }
            }

            // Detectar la entrada de un número
            if (evento.type == sf::Event::TextEntered && filaSeleccionada != -1 && columnaSeleccionada != -1) {
                if (evento.text.unicode >= '1' && evento.text.unicode <= '9') {
                    cuadricula[filaSeleccionada][columnaSeleccionada] = evento.text.unicode - '0';
                    filaSeleccionada = -1;
                    columnaSeleccionada = -1;
                }
            }
        }

        ventana_Dificil.clear(sf::Color(250, 235, 215));
        ventana_Dificil.draw(tituloModo);

        // Dibujar la cuadrícula
        for (int i = 0; i <= tamanoCuadricula; ++i) {
            float grosorLinea = (i % 3 == 0) ? 3.0f : 1.0f; // Grosor de la línea
            sf::RectangleShape linea(sf::Vector2f(tamanoCuadricula * tamanoCelda, grosorLinea));
            linea.setFillColor(sf::Color::Black);
            linea.setPosition(desplazamientoX, desplazamientoY + i * tamanoCelda - (grosorLinea / 2));
            ventana_Dificil.draw(linea);

            sf::RectangleShape linea2(sf::Vector2f(grosorLinea, tamanoCuadricula * tamanoCelda));
            linea2.setFillColor(sf::Color::Black);
            linea2.setPosition(desplazamientoX + i * tamanoCelda - (grosorLinea / 2), desplazamientoY);
            ventana_Dificil.draw(linea2);
        }

        // Dibujar números en la cuadrícula
        for (int fila = 0; fila < tamanoCuadricula; ++fila) {
            for (int columna = 0; columna < tamanoCuadricula; ++columna) {
                if (cuadricula[fila][columna] != 0) {
                    sf::Text numero(std::to_string(cuadricula[fila][columna]), fuente, 30);
                    numero.setFillColor(sf::Color::Black);
                    numero.setPosition(desplazamientoX + columna * tamanoCelda + 15, desplazamientoY + fila * tamanoCelda + 5);
                    ventana_Dificil.draw(numero);
                }
            }
        }

        // Dibujar el rectángulo alrededor de la celda seleccionada
        if (filaSeleccionada != -1 && columnaSeleccionada != -1) {
            sf::RectangleShape rectanguloSeleccion(sf::Vector2f(tamanoCelda, tamanoCelda));
            rectanguloSeleccion.setFillColor(sf::Color::Transparent);
            rectanguloSeleccion.setOutlineColor(sf::Color(0, 255, 0));
            rectanguloSeleccion.setOutlineThickness(3);
            rectanguloSeleccion.setPosition(desplazamientoX + columnaSeleccionada * tamanoCelda, desplazamientoY + filaSeleccionada * tamanoCelda);
            ventana_Dificil.draw(rectanguloSeleccion);
        }

        // Dibujar botón de pistas y corazones de vidas
        ventana_Dificil.draw(botonPistas);
        ventana_Dificil.draw(textoPistas);
        for (const auto& corazon : corazones) {
            ventana_Dificil.draw(corazon);
        }

        ventana_Dificil.display();
    }
}

// Función para manejar la ventana del juego
void ventana_Juego() {
    sf::RenderWindow ventana_Juego(sf::VideoMode(800, 600), "Juego de Sudoku");

    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra1.ttf")) return;

    sf::Text tituloModo("Modo de Juego", font, 50);
    tituloModo.setFillColor(sf::Color(221, 36, 121));
    tituloModo.setPosition(100, 20);
    tituloModo.setLetterSpacing(1.5f);

    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("cuadricula.png")) return;
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture);
    gridSprite.setPosition(450, 4);
    gridSprite.setScale(0.9f, 0.9f);

    sf::Font font2;
    if (!font2.loadFromFile("Tipo_Letra3.ttf")) return;

    sf::RectangleShape buttonRegresar(sf::Vector2f(150, 50));
    buttonRegresar.setFillColor(sf::Color(221, 36, 121));
    buttonRegresar.setPosition(100, 500);

    sf::Text textRegresar("Regresar", font2, 30);
    textRegresar.setFillColor(sf::Color::White);
    textRegresar.setPosition(buttonRegresar.getPosition().x + 10, buttonRegresar.getPosition().y + 5);

    // Botones de dificultad
    std::array<sf::RectangleShape, 4> botones = {
        sf::RectangleShape(sf::Vector2f(200, 50)),
        sf::RectangleShape(sf::Vector2f(200, 50)),
        sf::RectangleShape(sf::Vector2f(200, 50)),
        sf::RectangleShape(sf::Vector2f(200, 50))
    };
    
    botones[0].setFillColor(sf::Color(221, 36, 121));
    botones[0].setPosition(100, 150);

    sf::Text textFacil("Fácil", font2, 30);
    textFacil.setFillColor(sf::Color::White);
    textFacil.setPosition(botones[0].getPosition().x + 65, botones[0].getPosition().y + 5);

    botones[1].setFillColor(sf::Color(221, 36, 121));
    botones[1].setPosition(100, 220);

    sf::Text textMedio("Medio", font2, 30);
    textMedio.setFillColor(sf::Color::White);
    textMedio.setPosition(botones[1].getPosition().x + 65, botones[1].getPosition().y + 5);

    botones[2].setFillColor(sf::Color(221, 36, 121));
    botones[2].setPosition(100, 290);

    sf::Text textDificil("Difícil", font2, 30);
    textDificil.setFillColor(sf::Color::White);
    textDificil.setPosition(botones[2].getPosition().x + 60, botones[2].getPosition().y + 5);

    while (ventana_Juego.isOpen()) {
        sf::Event event;
        while (ventana_Juego.pollEvent(event)) {
            if (event.type == sf::Event::Closed) ventana_Juego.close();

            // Detección de clic en botón "Regresar"
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (buttonRegresar.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    ventana_Juego.close();
                    ventana_Principal();
                }

                // Detección de clic en botones de dificultad
                if (botones[0].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    ventana_Juego.close();
                    ventanaSudokuFacil();
                } else if (botones[1].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    ventana_Juego.close();
                    ventanaSudokuMedio();
                } else if (botones[2].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    ventana_Juego.close();
                    ventana_Dificil();
                }
            }
        }

        ventana_Juego.clear(sf::Color(250, 235, 215));
        ventana_Juego.draw(tituloModo);
        ventana_Juego.draw(gridSprite);
        ventana_Juego.draw(buttonRegresar);
        ventana_Juego.draw(textRegresar);

        ventana_Juego.draw(botones[0]);
        ventana_Juego.draw(textFacil);

        ventana_Juego.draw(botones[1]);
        ventana_Juego.draw(textMedio);

        ventana_Juego.draw(botones[2]);
        ventana_Juego.draw(textDificil);

        ventana_Juego.display();
    }
}
