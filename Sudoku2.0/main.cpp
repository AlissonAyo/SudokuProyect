#include <SFML/Graphics.hpp>
#include <iostream>
#include "MainMenu.h"
#include "Sudoku.h"

using namespace sf;


int modoFacil()
{
    RenderWindow facil(VideoMode(960, 720), "Fácil");
    RectangleShape fondoFacil(Vector2f(960, 720));
    Texture texturaFacil;
    Sudoku sudokuFacil;  // Crear una instancia de la clase Sudoku

    if (!texturaFacil.loadFromFile("Fondos/fondofacil.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo fácil" << std::endl;
        return -1;
    }
    fondoFacil.setTexture(&texturaFacil);

    while (facil.isOpen())
    {
        Event evento;
        while (facil.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                facil.close();
            }
        }

        facil.clear();
        facil.draw(fondoFacil);
        sudokuFacil.dibujar(facil);  // Dibujar la cuadrícula de Sudoku
        facil.display();
    }

    return 0;
}

int modoMedio()
{
    RenderWindow medio(VideoMode(960, 720), "Medio");
    RectangleShape fondoMedio(Vector2f(960, 720));
    Texture texturaMedio;
    Sudoku sudokuMedio;  // Crear una instancia de la clase Sudoku

    if (!texturaMedio.loadFromFile("Fondos/fondomedio.png"))
    {
        std::cerr << "No se pudo cargar la textura del fondo del modo medio" << std::endl;
        return -1;
    }
    fondoMedio.setTexture(&texturaMedio);

    while (medio.isOpen())
    {
        Event evento;
        while (medio.pollEvent(evento))
        {
            if (evento.type == Event::Closed)
            {
                medio.close();
            }
        }

        medio.clear();
        medio.draw(fondoMedio);
        sudokuMedio.dibujar(medio);  // Dibujar la cuadrícula de Sudoku
        medio.display();
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


