/**********************************************************************************
// PeaoRebelde.cpp - Ponto de entrada do jogo
//
// Descrição: WinMain, definição das variáveis globais e inicialização da engine.
**********************************************************************************/

#include "Engine.h"
#include "PeaoRebelde.h"
#include "Resources.h"
#include "Home.h"

// ---------------------------------------------------------------------------------
// Definição das variáveis globais

Scene*      gScene          = nullptr;
Audio*      gAudio          = nullptr;

int         gCurrentStage   = 1;
int         gPawnHearts     = PAWN_START_HEARTS;
bool        gHasRangedAttack = false;
long        gScore          = 0;

ControlMode gControlMode    = CTRL_KEYBOARD_MOUSE;
extern bool gViewBBox;

// ---------------------------------------------------------------------------------

//int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//    Engine* engine = new Engine();
//
//    engine->window->Mode(WINDOWED);
//    engine->window->Size(WINDOW_WIDTH, WINDOW_HEIGHT);
//    engine->window->Color(10, 10, 20);          // fundo escuro padrão
//    engine->window->Title("Peão Rebelde");
//    engine->window->Icon(IDI_ICON);
//
//    engine->Start(new Home());
//
//    delete engine;
//    return 0;
//}
