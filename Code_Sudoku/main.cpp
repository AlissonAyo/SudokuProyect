#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include "sudoku_interfaz.hpp"

void ventana_Manual();
void ventana_Principal();
void ventana_Juego();
void ventanaSudokuMedio();

void ventana_Dificil() {
    const int tamanoCuadricula = 9;
    const int tamanoCelda = 50;
    const int desplazamientoX = 100;
    const int desplazamientoY = 100;

    sf::RenderWindow ventana_Dificil(sf::VideoMode(800, 600), "Modo dificil");
    sf::Font fuente;
    if (!fuente.loadFromFile("Tipo_Letra1.ttf")) {
        return;
    }

    sf::Text tituloModo("SUDOKU", fuente, 50);
    tituloModo.setFillColor(sf::Color(221, 36, 121));
    tituloModo.setPosition(100, 20);
    tituloModo.setLetterSpacing(1.5f);

    std::array<std::array<int, tamanoCuadricula>, tamanoCuadricula> cuadricula = {
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array<int, tamanoCuadricula>{0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

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
    if (!fuente2.loadFromFile("Tipo_Letra3.ttf")) {
        return;
    }

    sf::Text textoPistas("Pistas", fuente2, 20);
    textoPistas.setFillColor(sf::Color::Black);
    textoPistas.setPosition(560, 60);

    // Crea corazones para las vidas
    std::vector<sf::CircleShape> corazones(vidas);
    for (int i = 0; i < vidas; ++i) {
        corazones[i].setRadius(10);
        corazones[i].setPointCount(100);
        corazones[i].setFillColor(sf::Color(255, 0, 0));  // Rojo
        corazones[i].setPosition(560 + i * 30, 20);  // Posición inicial ajustada
    }

    while (ventana_Dificil.isOpen()) {
        sf::Event evento;
        while (ventana_Dificil.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana_Dificil.close();
            }

            // Detectar el clic en la cuadrícula
            if (evento.type == sf::Event::MouseButtonPressed) {
                if (evento.mouseButton.button == sf::Mouse::Left) {
                    int x = evento.mouseButton.x;
                    int y = evento.mouseButton.y;

                    if (x >= desplazamientoX && x < desplazamientoX + tamanoCuadricula * tamanoCelda &&
                        y >= desplazamientoY && y < desplazamientoY + tamanoCuadricula * tamanoCelda) {
                        columnaSeleccionada = (x - desplazamientoX) / tamanoCelda;
                        filaSeleccionada = (y - desplazamientoY) / tamanoCelda;
                    }
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
            sf::Color colorLinea = sf::Color::Black; // Color de la línea

            // Líneas horizontales
            sf::RectangleShape linea(sf::Vector2f(tamanoCuadricula * tamanoCelda, grosorLinea));
            linea.setFillColor(colorLinea);
            linea.setPosition(desplazamientoX, desplazamientoY + i * tamanoCelda - (grosorLinea / 2));
            ventana_Dificil.draw(linea);

            // Líneas verticales
            sf::RectangleShape linea2(sf::Vector2f(grosorLinea, tamanoCuadricula * tamanoCelda));
            linea2.setFillColor(colorLinea);
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


// Funcion para manejar la ventana del juego
void ventana_Juego()
{
    sf::RenderWindow ventana_Juego(sf::VideoMode(800, 600), "Juego de Sudoku");

    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra1.ttf"))
    {
        return;
    }
    // Título
    sf::Text tituloModo("Modo de Juego", font, 50);
    tituloModo.setFillColor(sf::Color(221, 36, 121));
    tituloModo.setPosition(100, 20);
    tituloModo.setLetterSpacing(1.5f);
    //imagen png ventana juego

    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("cuadricula.png"))
    {
        return;
    }
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture);
    gridSprite.setPosition(450, 4);
    gridSprite.setScale(0.9f, 0.9f);

    sf::Font font2;
    if (!font2.loadFromFile("Tipo_Letra3.ttf"))
    {
        return;
    }

    sf::RectangleShape buttonRegresar(sf::Vector2f(150, 50));
    buttonRegresar.setFillColor(sf::Color(221, 36, 121));
    buttonRegresar.setPosition(100, 500);

    sf::Text textRegresar("Regresar", font2, 30);
    textRegresar.setFillColor(sf::Color::White);
    textRegresar.setPosition(
        buttonRegresar.getPosition().x + 10,
        buttonRegresar.getPosition().y + 5
    );

    // Botones de dificultad
    sf::RectangleShape buttonFacil(sf::Vector2f(200, 50));
    buttonFacil.setFillColor(sf::Color(221, 36, 121));
    buttonFacil.setPosition(100, 150);

    sf::Text textFacil("Facil", font2, 30);
    textFacil.setFillColor(sf::Color::White);
    textFacil.setPosition(
        buttonFacil.getPosition().x + 65,
        buttonFacil.getPosition().y + 5
    );

    sf::RectangleShape buttonMedio(sf::Vector2f(200, 50));
    buttonMedio.setFillColor(sf::Color(221, 36, 121));
    buttonMedio.setPosition(100, 220);

    sf::Text textMedio("Medio", font2, 30);
    textMedio.setFillColor(sf::Color::White);
    textMedio.setPosition(
        buttonMedio.getPosition().x + 65,
        buttonMedio.getPosition().y + 5
    );

    sf::RectangleShape buttonDificil(sf::Vector2f(200, 50));
    buttonDificil.setFillColor(sf::Color(221, 36, 121));
    buttonDificil.setPosition(100, 290);

    sf::Text textDificil("Dificil", font2, 30);
    textDificil.setFillColor(sf::Color::White);
    textDificil.setPosition(
        buttonDificil.getPosition().x + 50,
        buttonDificil.getPosition().y + 5
    );

    sf::RectangleShape buttonAutomatico(sf::Vector2f(200, 50));
    buttonAutomatico.setFillColor(sf::Color(221, 36, 121));
    buttonAutomatico.setPosition(100, 360);

    sf::Text textAutomatico("Automatico", font2, 30);
    textAutomatico.setFillColor(sf::Color::White);
    textAutomatico.setPosition(
        buttonAutomatico.getPosition().x + 20,
        buttonAutomatico.getPosition().y + 5
    );

    while (ventana_Juego.isOpen())
    {
        sf::Event event;
        while (ventana_Juego.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                ventana_Juego.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventana_Juego);

                if(buttonRegresar.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    ventana_Juego.close();
                    ventana_Principal();

                }
                else if (buttonFacil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    // Acción para el botón Fácil
                }
                else if (buttonMedio.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    // Acción para el botón Medio
                    ventanaSudokuMedio();
                }
                else if (buttonDificil.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    ventana_Dificil();
                }
                else if (buttonAutomatico.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    // Acción para el botón Automático
                }
            }
        }

        ventana_Juego.clear(sf::Color(250, 235, 215));
        ventana_Juego.draw(tituloModo);
        ventana_Juego.draw(gridSprite);
        ventana_Juego.draw(buttonRegresar);
        ventana_Juego.draw(textRegresar);
        ventana_Juego.draw(buttonFacil);
        ventana_Juego.draw(textFacil);
        ventana_Juego.draw(buttonMedio);
        ventana_Juego.draw(textMedio);
        ventana_Juego.draw(buttonDificil);
        ventana_Juego.draw(textDificil);
        ventana_Juego.draw(buttonAutomatico);
        ventana_Juego.draw(textAutomatico);
        ventana_Juego.display();
    }
}

void ventana_Manual_2()
{
    sf::RenderWindow ventana_Manual_2(sf::VideoMode(800, 600), "Manual de Juego");

    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra3.ttf"))
    {
        return;
    }

    sf::Text instruccionesText("El sudoku es un juego de lógica y razonamiento, \npor lo que usted no debería adivinar.Si usted no sabe qué número colocar\n en un espacio, siga escaneando las otras áreas de la cuadrícula\n hasta que vea la oportunidad de colocar un número. ", font, 20);
    instruccionesText.setFillColor(sf::Color::Black);
    instruccionesText.setPosition(50, 50);

    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("Manual2.png"))
    {
        return;
    }
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture);
    gridSprite.setPosition(250, 150);
    gridSprite.setScale(0.6f, 0.6f);

    sf::RectangleShape buttonRegresar(sf::Vector2f(150, 50));
    buttonRegresar.setFillColor(sf::Color(221, 36, 121));
    buttonRegresar.setPosition(100, 500);

    sf::Text textRegresar("Regresar", font, 30);
    textRegresar.setFillColor(sf::Color::White);
    textRegresar.setPosition(
        buttonRegresar.getPosition().x + 10,
        buttonRegresar.getPosition().y + 5
    );

    while (ventana_Manual_2.isOpen())
    {
        sf::Event event;
        while (ventana_Manual_2.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                ventana_Manual_2.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventana_Manual_2);

                if (buttonRegresar.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    ventana_Manual_2.close();
                    ventana_Manual();
                    break;
                }
            }
        }

        ventana_Manual_2.clear(sf::Color(250, 235, 215));
        ventana_Manual_2.draw(instruccionesText);
        ventana_Manual_2.draw(gridSprite);
        ventana_Manual_2.draw(buttonRegresar);
        ventana_Manual_2.draw(textRegresar);
        ventana_Manual_2.display();
    }
}


// Función para manejar la ventana del manual del juego
void ventana_Manual()
{
    sf::RenderWindow ventana_Manual(sf::VideoMode(800, 600), "Manual de Juego");

    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra3.ttf"))
    {
        return;
    }

    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("Manual1.png"))
    {
        return;
    }
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture);
    gridSprite.setPosition(250, 150);
    gridSprite.setScale(0.6f, 0.6f);

    sf::Text manualText("Sudoku se juega en una cuadrícula de 9 x 9 espacios.\nDentro de las filas y columnas hay 9 cuadrados \n(compuestos de 3 x 3 espacios). Cada fila, columna y cuadrado \n (9 espacios cada uno) debe completarse con los números del 1 al 9,\n sin repetir ningún número dentro de la fila, columna o cuadrado.", font, 20);
    manualText.setFillColor(sf::Color::Black);
    manualText.setPosition(50, 20);

    sf::RectangleShape buttonRegresar(sf::Vector2f(150, 50));
    buttonRegresar.setFillColor(sf::Color(221, 36, 121));
    buttonRegresar.setPosition(100, 500);

    sf::Text textRegresar("Regresar", font, 30);
    textRegresar.setFillColor(sf::Color::White);
    textRegresar.setPosition(
        buttonRegresar.getPosition().x + 10,
        buttonRegresar.getPosition().y + 5
    );

    sf::RectangleShape buttonSiguiente(sf::Vector2f(200, 50));
    buttonSiguiente.setFillColor(sf::Color(221, 36, 121));
    buttonSiguiente.setPosition(325, 500);

    sf::Text textSiguiente("Siguiente", font, 30);
    textSiguiente.setFillColor(sf::Color::White);
    textSiguiente.setPosition(
        buttonSiguiente.getPosition().x + 10,
        buttonSiguiente.getPosition().y + 5
    );

    while (ventana_Manual.isOpen())
    {
        sf::Event event;
        while (ventana_Manual.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                ventana_Manual.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(ventana_Manual);

                if (buttonSiguiente.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    ventana_Manual.close();
                    ventana_Manual_2();
                    break;
                }
                else if (buttonRegresar.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    ventana_Manual.close();
                    ventana_Principal();
                    break;
                }
            }
        }

        ventana_Manual.clear(sf::Color(250, 235, 215));
        ventana_Manual.draw(manualText);
        ventana_Manual.draw(gridSprite);
        ventana_Manual.draw(buttonRegresar);
        ventana_Manual.draw(textRegresar);
        ventana_Manual.draw(buttonSiguiente);
        ventana_Manual.draw(textSiguiente);
        ventana_Manual.display();
    }
}

void ventana_Principal()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sudoku");

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra2.ttf"))
    {
        return;
    }

    // Título
    sf::Text titulo("SUDOKU", font, 85);
    titulo.setFillColor(sf::Color(221, 36, 121));
    titulo.setPosition(100, 20);
    titulo.setLetterSpacing(1.5f);

    // Fondo
    sf::RectangleShape fondo(sf::Vector2f(800, 600));
    fondo.setFillColor(sf::Color(250, 235, 215));

    // Botón de juego
    sf::RectangleShape buttonJuego(sf::Vector2f(150, 50));
    buttonJuego.setFillColor(sf::Color(221, 36, 121));
    buttonJuego.setPosition(100, 150);

    sf::Text jugarText("Jugar", font, 30);
    jugarText.setFillColor(sf::Color::White);
    jugarText.setPosition(
        buttonJuego.getPosition().x + 25,
        buttonJuego.getPosition().y + 5
    );

    // Botón Cómo Jugar
    sf::RectangleShape buttoncomoJugar(sf::Vector2f(200, 50));
    buttoncomoJugar.setFillColor(sf::Color(221, 36, 121));
    buttoncomoJugar.setPosition(100, 220);

    sf::Text ComoJugarText("Como Jugar", font, 30);
    ComoJugarText.setFillColor(sf::Color::White);
    ComoJugarText.setPosition(
        buttoncomoJugar.getPosition().x + 5,
        buttoncomoJugar.getPosition().y + 5
    );

    // Cargar la imagen de la cuadrícula de Sudoku
    sf::Texture gridTexture;
    if (!gridTexture.loadFromFile("cuadricula.png"))
    {
        return ;
    }
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture);
    gridSprite.setPosition(400, 1);
    gridSprite.setScale(0.9f, 0.9f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (buttonJuego.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    window.close();
                    ventana_Juego();
                    break;
                }
                else if (buttoncomoJugar.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    window.close();
                    ventana_Manual();
                    break;
                }
            }
        }
        window.clear();
        window.draw(fondo);
        window.draw(gridSprite);
        window.draw(titulo);
        window.draw(buttonJuego);
        window.draw(jugarText);
        window.draw(buttoncomoJugar);
        window.draw(ComoJugarText);
        window.display();
    }
}

void ventanaSudokuMedio() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jugar", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("Tipo_Letra2.ttf")) return;

    sf::RectangleShape fondo(sf::Vector2f(800, 600));
    fondo.setFillColor(sf::Color(250, 235, 215));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(fondo);
        graficarSudoku(window, 20, 20, 50, 5, sf::Color(105, 105, 105), sf::Color(169, 169, 169), sf::Color(240, 248, 255));
        window.display();
    }
}

int main()
{
    ventana_Principal();

    return 0;
}

