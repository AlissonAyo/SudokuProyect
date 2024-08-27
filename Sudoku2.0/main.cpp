#include <SFML/Graphics.hpp>
#include "MainMenu.h"

using namespace sf;

int main() {
    RenderWindow menu(VideoMode(960, 720), "Menu Principal", Style::Default);
    MainMenu mainMenu(menu.getSize().x, menu.getSize().y);

    // Fondo del menu principal
    RectangleShape fondoPrincipal(Vector2f(960, 720));
    Texture texturaPrincipal;
    if (!texturaPrincipal.loadFromFile("Fondos/fondoPrincipal.png")) {
        std::cerr << "No se pudo cargar la textura del fondo principal" << std::endl;
        return -1;
    }
    fondoPrincipal.setTexture(&texturaPrincipal);

    // Fondo del juego
    RectangleShape fondoJuego(Vector2f(960, 720));
    Texture texturaJuego;
    if (!texturaJuego.loadFromFile("Fondos/fondoModo.png")) {
        std::cerr << "No se pudo cargar la textura del fondo del juego" << std::endl;
        return -1;
    }
    fondoJuego.setTexture(&texturaJuego);

    // Fondo de opciones
    RectangleShape fondoOpciones(Vector2f(960, 720));
    Texture texturaOpciones;
    if (!texturaOpciones.loadFromFile("Fondos/fondoOpciones.png")) {
        std::cerr << "No se pudo cargar la textura del fondo de opciones" << std::endl;
        return -1;
    }
    fondoOpciones.setTexture(&texturaOpciones);

    // Fondos para "Como Jugar"
    RectangleShape fondoComoJugar(Vector2f(960, 720));
    Texture texturaComoJugar[7];
    std::string fondos[] = {
        "Fondos/fondoManual.png",
        "Fondos/fondoManual1.png",
        "Fondos/fondoManual2.png",
        "Fondos/fondoManual3.png",
        "Fondos/fondoManual4.png",
        "Fondos/fondoManual5.png",
        "Fondos/fondoManual6.png"
    };

    for (int i = 0; i < 7; i++) {
        if (!texturaComoJugar[i].loadFromFile(fondos[i])) {
            std::cerr << "No se pudo cargar la textura " << fondos[i] << std::endl;
            return -1;
        }
    }

    int ventanaComoJugar = 0;

    while (menu.isOpen()) {
        Event evento;
        while (menu.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                menu.close();
            }

            if (evento.type == Event::KeyReleased) {
                if (evento.key.code == Keyboard::Down) {
                    mainMenu.moverAbajo();
                }

                if (evento.key.code == Keyboard::Up) {
                    mainMenu.moverArriba();
                }

                if (evento.key.code == Keyboard::Return) {
                    if (mainMenu.esSubMenuActivo()) {
                        int y = mainMenu.menuSubMenuSeleccionado();

                        if (y == 0) { // Opción "Facil"
                            RenderWindow facil(VideoMode(960, 720), "Facil");
                            while (facil.isOpen()) {
                                Event evnt;
                                while (facil.pollEvent(evnt)) {
                                    if (evnt.type == Event::Closed) {
                                        facil.close();
                                    }
                                }
                                facil.clear();
                                // Aquí puedes añadir más código para la ventana "Facil"
                                facil.display();
                            }
                        } else if (y == 1) { // Opción "Medio"
                            RenderWindow medio(VideoMode(960, 720), "Medio");
                            while (medio.isOpen()) {
                                Event evnt;
                                while (medio.pollEvent(evnt)) {
                                    if (evnt.type == Event::Closed) {
                                        medio.close();
                                    }
                                }
                                medio.clear();
                                // Aquí puedes añadir más código para la ventana "Medio"
                                medio.display();
                            }
                        } else if (y == 2) { // Opción "Dificil"
                            RenderWindow dificil(VideoMode(960, 720), "Dificil");
                            while (dificil.isOpen()) {
                                Event evnt;
                                while (dificil.pollEvent(evnt)) {
                                    if (evnt.type == Event::Closed) {
                                        dificil.close();
                                    }
                                }
                                dificil.clear();
                                // Aquí puedes añadir más código para la ventana "Dificil"
                                dificil.display();
                            }
                        } else if (y == 3) { // Opción "Automatico"
                            RenderWindow automatico(VideoMode(960, 720), "Automatico");
                            while (automatico.isOpen()) {
                                Event evnt;
                                while (automatico.pollEvent(evnt)) {
                                    if (evnt.type == Event::Closed) {
                                        automatico.close();
                                    }
                                }
                                automatico.clear();
                                // Aquí puedes añadir más código para la ventana "Automatico"
                                automatico.display();
                            }
                        } else if (y == 4) { // Opción "Salir"
                            mainMenu.setSubMenu(false);
                            fondoPrincipal.setTexture(&texturaPrincipal); // Cambiar al fondo principal
                        }
                    } else {
                        int x = mainMenu.menuPrincipalSeleccionado();

                        if (x == 0) { // Opción "Jugar"
                            fondoPrincipal.setTexture(&texturaJuego); // Cambiar al fondo del submenú
                            mainMenu.setSubMenu(true);
                        } else if (x == 1) { // Opción "Opciones"
                            RenderWindow opciones(VideoMode(960, 720), "Opciones");
                            while (opciones.isOpen()) {
                                Event aevent;
                                while (opciones.pollEvent(aevent)) {
                                    if (aevent.type == Event::Closed ||
                                        (aevent.type == Event::KeyPressed && aevent.key.code == Keyboard::Escape)) {
                                        opciones.close();
                                    }
                                }
                                opciones.clear();
                                opciones.draw(fondoOpciones);
                                opciones.display();
                            }
                        } else if (x == 2) { // Opción "Como Jugar"
                            ventanaComoJugar = 0;
                            fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                            RenderWindow comoJugar(VideoMode(960, 720), "Como Jugar");
                            while (comoJugar.isOpen()) {
                                Event aevent;
                                while (comoJugar.pollEvent(aevent)) {
                                    if (aevent.type == Event::Closed) {
                                        comoJugar.close();
                                    }

                                    if (aevent.type == Event::KeyPressed) {
                                        if (aevent.key.code == Keyboard::Right) {
                                            if (ventanaComoJugar < 6) {
                                                ventanaComoJugar++;
                                                fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                            }
                                        } else if (aevent.key.code == Keyboard::Left) {
                                            if (ventanaComoJugar > 0) {
                                                ventanaComoJugar--;
                                                fondoComoJugar.setTexture(&texturaComoJugar[ventanaComoJugar]);
                                            } else {
                                                comoJugar.close(); // Cerrar la ventana si se regresa al inicio
                                            }
                                        } else if (aevent.key.code == Keyboard::Escape) {
                                            comoJugar.close();
                                        }
                                    }
                                }
                                comoJugar.clear();
                                comoJugar.draw(fondoComoJugar);
                                comoJugar.display();
                            }
                        } else if (x == 3) { // Opción "Salir"
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


