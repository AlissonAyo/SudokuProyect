#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) : subMenuActivo(false) {
    if (!fuente.loadFromFile("Letra/letra1.ttf")) {
        std::cerr << "No se encontró la fuente";
    }

    // Menú Principal
    menuPrincipal[0].setFont(fuente);
    menuPrincipal[0].setFillColor(Color::White);
    menuPrincipal[0].setString("Jugar");
    menuPrincipal[0].setCharacterSize(70);
    menuPrincipal[0].setPosition(width / 2 - menuPrincipal[0].getGlobalBounds().width / 2, 200);

    menuPrincipal[1].setFont(fuente);
    menuPrincipal[1].setFillColor(Color::White);
    menuPrincipal[1].setString("Creadores");
    menuPrincipal[1].setCharacterSize(70);
    menuPrincipal[1].setPosition(width / 2 - menuPrincipal[1].getGlobalBounds().width / 2, 300);

    menuPrincipal[2].setFont(fuente);
    menuPrincipal[2].setFillColor(Color::White);
    menuPrincipal[2].setString("Como Jugar");
    menuPrincipal[2].setCharacterSize(70);
    menuPrincipal[2].setPosition(width / 2 - menuPrincipal[2].getGlobalBounds().width / 2, 400);

    menuPrincipal[3].setFont(fuente);
    menuPrincipal[3].setFillColor(Color::White);
    menuPrincipal[3].setString("Salir");
    menuPrincipal[3].setCharacterSize(70);
    menuPrincipal[3].setPosition(width / 2 - menuPrincipal[3].getGlobalBounds().width / 2, 500);

    MainMenuSeleccionado = 0;
    menuPrincipal[MainMenuSeleccionado].setFillColor(Color::Blue);

    // Inicialización del submenú
    inicializarSubMenu(width);
}

void MainMenu::inicializarSubMenu(float width) {
    subMenu[0].setFont(fuente);
    subMenu[0].setFillColor(Color::White);
    subMenu[0].setString("Facil");
    subMenu[0].setCharacterSize(70);
    subMenu[0].setPosition(width / 2 - subMenu[0].getGlobalBounds().width / 2, 200);

    subMenu[1].setFont(fuente);
    subMenu[1].setFillColor(Color::White);
    subMenu[1].setString("Medio");
    subMenu[1].setCharacterSize(70);
    subMenu[1].setPosition(width / 2 - subMenu[1].getGlobalBounds().width / 2, 300);

    subMenu[2].setFont(fuente);
    subMenu[2].setFillColor(Color::White);
    subMenu[2].setString("Dificil");
    subMenu[2].setCharacterSize(70);
    subMenu[2].setPosition(width / 2 - subMenu[2].getGlobalBounds().width / 2, 400);

    subMenu[3].setFont(fuente);
    subMenu[3].setFillColor(Color::White);
    subMenu[3].setString("Automatico");
    subMenu[3].setCharacterSize(70);
    subMenu[3].setPosition(width / 2 - subMenu[3].getGlobalBounds().width / 2, 500);

    subMenu[4].setFont(fuente);
    subMenu[4].setFillColor(Color::White);
    subMenu[4].setString("Salir");
    subMenu[4].setCharacterSize(70);
    subMenu[4].setPosition(width / 2 - subMenu[4].getGlobalBounds().width / 2, 600);

    SubMenuSeleccionado = 0;
    subMenu[SubMenuSeleccionado].setFillColor(Color::Blue);
}

MainMenu::~MainMenu() {
}

void MainMenu::dibujar(RenderWindow& window) {
    if (subMenuActivo) {
        for (int i = 0; i < Max_submenu; i++) {
            window.draw(subMenu[i]);
        }
    } else {
        for (int i = 0; i < Max_menu1; i++) {
            window.draw(menuPrincipal[i]);
        }
    }
}

void MainMenu::moverArriba() {
    if (subMenuActivo) {
        if (SubMenuSeleccionado - 1 >= 0) {
            subMenu[SubMenuSeleccionado].setFillColor(Color::White);
            SubMenuSeleccionado--;
            subMenu[SubMenuSeleccionado].setFillColor(Color::Blue);
        } else {
            subMenu[SubMenuSeleccionado].setFillColor(Color::White);
            SubMenuSeleccionado = Max_submenu - 1;
            subMenu[SubMenuSeleccionado].setFillColor(Color::Blue);
        }
    } else {
        if (MainMenuSeleccionado - 1 >= 0) {
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::White);
            MainMenuSeleccionado--;
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::Blue);
        } else {
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::White);
            MainMenuSeleccionado = Max_menu1 - 1;
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::Blue);
        }
    }
}

void MainMenu::moverAbajo() {
    if (subMenuActivo) {
        if (SubMenuSeleccionado + 1 < Max_submenu) {
            subMenu[SubMenuSeleccionado].setFillColor(Color::White);
            SubMenuSeleccionado++;
            subMenu[SubMenuSeleccionado].setFillColor(Color::Blue);
        } else {
            subMenu[SubMenuSeleccionado].setFillColor(Color::White);
            SubMenuSeleccionado = 0;
            subMenu[SubMenuSeleccionado].setFillColor(Color::Blue);
        }
    } else {
        if (MainMenuSeleccionado + 1 < Max_menu1) {
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::White);
            MainMenuSeleccionado++;
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::Blue);
        } else {
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::White);
            MainMenuSeleccionado = 0;
            menuPrincipal[MainMenuSeleccionado].setFillColor(Color::Blue);
        }
    }
}


