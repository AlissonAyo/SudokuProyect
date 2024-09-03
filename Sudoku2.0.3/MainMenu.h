#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;
#define Max_menu1 4
#define Max_submenu 5

class MainMenu {
public:
    MainMenu(float width, float height);
    void dibujar(RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int menuPrincipalSeleccionado() {
        return MainMenuSeleccionado;
    }
    bool esSubMenuActivo() const {
        return subMenuActivo;
    }
    void setSubMenu(bool activo) {
        subMenuActivo = activo;
    }
    int menuSubMenuSeleccionado() {
        return SubMenuSeleccionado;
    }
    ~MainMenu();
public:
    void inicializarSubMenu(float width);
    int MainMenuSeleccionado;
    int SubMenuSeleccionado;
    bool subMenuActivo;
    Font fuente;
    Text menuPrincipal[Max_menu1];
    Text subMenu[Max_submenu];
};
#endif
