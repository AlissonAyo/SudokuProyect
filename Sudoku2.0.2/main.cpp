#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "MainMenu.h"
#include "Sudoku.h"

bool esValido(int tablero[9][9], int fila, int columna, int numero) {
    for (int i = 0; i < 9; i++) {
        if (tablero[fila][i] == numero || tablero[i][columna] == numero) {
            return false;
        }
    }

    int inicioFila = fila - fila % 3;
    int inicioColumna = columna - columna % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tablero[i + inicioFila][j + inicioColumna] == numero) {
                return false;
            }
        }
    }

    return true;
}

// Función para rellenar el tablero con números aleatorios en modo fácil
void rellenarSudokuAleatorio(int tablero[9][9]) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Inicializa la semilla de números aleatorios

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero[i][j] == 0) { // Si la celda está vacía
                std::vector<int> numerosDisponibles;
                for (int num = 1; num <= 9; num++) {
                    if (esValido(tablero, i, j, num)) {
                        numerosDisponibles.push_back(num);
                    }
                }

                if (!numerosDisponibles.empty()) {
                    int indiceAleatorio = std::rand() % numerosDisponibles.size();
                    tablero[i][j] = numerosDisponibles[indiceAleatorio];
                }
            }
        }
    }
}



int modoFacil()
{
RenderWindow medio(VideoMode(960, 720), "Facil");
    RectangleShape fondoMedio(Vector2f(960, 720));
    Texture texturaMedio;
    Sudoku sudokuMedio;

    if (!texturaMedio.loadFromFile("Fondos/fondofacil.png")) {
        std::cerr << "No se pudo cargar la textura del fondo del modo medio" << std::endl;
        return -1;
    }
    fondoMedio.setTexture(&texturaMedio);

    rellenarSudokuAleatorio(sudokuMedio.tablero);

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;
    int valorIngresado = 0;

    // Animación de selección
    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    // Ventana flotante para mensajes
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

    while (medio.isOpen()) {
        Event evento;
        while (medio.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                medio.close();
            }
            if (evento.type == Event::KeyPressed) {
                // Mover la selección de la celda con las teclas de dirección
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0) {
                    filaSeleccionada--;
                } else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8) {
                    filaSeleccionada++;
                } else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0) {
                    columnaSeleccionada--;
                } else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8) {
                    columnaSeleccionada++;
                }

                // Ingresar un valor en la celda usando las teclas numéricas
                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9) {
                    valorIngresado = evento.key.code - Keyboard::Num0;

                    if (esValido(sudokuMedio.tablero, filaSeleccionada, columnaSeleccionada, valorIngresado)) {
                        sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                    } else {
                        mensajeTexto.setString("Valor no valido para esta casilla.");
                        mostrarVentanaFlotante = true;
                        tiempoMostrarVentana = 0;
                    }
                }

                // Limpiar la casilla si presiona '0'
                if (evento.key.code == Keyboard::Num0) {
                    sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
        }

        // Actualizar posición de la selección
        seleccion.setPosition(
            sudokuMedio.offsetX + columnaSeleccionada * 40 + 1,
            sudokuMedio.offsetY + filaSeleccionada * 40 + 1
        );

        // Animación de la selección
        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        float escala = 1 + 0.1f * sin(tiempoTranscurrido * 5);
        seleccion.setScale(escala, escala);

        medio.clear();
        medio.draw(fondoMedio);
        sudokuMedio.dibujar(medio);
        medio.draw(seleccion);
        medio.display();

        // Mostrar ventana flotante si es necesario
        if (mostrarVentanaFlotante) {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2) {  // Mostrar durante 2 segundos
                mostrarVentanaFlotante = false;
                ventanaFlotante.setVisible(false);
            } else {
                ventanaFlotante.setVisible(true);
            }
        } else {
            relojAnimacion.restart();
        }
    }

    return 0;
}


int modoMedio() {
    RenderWindow medio(VideoMode(960, 720), "Medio");
    RectangleShape fondoMedio(Vector2f(960, 720));
    Texture texturaMedio;
    Sudoku sudokuMedio;

    if (!texturaMedio.loadFromFile("Fondos/fondomedio.png")) {
        std::cerr << "No se pudo cargar la textura del fondo del modo medio" << std::endl;
        return -1;
    }
    fondoMedio.setTexture(&texturaMedio);

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;
    int valorIngresado = 0;

    // Animación de selección
    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

    // Ventana flotante para mensajes
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

    while (medio.isOpen()) {
        Event evento;
        while (medio.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                medio.close();
            }
            if (evento.type == Event::KeyPressed) {
                // Mover la selección de la celda con las teclas de dirección
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0) {
                    filaSeleccionada--;
                } else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8) {
                    filaSeleccionada++;
                } else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0) {
                    columnaSeleccionada--;
                } else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8) {
                    columnaSeleccionada++;
                }

                // Ingresar un valor en la celda usando las teclas numéricas
                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9) {
                    valorIngresado = evento.key.code - Keyboard::Num0;

                    if (esValido(sudokuMedio.tablero, filaSeleccionada, columnaSeleccionada, valorIngresado)) {
                        sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;
                    } else {
                        mensajeTexto.setString("Valor no valido para esta casilla.");
                        mostrarVentanaFlotante = true;
                        tiempoMostrarVentana = 0;
                    }
                }

                // Limpiar la casilla si presiona '0'
                if (evento.key.code == Keyboard::Num0) {
                    sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
        }

        // Actualizar posición de la selección
        seleccion.setPosition(
            sudokuMedio.offsetX + columnaSeleccionada * 40 + 1,
            sudokuMedio.offsetY + filaSeleccionada * 40 + 1
        );

        // Animación de la selección
        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        float escala = 1 + 0.1f * sin(tiempoTranscurrido * 5);
        seleccion.setScale(escala, escala);

        medio.clear();
        medio.draw(fondoMedio);
        sudokuMedio.dibujar(medio);
        medio.draw(seleccion);
        medio.display();

        // Mostrar ventana flotante si es necesario
        if (mostrarVentanaFlotante) {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2) {  // Mostrar durante 2 segundos
                mostrarVentanaFlotante = false;
                ventanaFlotante.setVisible(false);
            } else {
                ventanaFlotante.setVisible(true);
            }
        } else {
            relojAnimacion.restart();
        }
    }

    return 0;
}
int modoDificil()
{
    RenderWindow dificil(VideoMode(960, 720), "Difícil");
    RectangleShape fondoDificil(Vector2f(960, 720));
    Texture texturaDificil;
    Sudoku sudokuDificil;  // Crear una instancia de la clase Sudoku

    if (!texturaDificil.loadFromFile("Fondos/fondodificil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo difícil" << std::endl;
        return -1;
    }
    fondoDificil.setTexture(&texturaDificil);

    while (dificil.isOpen())
    {
        Event evento;
        while (dificil.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                dificil.close();
            }
        }

        dificil.clear();
        dificil.draw(fondoDificil);
        sudokuDificil.dibujar(dificil);  // Dibujar la cuadrícula de Sudoku
        dificil.display();
    }

    return 0;
}

int modoAutomatico()
{
    RenderWindow automatico(VideoMode(960, 720), "Automático");
    RectangleShape fondoAutomatico(Vector2f(960, 720));
    Texture texturaAutomatico;
    Sudoku sudokuAutomatico;  // Crear una instancia de la clase Sudoku

    if (!texturaAutomatico.loadFromFile("Fondos/fondoautomatico.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo automático" << std::endl;
        return -1;
    }
    fondoAutomatico.setTexture(&texturaAutomatico);

    while (automatico.isOpen())
    {
        Event evento;
        while (automatico.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                automatico.close();
            }
        }

        automatico.clear();
        automatico.draw(fondoAutomatico);
        sudokuAutomatico.dibujar(automatico);  // Dibujar la cuadrícula de Sudoku
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
