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
    using namespace sf;

    RenderWindow facil(VideoMode(960, 720), "Sudoku Facil");
    RectangleShape fondoFacil(Vector2f(960, 720));
    Texture texturaFacil;
    Sudoku sudokuFacil;

    if (!texturaFacil.loadFromFile("Fondos/fondofacil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo f�cil" << std::endl;
        return -1;
    }
    fondoFacil.setTexture(&texturaFacil);

    sudokuFacil.generarSudokuCompleto();
    sudokuFacil.eliminarNumeros(30);  // 30 celdas vac�as para el modo f�cil
    sudokuFacil.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    // Vector para almacenar el color original de las celdas
    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    RenderWindow ventanaFlotante(VideoMode(300, 100), "Mensaje", Style::None);
    ventanaFlotante.setPosition(facil.getPosition() + Vector2i(330, 310));
    Text mensajeTexto;
    mensajeTexto.setFont(sudokuFacil.font);
    mensajeTexto.setCharacterSize(18);
    mensajeTexto.setFillColor(Color::Black);
    mensajeTexto.setPosition(10, 10);

    Clock relojAnimacion;
    bool mostrarVentanaFlotante = false;
    float tiempoMostrarVentana = 0;

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
    textoBotonMenu.setString("Menu");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(300, 200));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(330, 260);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Regresar al Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; ++i)
    {
        Text opcion;
        opcion.setFont(sudokuFacil.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(20);
        opcion.setFillColor(Color::White);
        opcion.setPosition(350, 280 + i * 50);
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

                    // Solo permite modificar celdas que no son fijas
                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                            sudokuFacil.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;

                        sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3
                        if (sudokuFacil.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuFacil.grid[fila][columna].setFillColor(Color::Green);  // Cambia el color a verde
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada]){
                            mensajeTexto.setString("Valor no valido o casilla fija.");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuFacil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("Game Over");
                            mostrarVentanaFlotante = true;
                            tiempoMostrarVentana = 0;
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
                                    return 0; // Volver al men� principal
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
                        // Restaurar el color original de la celda previamente resaltada
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

                        // Calcular la fila y columna seleccionadas en funci�n de la posici�n del mouse
                        columnaSeleccionada = (mousePos.x - sudokuFacil.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuFacil.offsetY) / 40;

                        // Asegurarse de que la selecci�n est� dentro del tablero
                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                                columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            // Guardar el color original de la nueva celda seleccionada
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuFacil.grid[filaSeleccionada][columnaSeleccionada].getFillColor();

                            // Resaltar la fila, columna y bloque 3x3 de la celda seleccionada
                            for (int i = 0; i < 9; ++i)
                            {
                                sudokuFacil.grid[filaSeleccionada][i].setFillColor(Color(200, 200, 250)); // Resaltar fila
                                sudokuFacil.grid[i][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar columna
                            }

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuFacil.grid[i][j].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3
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

            if (minutos >= 5)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarVentanaFlotante = true;
                tiempoMostrarVentana = 0;
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
            for (const auto& opcion : opcionesMenu)
            {
                facil.draw(opcion);
            }
        }

        if (mostrarVentanaFlotante)
        {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2)
            {
                mostrarVentanaFlotante = false;
                ventanaFlotante.setVisible(false);
            }
            else
            {
                ventanaFlotante.setVisible(true);
            }
        }
        else
        {
            relojAnimacion.restart();
        }

        facil.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(2));
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
    sudokuMedio.eliminarNumeros(40);  // 40 celdas vac�as para el modo medio
    sudokuMedio.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    // Vector para almacenar el color original de las celdas
    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    RenderWindow ventanaFlotante(VideoMode(300, 100), "Mensaje", Style::None);
    ventanaFlotante.setPosition(medio.getPosition() + Vector2i(330, 310));
    Text mensajeTexto;
    mensajeTexto.setFont(sudokuMedio.font);
    mensajeTexto.setCharacterSize(18);
    mensajeTexto.setFillColor(Color::Black);
    mensajeTexto.setPosition(10, 10);

    Clock relojAnimacion;
    bool mostrarVentanaFlotante = false;
    float tiempoMostrarVentana = 0;

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

    RectangleShape botonMenu(Vector2f(100, 50));
    botonMenu.setFillColor(Color::Blue);
    botonMenu.setPosition(850, 10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuMedio.font);
    textoBotonMenu.setString("Menu");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(300, 200));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(330, 260);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Regresar al Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; ++i)
    {
        Text opcion;
        opcion.setFont(sudokuMedio.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(20);
        opcion.setFillColor(Color::White);
        opcion.setPosition(350, 280 + i * 50);
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

                    // Solo permite modificar celdas que no son fijas
                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                            sudokuMedio.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                        sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3

                        if (sudokuMedio.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuMedio.grid[fila][columna].setFillColor(Color::Green);  // Cambia el color a verde
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada]){
                            mensajeTexto.setString("Valor no valido o casilla fija.");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("Game Over");
                            mostrarVentanaFlotante = true;
                            tiempoMostrarVentana = 0;
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
                                    return 0; // Volver al men� principal
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
                        // Restaurar el color original de la celda previamente resaltada
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

                        // Calcular la fila y columna seleccionadas en funci�n de la posici�n del mouse
                        columnaSeleccionada = (mousePos.x - sudokuMedio.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuMedio.offsetY) / 40;

                        // Asegurarse de que la selecci�n est� dentro del tablero
                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                                columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            // Guardar el color original de la nueva celda seleccionada
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuMedio.grid[filaSeleccionada][columnaSeleccionada].getFillColor();

                            // Resaltar la fila, columna y bloque 3x3 de la celda seleccionada
                            for (int i = 0; i < 9; ++i)
                            {
                                sudokuMedio.grid[filaSeleccionada][i].setFillColor(Color(200, 200, 250)); // Resaltar fila
                                sudokuMedio.grid[i][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar columna
                            }

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuMedio.grid[i][j].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3
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

            if (minutos >= 5)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarVentanaFlotante = true;
                tiempoMostrarVentana = 0;
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
            for (const auto& opcion : opcionesMenu)
            {
                medio.draw(opcion);
            }
        }

        if (mostrarVentanaFlotante)
        {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2)
            {
                mostrarVentanaFlotante = false;
                ventanaFlotante.setVisible(false);
            }
            else
            {
                ventanaFlotante.setVisible(true);
            }
        }
        else
        {
            relojAnimacion.restart();
        }

        medio.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(2));
            return 0; // Volver al men� principal despu�s de 2 segundos
        }
    }

    return 0;
}

int modoDificil()
{
    RenderWindow dificil(VideoMode(960, 720), "Sudoku Dificil");
    RectangleShape fondoDificil(Vector2f(960, 720));
    Texture texturaDificil;
    Sudoku sudokuDificil;

    if (!texturaDificil.loadFromFile("Fondos/fondodificil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo dif�cil" << std::endl;
        return -1;
    }
    fondoDificil.setTexture(&texturaDificil);

    sudokuDificil.generarSudokuCompleto();
    sudokuDificil.eliminarNumeros(45);  // 45 celdas vac�as para el modo dif�cil
    sudokuDificil.marcarCeldasFijas();

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    // Vector para almacenar el color original de las celdas
    std::vector<std::vector<Color>> coloresOriginales(9, std::vector<Color>(9, Color::White));

    RenderWindow ventanaFlotante(VideoMode(300, 100), "Mensaje", Style::None);
    ventanaFlotante.setPosition(dificil.getPosition() + Vector2i(330, 310));
    Text mensajeTexto;
    mensajeTexto.setFont(sudokuDificil.font);
    mensajeTexto.setCharacterSize(18);
    mensajeTexto.setFillColor(Color::Black);
    mensajeTexto.setPosition(10, 10);

    Clock relojAnimacion;
    bool mostrarVentanaFlotante = false;
    float tiempoMostrarVentana = 0;

    Clock cronometro;
    Time tiempoAcumulado = Time::Zero;
    Text timerText;
    timerText.setFont(sudokuDificil.font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);

    int vidas = 3;
    Text vidasText;
    vidasText.setFont(sudokuDificil.font);
    vidasText.setCharacterSize(24);
    vidasText.setFillColor(Color::White);
    vidasText.setPosition(10, 680);
    vidasText.setString("Vidas: " + std::to_string(vidas));

    RectangleShape botonMenu(Vector2f(100, 50));
    botonMenu.setFillColor(Color::Blue);
    botonMenu.setPosition(850, 10);

    Text textoBotonMenu;
    textoBotonMenu.setFont(sudokuDificil.font);
    textoBotonMenu.setString("Menu");
    textoBotonMenu.setCharacterSize(20);
    textoBotonMenu.setFillColor(Color::White);
    textoBotonMenu.setPosition(870, 20);

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(300, 200));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(330, 260);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Regresar al Menu Principal", "Salir del Juego"};
    for (int i = 0; i < 3; ++i)
    {
        Text opcion;
        opcion.setFont(sudokuDificil.font);
        opcion.setString(textosOpciones[i]);
        opcion.setCharacterSize(20);
        opcion.setFillColor(Color::White);
        opcion.setPosition(350, 280 + i * 50);
        opcionesMenu.push_back(opcion);
    }

    bool juegoTerminado = false;

    while (dificil.isOpen())
    {
        Event evento;
        while (dificil.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                dificil.close();
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

                    // Solo permite modificar celdas que no son fijas
                    if (!esCeldaFija[filaSeleccionada][columnaSeleccionada] &&
                            sudokuDificil.esValido(filaSeleccionada, columnaSeleccionada, valorIngresado))
                    {
                        sudokuDificil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                        sudokuDificil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3

                        if (sudokuDificil.verificarSubTablero(filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3))
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuDificil.grid[fila][columna].setFillColor(Color::Green);  // Cambia el color a verde
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!esCeldaFija[filaSeleccionada][columnaSeleccionada]){
                            mensajeTexto.setString("Valor no valido o casilla fija.");
                            vidas--;
                            vidasText.setString("Vidas: " + std::to_string(vidas));
                            sudokuDificil.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                            sudokuDificil.grid[filaSeleccionada][columnaSeleccionada].setFillColor(Color::Red);
                        }
                        if (vidas <= 0)
                        {
                            mensajeTexto.setString("Game Over");
                            mostrarVentanaFlotante = true;
                            tiempoMostrarVentana = 0;
                            juegoTerminado = true;
                        }
                    }
                }

                if (evento.key.code == Keyboard::Num0 && !esCeldaFija[filaSeleccionada][columnaSeleccionada])
                {
                    sudokuDificil.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
            if (evento.type == Event::MouseButtonPressed)
            {
                if (evento.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(dificil);
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
                                    return 0; // Volver al men� principal
                                }
                                else if (i == 2)     // Salir del Juego
                                {
                                    dificil.close();
                                    return -1; // Salir del juego
                                }
                            }
                        }
                    }
                    else
                    {
                        // Restaurar el color original de la celda previamente resaltada
                        for (int i = 0; i < 9; i++)
                        {
                            for (int j = 0; j < 9; j++)
                            {
                                if (sudokuDificil.grid[i][j].getFillColor() == Color(200, 200, 250))
                                {
                                    sudokuDificil.grid[i][j].setFillColor(coloresOriginales[i][j]);
                                }
                            }
                        }

                        // Calcular la fila y columna seleccionadas en funci�n de la posici�n del mouse
                        columnaSeleccionada = (mousePos.x - sudokuDificil.offsetX) / 40;
                        filaSeleccionada = (mousePos.y - sudokuDificil.offsetY) / 40;

                        // Asegurarse de que la selecci�n est� dentro del tablero
                        if (filaSeleccionada >= 0 && filaSeleccionada < 9 &&
                            columnaSeleccionada >= 0 && columnaSeleccionada < 9)
                        {
                            // Guardar el color original de la nueva celda seleccionada
                            coloresOriginales[filaSeleccionada][columnaSeleccionada] = sudokuDificil.grid[filaSeleccionada][columnaSeleccionada].getFillColor();

                            // Resaltar la fila, columna y bloque 3x3 de la celda seleccionada
                            for (int i = 0; i < 9; ++i)
                            {
                                sudokuDificil.grid[filaSeleccionada][i].setFillColor(Color(200, 200, 250)); // Resaltar fila
                                sudokuDificil.grid[i][columnaSeleccionada].setFillColor(Color(200, 200, 250)); // Resaltar columna
                            }

                            int inicioFila = filaSeleccionada - filaSeleccionada % 3;
                            int inicioColumna = columnaSeleccionada - columnaSeleccionada % 3;
                            for (int i = inicioFila; i < inicioFila + 3; ++i)
                            {
                                for (int j = inicioColumna; j < inicioColumna + 3; ++j)
                                {
                                    sudokuDificil.grid[i][j].setFillColor(Color(200, 200, 250)); // Resaltar bloque 3x3
                                }
                            }
                        }
                    }
                }
            }
        }

        seleccion.setPosition(
            sudokuDificil.offsetX + columnaSeleccionada * 40 + 1,
            sudokuDificil.offsetY + filaSeleccionada * 40 + 1
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

            if (minutos >= 5)
            {
                mensajeTexto.setString("Tiempo finalizado");
                mostrarVentanaFlotante = true;
                tiempoMostrarVentana = 0;
                juegoTerminado = true;
            }
        }

        dificil.clear();
        dificil.draw(fondoDificil);
        sudokuDificil.dibujar(dificil);
        dificil.draw(seleccion);
        dificil.draw(timerText);
        dificil.draw(vidasText);
        dificil.draw(botonMenu);
        dificil.draw(textoBotonMenu);

        if (menuAbierto)
        {
            dificil.draw(fondoMenuPausa);
            for (const auto& opcion : opcionesMenu)
            {
                dificil.draw(opcion);
            }
        }

        if (mostrarVentanaFlotante)
        {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2)
            {
                mostrarVentanaFlotante = false;
                ventanaFlotante.setVisible(false);
            }
            else
            {
                ventanaFlotante.setVisible(true);
            }
        }
        else
        {
            relojAnimacion.restart();
        }

        dificil.display();

        if (juegoTerminado)
        {
            sf::sleep(sf::seconds(2));
            return 0; // Volver al men� principal despu�s de 2 segundos
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

    bool menuAbierto = false;
    RectangleShape fondoMenuPausa(Vector2f(300, 200));
    fondoMenuPausa.setFillColor(Color(100, 100, 100, 200));
    fondoMenuPausa.setPosition(330, 260);

    std::vector<Text> opcionesMenu;
    std::vector<std::string> textosOpciones = {"Volver a la partida", "Reiniciar", "Menu Principal"};
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
    int resoluble = true;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    if (!texturaAutomatico.loadFromFile("Fondos/fondoautomatico.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo autom�tico" << std::endl;
        return -1;
    }
    fondoAutomatico.setTexture(&texturaAutomatico);

    while (automatico.isOpen())
    {
        Time tiempo = clock.getElapsedTime();
        Event evento;
        if (tiempo.asSeconds() > tiempoMensaje) mensajeVisible = false;
        while (automatico.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                automatico.close();
            }
            if (evento.type == Event::KeyPressed)
            {
                if (evento.key.code == Keyboard::Up && fila > 0)
                {
                    fila--;
                }
                else if (evento.key.code == Keyboard::Down && fila < 8)
                {
                    fila++;
                }
                else if (evento.key.code == Keyboard::Left && columna > 0)
                {
                    columna--;
                }
                else if (evento.key.code == Keyboard::Right && columna < 8)
                {
                    columna++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9)
                {
                    valor = evento.key.code - Keyboard::Num0;

                    if (sudokuAutomatico.esValido(fila, columna, valor))
                    {
                        sudokuAutomatico.tablero[fila][columna] = valor;
                    }
                    else
                    {
                        floatingText.setString("Numero repetido!");
                        mensajeVisible = true;
                        clock.restart();
                    }
                }

                if (evento.key.code == Keyboard::Key::Enter)
                {
                    if(!sudokuAutomatico.llenar(0, 0))
                    {
                        floatingText.setString("No se puede resolver este sudoku!");
                        mensajeVisible = true;
                        clock.restart();
                    }
                    else
                    {
                        floatingText.setString("Sudoku resuelto! Muy facil!");
                        mensajeVisible = true;
                        clock.restart();
                    }
                }

                if (evento.key.code == Keyboard::Num0)
                {
                    sudokuAutomatico.tablero[fila][columna] = 0;
                }
            }
        }

        seleccion.setPosition(
            sudokuAutomatico.offsetX + columna * 40 + 1,
            sudokuAutomatico.offsetY + fila * 40 + 1
        );

        automatico.clear();

        if (mensajeVisible) automatico.draw(floatingText);
        automatico.draw(fondoAutomatico);
        sudokuAutomatico.dibujar(automatico);  // Dibujar la cuadr�cula de Sudoku
        automatico.draw(seleccion);

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


    // Corrected audio code
    sf::Music musicaDeFondo;
    if (!musicaDeFondo.openFromFile("Pirata.ogg"))
    {
        std::cerr << "No se pudo cargar la musica de fondo" << std::endl;
        return -1;
    }

    // Configurar la m�sica para que se repita continuamente
    musicaDeFondo.setLoop(true);
    musicaDeFondo.play();  // Comenzar la m�sica de fondo

    RectangleShape fondoOpciones(Vector2f(960, 720));
    Texture texturaOpciones;
    if (!texturaOpciones.loadFromFile("Fondos/fondoOpciones.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo de opciones" << std::endl;
        return -1;
    }
    fondoOpciones.setTexture(&texturaOpciones);

    RectangleShape fondoComoJugar(Vector2f(960, 720));
    Texture texturaComoJugar[7];
    std::string fondos[] =
    {
        "Fondos/fondoManual.png",
        "Fondos/fondoManual1.png",
        "Fondos/fondoManual2.png",
        "Fondos/fondoManual3.png",
        "Fondos/fondoManual4.png",
        "Fondos/fondoManual5.png",
        "Fondos/fondoManual6.png"
    };

    for (int i = 0; i < 7; i++)
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
                            while (comoJugar.isOpen())
                            {
                                Event aevent;
                                while (comoJugar.pollEvent(aevent))
                                {
                                    if (aevent.type == Event::Closed)
                                    {
                                        comoJugar.close();
                                    }

                                    if (aevent.type == Event::KeyPressed)
                                    {
                                        if (aevent.key.code == Keyboard::Right)
                                        {
                                            if (ventanaComoJugar < 6)
                                            {
                                                ventanaComoJugar++;
                                                fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                            }
                                        }
                                        else if (aevent.key.code == Keyboard::Left)
                                        {
                                            if (ventanaComoJugar > 0)
                                            {
                                                ventanaComoJugar--;
                                                fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                            }
                                            else
                                            {
                                                comoJugar.close();
                                            }
                                        }
                                        else if (aevent.key.code == Keyboard::Escape)
                                        {
                                            comoJugar.close();
                                        }
                                    }
                                }
                                comoJugar.clear();
                                comoJugar.draw(fondoComoJugar);
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
