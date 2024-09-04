#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "MainMenu.h"
#include "Sudoku.h"

bool esCeldaFija[9][9] = { false };

int modoFacil()
{
    RenderWindow facil(VideoMode(960, 720), "Sudoku Facil");
    RectangleShape fondoFacil(Vector2f(960, 720));
    Texture texturaFacil;
    Sudoku sudokuFacil;

    if (!texturaFacil.loadFromFile("Fondos/fondofacil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo facil" << std::endl;
        return -1;
    }
    fondoFacil.setTexture(&texturaFacil);

    sudokuFacil.generarSudokuCompleto();
    sudokuFacil.eliminarNumeros(30);  // 30 celdas vacías para el modo fácil
    sudokuFacil.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    Text mensajeTexto;
    mensajeTexto.setFont(sudokuFacil.font);
    mensajeTexto.setCharacterSize(48); // Ajusta el tamaño del texto
    mensajeTexto.setFillColor(Color::Red);
    mensajeTexto.setPosition(330, 320); // Ajusta la posición del mensaje

    Clock relojAnimacion;
    bool mostrarMensaje = false;
    float tiempoMostrarMensaje = 0;

    Clock cronometro;
    Time tiempoAcumulado = Time::Zero;
    Text timerText;
    timerText.setFont(sudokuFacil.font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);

    int vidas = 10;
    Text vidasText;
    vidasText.setFont(sudokuFacil.font);
    vidasText.setCharacterSize(24);
    vidasText.setFillColor(Color::White);
    vidasText.setPosition(10, 680);
    vidasText.setString("Vidas: " + std::to_string(vidas));

    RectangleShape botonMenu(Vector2f(100, 50));
    botonMenu.setFillColor(Color::Blue);
    botonMenu.setPosition(850, 10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuFacil.font);
    textoBotonMenu.setString("Pausa");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(400, 300));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(280, 210);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; i++)
    {
        Text opcion;
        opcion.setFont(sudokuFacil.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(24);
        opcion.setFillColor(Color::White);
        opcion.setPosition(480, 260 + i * 70); // Posición temporal, se ajustará más adelante
        opcionesMenu.push_back(opcion);
    }

    bool juegoTerminado = false;

    while (facil.isOpen())
    {
        Event evento;
        while (facil.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                facil.close();
            }
            if (evento.type == Event::KeyPressed && !menuAbierto && !juegoTerminado)
            {
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0)
                {
                    filaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8)
                {
                    filaSeleccionada++;
                }
                else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0)
                {
                    columnaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8)
                {
                    columnaSeleccionada++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9)
                {
                    int valorIngresado = evento.key.code - Keyboard::Num0;

                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                        sudokuFacil.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                        sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250));

                        if (sudokuFacil.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuFacil.grid[fila][columna].setFillColor(Color::Green);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada])
                        {
                            mensajeTexto.setString("Número Inválido");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("GAME OVER");
                            mostrarMensaje = true;
                            tiempoMostrarMensaje = 0;
                            juegoTerminado = true;
                        }
                    }
                }

                if (evento.key.code == Keyboard::Num0 && !esCeldaFija[filaSeleccionada][columnaSeleccionada])
                {
                    sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
            if (evento.type == Event::MouseButtonPressed)
            {
                if (evento.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(facil);
                    if (botonMenu.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuAbierto = !menuAbierto;
                        if (menuAbierto)
                        {
                            tiempoAcumulado += cronometro.getElapsedTime();
                            cronometro.restart();
                        }
                        else
                        {
                            cronometro.restart();
                        }
                    }
                    if (menuAbierto)
                    {
                        for (int i = 0; i < opcionesMenu.size(); ++i)
                        {
                            if (opcionesMenu[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i == 0)   // Volver a la partida
                                {
                                    menuAbierto = false;
                                    cronometro.restart();
                                }
                                else if (i == 1)     // Regresar al Menu Principal
                                {
                                    return 0; // Volver al menú principal
                                }
                                else if (i == 2)     // Salir del Juego
                                {
                                    facil.close();
                                    return -1; // Salir del juego
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            for (int j = 0; j < 9; j++)
                            {
                                if (sudokuFacil.grid[i][j].getFillColor() == Color(200, 200, 250))
                                {
                                    sudokuFacil.grid[i][j].setFillColor(coloresOriginales[i][j]);
                                }
                            }
                        }

                        columnaSeleccionada = (mousePos.x - sudokuFacil.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuFacil.offsetY) / 40;

                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                            columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].getFillColor();

                            for (int i = 0; i < 9; ++i)
                            {
                                sudokuFacil.grid[filaSeleccionada][i].setFillColor(Color(200, 200, 250));
                                sudokuFacil.grid[i][columnaSeleccionada].setFillColor(Color(200, 200, 250));
                            }

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuFacil.grid[i][j].setFillColor(Color(200, 200, 250));
                                }
                            }
                        }
                    }
                }
            }
        }

        seleccion.setPosition(
            sudokuFacil.offsetX + columnaSeleccionada * 40 + 1,
            sudokuFacil.offsetY + filaSeleccionada * 40 + 1
        );

        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        float escala = 1 + 0.1f * sin(tiempoTranscurrido * 5);
        seleccion.setScale(escala, escala);

        if (!menuAbierto && !juegoTerminado)
        {
            Time tiempoTotal = tiempoAcumulado + cronometro.getElapsedTime();
            int segundos = static_cast<int>(tiempoTotal.asSeconds());
            int minutos = segundos / 60;
            segundos %= 60;
            timerText.setString("Tiempo: " + std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos));

            if (minutos >= 7)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarMensaje = true;
                tiempoMostrarMensaje = 0;
                juegoTerminado = true;
            }
        }

        facil.clear();
        facil.draw(fondoFacil);
        sudokuFacil.dibujar(facil);
        facil.draw(seleccion);
        facil.draw(timerText);
        facil.draw(vidasText);
        facil.draw(botonMenu);
        facil.draw(textoBotonMenu);

        if (menuAbierto)
        {
            facil.draw(fondoMenuPausa);
            for (auto& opcion : opcionesMenu)
            {
                // Center align each option
                FloatRect textRect = opcion.getLocalBounds();
                opcion.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                opcion.setPosition(Vector2f(480, opcion.getPosition().y));

                // Highlight on hover
                if (opcion.getGlobalBounds().contains(Mouse::getPosition(facil).x, Mouse::getPosition(facil).y))
                {
                    opcion.setFillColor(Color::Yellow);
                }
                else
                {
                    opcion.setFillColor(Color::White);
                }

                facil.draw(opcion);
            }
        }

        if (mostrarMensaje)
        {
            facil.draw(mensajeTexto);
            tiempoMostrarMensaje += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarMensaje >= 7) // Tiempo durante el cual el mensaje es visible
            {
                mostrarMensaje = false;
            }
        }

        facil.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(3)); // Espera antes de cerrar el juego
            return 0;
        }
    }

    return 0;
}

int modoMedio()
{
    RenderWindow medio(VideoMode(960, 720), "Sudoku Medio");
    RectangleShape fondoMedio(Vector2f(960, 720));
    Texture texturaMedio;
    Sudoku sudokuMedio;

    if (!texturaMedio.loadFromFile("Fondos/fondomedio.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo medio" << std::endl;
        return -1;
    }
    fondoMedio.setTexture(&texturaMedio);

    sudokuMedio.generarSudokuCompleto();
    sudokuMedio.eliminarNumeros(40);  // 40 celdas vacías para el modo medio
    sudokuMedio.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    Text mensajeTexto;
    mensajeTexto.setFont(sudokuMedio.font);
    mensajeTexto.setCharacterSize(36); // Tamaño del texto
    mensajeTexto.setFillColor(Color::Black);
    mensajeTexto.setPosition(320, 360); // Ajusta a la posición deseada en tu ventana

    Clock relojAnimacion;
    bool mostrarMensaje = false;
    float tiempoMostrarMensaje = 0;

    Clock cronometro;
    Time tiempoAcumulado = Time::Zero;
    Text timerText;
    timerText.setFont(sudokuMedio.font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);

    int vidas = 6;
    Text vidasText;
    vidasText.setFont(sudokuMedio.font);
    vidasText.setCharacterSize(24);
    vidasText.setFillColor(Color::White);
    vidasText.setPosition(10, 680);
    vidasText.setString("Vidas: " + std::to_string(vidas));

    RectangleShape botonMenu(Vector2f(120, 50));
    botonMenu.setFillColor(Color::Blue);
    botonMenu.setPosition(870,10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuMedio.font);
    textoBotonMenu.setString("Pausa");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(400, 300));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(280, 210);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; ++i)
    {
        Text opcion;
        opcion.setFont(sudokuMedio.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(24);
        opcion.setFillColor(Color::White);
        opcion.setPosition(0, 0); // Posición temporal, se ajustará más adelante
        opcionesMenu.push_back(opcion);
    }

    bool juegoTerminado = false;

    while (medio.isOpen())
    {
        Event evento;
        while (medio.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                medio.close();
            }
            if (evento.type == Event::KeyPressed && !menuAbierto && !juegoTerminado)
            {
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0)
                {
                    filaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8)
                {
                    filaSeleccionada++;
                }
                else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0)
                {
                    columnaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8)
                {
                    columnaSeleccionada++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9)
                {
                    int valorIngresado = evento.key.code - Keyboard::Num0;

                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                            sudokuMedio.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;

                        sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250));
                        if (sudokuMedio.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuMedio.grid[fila][columna].setFillColor(Color::Green);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada])
                        {
                            mensajeTexto.setString("Numero Invalido");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("GAME OVER");
                            mostrarMensaje = true;
                            tiempoMostrarMensaje = 0;
                            juegoTerminado = true;
                        }
                    }
                }

                if (evento.key.code == Keyboard::Num0 && !esCeldaFija[filaSeleccionada][columnaSeleccionada])
                {
                    sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
            if (evento.type == Event::MouseButtonPressed)
            {
                if (evento.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(medio);
                    if (botonMenu.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuAbierto = !menuAbierto;
                        if (menuAbierto)
                        {
                            tiempoAcumulado += cronometro.getElapsedTime();
                            cronometro.restart();
                        }
                        else
                        {
                            cronometro.restart();
                        }
                    }
                    if (menuAbierto)
                    {
                        for (int i = 0; i < opcionesMenu.size(); ++i)
                        {
                            if (opcionesMenu[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i == 0)   // Volver a la partida
                                {
                                    menuAbierto = false;
                                    cronometro.restart();
                                }
                                else if (i == 1)     // Regresar al Menu Principal
                                {
                                    return 0; // Volver al menú principal
                                }
                                else if (i == 2)     // Salir del Juego
                                {
                                    medio.close();
                                    return -1; // Salir del juego
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            for (int j = 0; j < 9; j++)
                            {
                                if (sudokuMedio.grid[i][j].getFillColor() == Color(200, 200, 250))
                                {
                                    sudokuMedio.grid[i][j].setFillColor(coloresOriginales[i][j]);
                                }
                            }
                        }

                        columnaSeleccionada = (mousePos.x - sudokuMedio.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuMedio.offsetY) / 40;

                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                                columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].getFillColor();
                            sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250));

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuMedio.grid[i][j].setFillColor(Color(200, 200, 250));
                                }
                            }
                        }
                    }
                }
            }
        }

        seleccion.setPosition(
            sudokuMedio.offsetX + columnaSeleccionada * 40 + 1,
            sudokuMedio.offsetY + filaSeleccionada * 40 + 1
        );

        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        float escala = 1 + 0.1f * sin(tiempoTranscurrido * 5);
        seleccion.setScale(escala, escala);

        if (!menuAbierto && !juegoTerminado)
        {
            Time tiempoTotal = tiempoAcumulado + cronometro.getElapsedTime();
            int segundos = static_cast<int>(tiempoTotal.asSeconds());
            int minutos = segundos / 60;
            segundos %= 60;
            timerText.setString("Tiempo: " + std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos));

            if (minutos >= 4)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarMensaje = true;
                tiempoMostrarMensaje = 0;
                juegoTerminado = true;
            }
        }

        medio.clear();
        medio.draw(fondoMedio);
        sudokuMedio.dibujar(medio);
        medio.draw(seleccion);
        medio.draw(timerText);
        medio.draw(vidasText);
        medio.draw(botonMenu);
        medio.draw(textoBotonMenu);

        if (menuAbierto)
        {
            medio.draw(fondoMenuPausa);
            for (int i = 0; i < opcionesMenu.size(); ++i)
            {
                float xPos = fondoMenuPausa.getPosition().x + (fondoMenuPausa.getSize().x - opcionesMenu[i].getLocalBounds().width) / 2;
                float yPos = fondoMenuPausa.getPosition().y + 50 + i * 80;  // Increased vertical spacing
                opcionesMenu[i].setPosition(xPos, yPos);

                // Create a background rectangle for each option
                RectangleShape optionBackground(Vector2f(300, 50));
                optionBackground.setPosition(xPos - 10, yPos - 10);
                optionBackground.setFillColor(Color(150, 150, 150, 200));

                if (optionBackground.getGlobalBounds().contains(Mouse::getPosition(medio).x, Mouse::getPosition(medio).y))
                {
                    opcionesMenu[i].setFillColor(Color::Yellow);
                    optionBackground.setFillColor(Color(200, 200, 200, 200));
                }
                else
                {
                    opcionesMenu[i].setFillColor(Color::White);
                }

                medio.draw(optionBackground);
                medio.draw(opcionesMenu[i]);
            }
        }

        if (mostrarMensaje)
        {
            mensajeTexto.setFillColor(Color::Red); // Cambiar color del mensaje si es un "GAME OVER"
            medio.draw(mensajeTexto);

            tiempoMostrarMensaje += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarMensaje >= 2)
            {
                mostrarMensaje = false;
                mensajeTexto.setFillColor(Color::Black); // Restablecer color después de mostrar el mensaje
            }
        }
        else
        {
            relojAnimacion.restart();
        }

        medio.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(3));
            return 0; // Volver al menú principal después de 2 segundos
        }
    }

    return 0;
}

int modoDificil()
{
    RenderWindow facil(VideoMode(960, 720), "Sudoku Dificil");
    RectangleShape fondoFacil(Vector2f(960, 720));
    Texture texturaFacil;
    Sudoku sudokuFacil;

    if (!texturaFacil.loadFromFile("Fondos/fondodificil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo facil" << std::endl;
        return -1;
    }
    fondoFacil.setTexture(&texturaFacil);

    sudokuFacil.generarSudokuCompleto();
    sudokuFacil.eliminarNumeros(50);  // 30 celdas vacías para el modo fácil
    sudokuFacil.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    Text mensajeTexto;
    mensajeTexto.setFont(sudokuFacil.font);
    mensajeTexto.setCharacterSize(48); // Ajusta el tamaño del texto
    mensajeTexto.setFillColor(Color::Red);
    mensajeTexto.setPosition(330, 320); // Ajusta la posición del mensaje

    Clock relojAnimacion;
    bool mostrarMensaje = false;
    float tiempoMostrarMensaje = 0;

    Clock cronometro;
    Time tiempoAcumulado = Time::Zero;
    Text timerText;
    timerText.setFont(sudokuFacil.font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);

    int vidas = 5;
    Text vidasText;
    vidasText.setFont(sudokuFacil.font);
    vidasText.setCharacterSize(24);
    vidasText.setFillColor(Color::White);
    vidasText.setPosition(10, 680);
    vidasText.setString("Vidas: " + std::to_string(vidas));

    RectangleShape botonMenu(Vector2f(100, 50));
    botonMenu.setFillColor(Color::Blue);
    botonMenu.setPosition(850, 10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuFacil.font);
    textoBotonMenu.setString("Pausa");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(400, 300));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(280, 210);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; i++)
    {
        Text opcion;
        opcion.setFont(sudokuFacil.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(24);
        opcion.setFillColor(Color::White);
        opcion.setPosition(480, 260 + i * 70); // Posición temporal, se ajustará más adelante
        opcionesMenu.push_back(opcion);
    }

    bool juegoTerminado = false;

    while (facil.isOpen())
    {
        Event evento;
        while (facil.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                facil.close();
            }
            if (evento.type == Event::KeyPressed && !menuAbierto && !juegoTerminado)
            {
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0)
                {
                    filaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8)
                {
                    filaSeleccionada++;
                }
                else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0)
                {
                    columnaSeleccionada--;
                }
                else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8)
                {
                    columnaSeleccionada++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9)
                {
                    int valorIngresado = evento.key.code - Keyboard::Num0;

                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                        sudokuFacil.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                        sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250));

                        if (sudokuFacil.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuFacil.grid[fila][columna].setFillColor(Color::Green);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada])
                        {
                            mensajeTexto.setString("Número Inválido");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("GAME OVER");
                            mostrarMensaje = true;
                            tiempoMostrarMensaje = 0;
                            juegoTerminado = true;
                        }
                    }
                }

                if (evento.key.code == Keyboard::Num0 && !esCeldaFija[filaSeleccionada][columnaSeleccionada])
                {
                    sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
            if (evento.type == Event::MouseButtonPressed)
            {
                if (evento.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(facil);
                    if (botonMenu.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuAbierto = !menuAbierto;
                        if (menuAbierto)
                        {
                            tiempoAcumulado += cronometro.getElapsedTime();
                            cronometro.restart();
                        }
                        else
                        {
                            cronometro.restart();
                        }
                    }
                    if (menuAbierto)
                    {
                        for (int i = 0; i < opcionesMenu.size(); ++i)
                        {
                            if (opcionesMenu[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                            {
                                if (i == 0)   // Volver a la partida
                                {
                                    menuAbierto = false;
                                    cronometro.restart();
                                }
                                else if (i == 1)     // Regresar al Menu Principal
                                {
                                    return 0; // Volver al menú principal
                                }
                                else if (i == 2)     // Salir del Juego
                                {
                                    facil.close();
                                    return -1; // Salir del juego
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 9; i++)
                        {
                            for (int j = 0; j < 9; j++)
                            {
                                if (sudokuFacil.grid[i][j].getFillColor() == Color(200, 200, 250))
                                {
                                    sudokuFacil.grid[i][j].setFillColor(coloresOriginales[i][j]);
                                }
                            }
                        }

                        columnaSeleccionada = (mousePos.x - sudokuFacil.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuFacil.offsetY) / 40;

                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                            columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].getFillColor();

                            for (int i = 0; i < 9; ++i)
                            {
                                sudokuFacil.grid[filaSeleccionada][i].setFillColor(Color(200, 200, 250));
                                sudokuFacil.grid[i][columnaSeleccionada].setFillColor(Color(200, 200, 250));
                            }

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuFacil.grid[i][j].setFillColor(Color(200, 200, 250));
                                }
                            }
                        }
                    }
                }
            }
        }

        seleccion.setPosition(
            sudokuFacil.offsetX + columnaSeleccionada * 40 + 1,
            sudokuFacil.offsetY + filaSeleccionada * 40 + 1
        );

        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        float escala = 1 + 0.1f * sin(tiempoTranscurrido * 5);
        seleccion.setScale(escala, escala);

        if (!menuAbierto && !juegoTerminado)
        {
            Time tiempoTotal = tiempoAcumulado + cronometro.getElapsedTime();
            int segundos = static_cast<int>(tiempoTotal.asSeconds());
            int minutos = segundos / 60;
            segundos %= 60;
            timerText.setString("Tiempo: " + std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos));

            if (minutos >= 2)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarMensaje = true;
                tiempoMostrarMensaje = 0;
                juegoTerminado = true;
            }
        }

        facil.clear();
        facil.draw(fondoFacil);
        sudokuFacil.dibujar(facil);
        facil.draw(seleccion);
        facil.draw(timerText);
        facil.draw(vidasText);
        facil.draw(botonMenu);
        facil.draw(textoBotonMenu);

        if (menuAbierto)
        {
            facil.draw(fondoMenuPausa);
            for (auto& opcion : opcionesMenu)
            {
                FloatRect textRect = opcion.getLocalBounds();
                opcion.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                opcion.setPosition(Vector2f(480, opcion.getPosition().y));

                // Highlight on hover
                if (opcion.getGlobalBounds().contains(Mouse::getPosition(facil).x, Mouse::getPosition(facil).y))
                {
                    opcion.setFillColor(Color::Yellow);
                }
                else
                {
                    opcion.setFillColor(Color::White);
                }

                facil.draw(opcion);
            }
        }

        if (mostrarMensaje)
        {
            facil.draw(mensajeTexto);
            tiempoMostrarMensaje += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarMensaje >= 7) // Tiempo durante el cual el mensaje es visible
            {
                mostrarMensaje = false;
            }
        }

        facil.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(4)); // Espera antes de cerrar el juego
            return 0;
        }
    }

    return 0;
}


int modoAutomatico()
{
    RenderWindow automatico(VideoMode(960, 720), "Automatico");
    RectangleShape fondoAutomatico(Vector2f(960, 720));
    Texture texturaAutomatico;
    Sudoku sudokuAutomatico;

    Text floatingText;
    floatingText.setFont(sudokuAutomatico.font);
    floatingText.setCharacterSize(20);
    floatingText.setFillColor(Color::White);
    floatingText.setPosition(100, 100);

    RectangleShape botonMenu(Vector2f(120, 50));
    botonMenu.setFillColor(Color::Transparent);
    botonMenu.setPosition(830, 10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuAutomatico.font);
    textoBotonMenu.setString("Pausar");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(840, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(300, 200));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(330, 260);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Renaudar", "Reiniciar", "Menu Principal"};
    for (int i = 0; i < 3; ++i)
    {
        Text opcion;
        opcion.setFont(sudokuAutomatico.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(20);
        opcion.setFillColor(Color::White);
        opcion.setPosition(350, 280 + i * 50);
        opcionesMenu.push_back(opcion);
    }

    Clock clock;
    float tiempoMensaje = 2;
    bool mensajeVisible = true;

    int fila = 0;
    int columna = 0;
    int valor = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    if (!texturaAutomatico.loadFromFile("Fondos/fondoautomatico.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo automático" << std::endl;
        return -1;
    }
    fondoAutomatico.setTexture(&texturaAutomatico);

    while (automatico.isOpen())
    {
        Time tiempo = clock.getElapsedTime();
        Event evento;
        if (tiempo.asSeconds() > tiempoMensaje && evento.key.code) mensajeVisible = false;
        while (automatico.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                automatico.close();
            }

            if (evento.type == Event::MouseButtonPressed)
            {
                if (evento.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(automatico);

                    if (mousePos.x >= sudokuAutomatico.offsetX && mousePos.x < sudokuAutomatico.offsetX + 9 * 40 &&
                        mousePos.y >= sudokuAutomatico.offsetY && mousePos.y < sudokuAutomatico.offsetY + 9 * 40 && !menuAbierto)
                    {
                        columna = (mousePos.x - sudokuAutomatico.offsetX) / 40;
                        fila = (mousePos.y - sudokuAutomatico.offsetY) / 40;
                    }

                    if (botonMenu.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        menuAbierto = !menuAbierto;
                    }
                    if (menuAbierto) {
                        for (int i = 0; i < opcionesMenu.size(); ++i) {
                            if (opcionesMenu[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                if (i == 0) { // Volver
                                    menuAbierto = false;
                                } else if (i == 1) { // Reiniciar
                                    for (int i = 0; i < 9; i++)
                                        for (int j = 0; j < 9; j++)
                                            sudokuAutomatico.tablero[i][j] = 0;
                                    mensajeVisible = false;
                                    menuAbierto = false;
                                } else if (i == 2) { // Menú principal
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }

            if (evento.type == Event::KeyPressed && !menuAbierto)
            {
                if (evento.key.code == Keyboard::Up && fila > 0) fila--;
                else if (evento.key.code == Keyboard::Down && fila < 8) fila++;
                else if (evento.key.code == Keyboard::Left && columna > 0) columna--;
                else if (evento.key.code == Keyboard::Right && columna < 8) columna++;

                if (fila != -1 && columna != -1)
                {
                    if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9)
                    {
                        mensajeVisible = false;
                        valor = evento.key.code - Keyboard::Num0;

                        if (sudokuAutomatico.esValido(fila, columna, valor))
                            sudokuAutomatico.tablero[fila][columna] = valor;
                        else
                        {
                            floatingText.setString("Numero repetido!");
                            mensajeVisible = true;
                            clock.restart();
                            tiempoMensaje = 1.5;
                        }
                    }

                    if (evento.key.code == Keyboard::Key::Enter)
                    {
                        if(!sudokuAutomatico.llenar(0, 0))
                        {
                            floatingText.setString("No se puede resolver este sudoku!");
                            mensajeVisible = true;
                            clock.restart();
                            tiempoMensaje = 1000;
                        }
                        else
                        {
                            floatingText.setString("Sudoku resuelto! Muy facil!");
                            mensajeVisible = true;
                            clock.restart();
                            tiempoMensaje = 1000;
                        }
                    }

                    if (evento.key.code == Keyboard::Num0)
                    {
                        mensajeVisible = false;
                        sudokuAutomatico.tablero[fila][columna] = 0;
                    }
                }
            }
        }

        if (fila != -1 && columna != -1)
        {
            seleccion.setPosition(
                sudokuAutomatico.offsetX + columna * 40 + 1,
                sudokuAutomatico.offsetY + fila * 40 + 1
            );
        }

        automatico.clear();

        if (mensajeVisible) automatico.draw(floatingText);
        automatico.draw(fondoAutomatico);
        sudokuAutomatico.dibujar(automatico);  // Dibujar la cuadricula de Sudoku
        if (fila != -1 && columna != -1) automatico.draw(seleccion);
        automatico.draw(botonMenu);
        automatico.draw(textoBotonMenu);

        if (menuAbierto) {
            automatico.draw(fondoMenuPausa);
            for (const auto& opcion : opcionesMenu) {
                automatico.draw(opcion);
            }
        }

        automatico.display();
    }

    return 0;
}

int interfaz()
{
    RenderWindow menu(VideoMode(960, 720), "Menu Principal", Style::Default);
    MainMenu mainMenu(menu.getSize().x, menu.getSize().y);

    RectangleShape fondoPrincipal(Vector2f(960, 720));
    Texture texturaPrincipal;
    if (!texturaPrincipal.loadFromFile("Fondos/fondoPrincipal.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo principal" << std::endl;
        return -1;
    }
    fondoPrincipal.setTexture(&texturaPrincipal);

    RectangleShape fondoJuego(Vector2f(960, 720));
    Texture texturaJuego;
    if (!texturaJuego.loadFromFile("Fondos/fondoModo.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del juego" << std::endl;
        return -1;
    }
    fondoJuego.setTexture(&texturaJuego);

    sf::Music musicaDeFondo;
    if (!musicaDeFondo.openFromFile("Pirata.ogg"))
    {
        std::cerr << "No se pudo cargar la musica de fondo" << std::endl;
        return -1;
    }

    musicaDeFondo.setLoop(true);
    musicaDeFondo.play();

    RectangleShape fondoOpciones(Vector2f(960, 720));
    Texture texturaOpciones;
    if (!texturaOpciones.loadFromFile("Fondos/fondoOpciones.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo de opciones" << std::endl;
        return -1;
    }
    fondoOpciones.setTexture(&texturaOpciones);

    RectangleShape fondoComoJugar(Vector2f(960, 720));
    Texture texturaComoJugar[4];
    std::string fondos[] =
    {
        "Fondos/fondoManual.png",
        "Fondos/fondoManual1.png",
        "Fondos/fondoManual2.png",
        "Fondos/fondoManual3.png"
    };

    for (int i = 0; i < 4; i++)
    {
        if (!texturaComoJugar[i].loadFromFile(fondos[i]))
        {
            std::cerr << "No se pudo cargar la textura " << fondos[i] << std::endl;
            return -1;
        }
    }

    int ventanaComoJugar = 0;

    while (menu.isOpen())
    {
        Event evento;
        while (menu.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                menu.close();
            }

            if (evento.type == Event::KeyReleased)
            {
                if (evento.key.code == Keyboard::Down)
                {
                    mainMenu.moverAbajo();
                }

                if (evento.key.code == Keyboard::Up)
                {
                    mainMenu.moverArriba();
                }

                if (evento.key.code == Keyboard::Return)
                {
                    if (mainMenu.esSubMenuActivo())
                    {
                        int y = mainMenu.menuSubMenuSeleccionado();

                        if (y == 0)
                        {
                            modoFacil();
                        }
                        else if (y == 1)
                        {
                            modoMedio();
                        }
                        else if (y == 2)
                        {
                            modoDificil();
                        }
                        else if (y == 3)
                        {
                            modoAutomatico();
                        }
                        else if (y == 4)
                        {
                            mainMenu.setSubMenu(false);
                            fondoPrincipal.setTexture(&texturaPrincipal);
                        }
                    }
                    else
                    {
                        int x = mainMenu.menuPrincipalSeleccionado();

                        if (x == 0)
                        {
                            fondoPrincipal.setTexture(&texturaJuego);
                            mainMenu.setSubMenu(true);
                        }
                        else if (x == 1)
                        {
                            RenderWindow opciones(VideoMode(960, 720), "Opciones");
                            while (opciones.isOpen())
                            {
                                Event aevent;
                                while (opciones.pollEvent(aevent))
                                {
                                    if (aevent.type == Event::Closed ||
                                        (aevent.type == Event::KeyPressed && aevent.key.code == Keyboard::Escape))
                                    {
                                        opciones.close();
                                    }
                                }
                                opciones.clear();
                                opciones.draw(fondoOpciones);
                                opciones.display();
                            }
                        }
                        else if (x == 2)
                        {
                            ventanaComoJugar = 0;
                            fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                            RenderWindow comoJugar(VideoMode(960, 720), "Como Jugar");

                            Texture texturaBotonRegresar, texturaBotonContinuar, texturaBotonTerminar;
                            texturaBotonRegresar.loadFromFile("Fondos/botonRegresar.png");
                            texturaBotonContinuar.loadFromFile("Fondos/botonContinuar.png");
                            texturaBotonTerminar.loadFromFile("Fondos/botonTerminar.png");

                            Sprite botonRegresar(texturaBotonRegresar);
                            botonRegresar.setPosition(50, 600);

                            Sprite botonContinuar(texturaBotonContinuar);
                            botonContinuar.setPosition(750, 600);

                            Sprite botonTerminar(texturaBotonTerminar);
                            botonTerminar.setPosition(750, 600);

                            while (comoJugar.isOpen())
                            {
                                Event aevent;
                                while (comoJugar.pollEvent(aevent))
                                {
                                    if (aevent.type == Event::Closed)
                                    {
                                        comoJugar.close();
                                    }

                                    if (aevent.type == Event::MouseButtonPressed)
                                    {
                                        if (aevent.mouseButton.button == Mouse::Left)
                                        {
                                            Vector2i posicionMouse = Mouse::getPosition(comoJugar);

                                            // Regresar
                                            if (botonRegresar.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y))
                                            {
                                                if (ventanaComoJugar > 0)
                                                {
                                                    ventanaComoJugar--;
                                                    fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                                }
                                                else
                                                {
                                                    comoJugar.close(); // Cerrar y regresar al menú principal
                                                }
                                            }

                                            // Continuar o Terminar
                                            if (ventanaComoJugar < 3)
                                            {
                                                if (botonContinuar.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y))
                                                {
                                                    ventanaComoJugar++;
                                                    fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                                }
                                            }
                                            else
                                            {
                                                if (botonTerminar.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y))
                                                {
                                                    comoJugar.close();
                                                }
                                            }
                                        }
                                    }
                                }

                                comoJugar.clear();
                                comoJugar.draw(fondoComoJugar);
                                comoJugar.draw(botonRegresar);

                                if (ventanaComoJugar < 3)
                                {
                                    comoJugar.draw(botonContinuar);
                                }
                                else
                                {
                                    comoJugar.draw(botonTerminar);
                                }

                                comoJugar.display();
                            }
                        }
                        else if (x == 3)
                        {
                            menu.close();
                        }
                    }
                }
            }
        }

        menu.clear();
        menu.draw(fondoPrincipal);
        mainMenu.dibujar(menu);
        menu.display();
    }

    return 0;
}

int main()
{
    return interfaz();
}
