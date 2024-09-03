#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "MainMenu.h"
#include "Sudoku.h"

bool esSeguro(int num, int i, int j, Sudoku &sudoku) {
    for (int k = 0; k < 9; k++) {
        if (sudoku.tablero[k][j] == num) return false;
        if (sudoku.tablero[i][k] == num) return false;

        int ki = 3 * (i / 3) + k % 3;
        int kj = 3 * (j / 3) + k / 3;
        if (sudoku.tablero[ki][kj] == num) return false;
    }
    return true;
}

bool llenar(int i, int j, Sudoku &sudoku) {
    if (j == 9) {
        if (i == 8) return true;
        i++;
        j = 0;
    }

    if (sudoku.tablero[i][j] != 0) return llenar(i, j + 1, sudoku);

    for (int num = 1; num <= 9; num++)
        if (esSeguro(num, i, j, sudoku)) {
            sudoku.tablero[i][j] = num;
            if (llenar(i, j + 1, sudoku)) return true;
            sudoku.tablero[i][j] = 0;
        }

    return false;
}

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

void generarSudokuAleatorio(int tablero[9][9], int numCeldasLlenas) {
    srand(time(0));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            tablero[i][j] = 0;
        }
    }

    for (int n = 0; n < numCeldasLlenas; n++) {
        int fila, columna, numero;
        do {
            fila = rand() % 9;
            columna = rand() % 9;
            numero = rand() % 9 + 1;
        } while (!esValido(tablero, fila, columna, numero) || tablero[fila][columna] != 0);

        tablero[fila][columna] = numero;
    }
}

bool verificarSubTablero(int tablero[9][9], int inicioFila, int inicioColumna) {
    bool numeros[10] = {false};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = tablero[inicioFila + i][inicioColumna + j];
            if (num != 0) {
                if (numeros[num]) return false;
                numeros[num] = true;
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

    generarSudokuAleatorio(sudokuMedio.tablero, 30);

    int filaSeleccionada = 0;
    int columnaSeleccionada = 0;

    RectangleShape seleccion(Vector2f(38, 38));
    seleccion.setFillColor(Color::Transparent);
    seleccion.setOutlineThickness(2);
    seleccion.setOutlineColor(Color::Yellow);

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
    Text timerText;
    timerText.setFont(sudokuMedio.font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 10);

    int vidas = 3;
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

    while (medio.isOpen()) {
        Event evento;
        while (medio.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                medio.close();
            }
            if (evento.type == Event::KeyPressed) {
                if (evento.key.code == Keyboard::Up && filaSeleccionada > 0) {
                    filaSeleccionada--;
                } else if (evento.key.code == Keyboard::Down && filaSeleccionada < 8) {
                    filaSeleccionada++;
                } else if (evento.key.code == Keyboard::Left && columnaSeleccionada > 0) {
                    columnaSeleccionada--;
                } else if (evento.key.code == Keyboard::Right && columnaSeleccionada < 8) {
                    columnaSeleccionada++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9) {
                    int valorIngresado = evento.key.code - Keyboard::Num0;

                    if (esValido(sudokuMedio.tablero, filaSeleccionada, columnaSeleccionada, valorIngresado)) {
                        sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = valorIngresado;

                        if (verificarSubTablero(sudokuMedio.tablero, filaSeleccionada - filaSeleccionada % 3, columnaSeleccionada - columnaSeleccionada % 3)) {
                            for (int i = 0; i < 3; i++) {
                                for (int j = 0; j < 3; j++) {
                                    int fila = filaSeleccionada - filaSeleccionada % 3 + i;
                                    int columna = columnaSeleccionada - columnaSeleccionada % 3 + j;
                                    sudokuMedio.grid[fila][columna].setFillColor(Color(173, 216, 230));
                                }
                            }
                        }
                    } else {
                        mensajeTexto.setString("Valor no valido para esta casilla.");
                        mostrarVentanaFlotante = true;
                        tiempoMostrarVentana = 0;
                        vidas--;
                        vidasText.setString("Vidas: " + std::to_string(vidas));
                        if (vidas <= 0) {
                            mensajeTexto.setString("Game Over");
                            mostrarVentanaFlotante = true;
                            tiempoMostrarVentana = 0;
                            medio.close();
                        }
                    }
                }

                if (evento.key.code == Keyboard::Num0) {
                    sudokuMedio.tablero[filaSeleccionada][columnaSeleccionada] = 0;
                }
            }
            if (evento.type == Event::MouseButtonPressed) {
                if (evento.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(medio);
                    if (botonMenu.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        medio.close();
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

        int segundos = static_cast<int>(cronometro.getElapsedTime().asSeconds());
        int minutos = segundos / 60;
        segundos %= 60;
        timerText.setString("Tiempo: " + std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos));

        if (minutos >= 5) {
            mensajeTexto.setString("Tiempo finalizado");
            mostrarVentanaFlotante = true;
            tiempoMostrarVentana = 0;
            medio.close();
        }

        medio.clear();
        medio.draw(fondoMedio);
        sudokuMedio.dibujar(medio);
        medio.draw(seleccion);
        medio.draw(timerText);
        medio.draw(vidasText);
        medio.draw(botonMenu);
        medio.draw(textoBotonMenu);
        medio.display();

        if (mostrarVentanaFlotante) {
            ventanaFlotante.clear(Color::White);
            ventanaFlotante.draw(mensajeTexto);
            ventanaFlotante.display();

            tiempoMostrarVentana += relojAnimacion.restart().asSeconds();
            if (tiempoMostrarVentana >= 2) {
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

int modoDificil() {
    RenderWindow dificil(VideoMode(960, 720), "Difícil");
    RectangleShape fondoDificil(Vector2f(960, 720));
    Texture texturaDificil;
    Sudoku sudokuDificil;

    if (!texturaDificil.loadFromFile("Fondos/fondodificil.png")) {
        std::cerr << "No se pudo cargar la textura del fondo del modo difícil" << std::endl;
        return -1;
    }
    fondoDificil.setTexture(&texturaDificil);

    while (dificil.isOpen()) {
        Event evento;
        while (dificil.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                dificil.close();
            }
        }

        dificil.clear();
        dificil.draw(fondoDificil);
        sudokuDificil.dibujar(dificil);
        dificil.display();
    }

    return 0;
}

int modoAutomatico() {
    RenderWindow automatico(VideoMode(960, 720), "Autom�tico");
    RectangleShape fondoAutomatico(Vector2f(960, 720));
    Texture texturaAutomatico;
    Sudoku sudokuAutomatico;

    int fila = 0;
    int columna = 0;
    int valor = 0;
    int resoluble = false;

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

    while (automatico.isOpen()) {
        Event evento;
        while (automatico.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                automatico.close();
            }
            if (evento.type == Event::KeyPressed) {
                if (evento.key.code == Keyboard::Up && fila > 0) {
                    fila--;
                } else if (evento.key.code == Keyboard::Down && fila < 8) {
                    fila++;
                } else if (evento.key.code == Keyboard::Left && columna > 0) {
                    columna--;
                } else if (evento.key.code == Keyboard::Right && columna < 8) {
                    columna++;
                }

                if (evento.key.code >= Keyboard::Num1 && evento.key.code <= Keyboard::Num9) {
                    valor = evento.key.code - Keyboard::Num0;

                    if (esValido(sudokuAutomatico.tablero, fila, columna, valor)) {
                        sudokuAutomatico.tablero[fila][columna] = valor;
                    }
                }

                if (evento.key.code == Keyboard::Key::Enter) {
                    llenar(0, 0, sudokuAutomatico);
                }

                if (evento.key.code == Keyboard::Num0) {
                    sudokuAutomatico.tablero[fila][columna] = 0;
                }
            }
        }

        seleccion.setPosition(
            sudokuAutomatico.offsetX + columna * 40 + 1,
            sudokuAutomatico.offsetY + fila * 40 + 1
        );

        automatico.clear();
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
