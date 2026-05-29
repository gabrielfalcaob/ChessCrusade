/**********************************************************************************
// PeaoRebelde.cpp - Ponto de entrada do jogo
**********************************************************************************/

#include "Engine.h"
#include "PeaoRebelde.h"
#include "Resources.h"
#include "Home.h"

// ---------------------------------------------------------------------------------
// Definição das variáveis globais

Scene*      gScene           = nullptr;
Audio*      gAudio           = nullptr;

int         gCurrentStage    = 1;
int         gPawnHearts      = PAWN_START_HEARTS;
bool        gHasRangedAttack = false;
long        gScore           = 0;

ControlMode gControlMode     = CTRL_KEYBOARD_ONLY;
bool        gViewBBox        = true;    // começa com BBox visível para testes

// ---------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    Engine* engine = new Engine();

    engine->window->Mode(WINDOWED);
    engine->window->Size(WINDOW_WIDTH, WINDOW_HEIGHT);
    engine->window->Color(0, 0, 0);        // tela preta
    engine->window->Title("Chess Crusade");
    engine->window->Icon(IDI_ICON);

    engine->Start(new Home());

    delete engine;
    return 0;
}
