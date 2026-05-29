/**********************************************************************************
// Projectile (Código Fonte)
// 
// Criação:     01 Out 2012
// Atualização: 20 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Movimento de projétil

**********************************************************************************/

#include "Engine.h"
#include "PeaoRebelde.h"
#include "Home.h"

// bool gViewBBox = false;
// Esta função WinMain original vai apenas disparar o seu jogo real
// 
//int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//    Engine* engine = new Engine();
//
//    engine->window->Mode(WINDOWED);
//    engine->window->Size(WINDOW_WIDTH, WINDOW_HEIGHT); // 1600x860
//    engine->window->Color(10, 10, 20);
//    engine->window->Title("Peão Rebelde");
//    engine->window->Icon(101); // IDI_ICON padrão da engine
//
//    // Inicializa o jogo chamando a sua Tela Inicial real!
//    engine->Start(new Home());
//
//    delete engine;
//    return 0;
//}