/**********************************************************************************
// Home.cpp - Tela Inicial
**********************************************************************************/

#include "Engine.h"
#include "Home.h"
#include "Stage1.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

void Home::Init()
{
    // backg = new Sprite("Resources/title_screen.png");
    // font  = new Font("Resources/font.png");

    // Reset completo ao voltar ao menu
    gCurrentStage    = 1;
    gPawnHearts      = PAWN_START_HEARTS;
    gHasRangedAttack = false;
    gScore           = 0;

    blinkTimer = 0.0f;
    showPrompt = true;
}

// ---------------------------------------------------------------------------------

void Home::Finalize()
{
    /*delete backg;
    delete font;*/
}

// ---------------------------------------------------------------------------------

void Home::Update()
{
    if (window->KeyPress(VK_ESCAPE))
        window->Close();

    // Qualquer tecla inicia o jogo
    if (window->KeyPress(VK_RETURN) || window->KeyPress(VK_SPACE)) {
        Engine::Next<Stage1>();
        return;
    }
        

    // Selecionar modo de controle
    if (window->KeyPress('1')) gControlMode = CTRL_KEYBOARD_MOUSE;
    if (window->KeyPress('2')) gControlMode = CTRL_KEYBOARD_ONLY;
    if (window->KeyPress('3')) gControlMode = CTRL_XBOX;

    // Piscar o "pressione ENTER"
    blinkTimer += gameTime;
    if (blinkTimer >= 0.5f) { blinkTimer = 0.0f; showPrompt = !showPrompt; }
}

// ---------------------------------------------------------------------------------

void Home::Draw()
{
    if (backg)
        backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    else
    {
        // Placeholder: título desenhado como retângulo
        Engine::renderer->BeginPixels();
        Rect title(window->CenterX() - 300.0f, 150.0f,
                   window->CenterX() + 300.0f, 250.0f);
        Engine::renderer->Draw(&title, 0xFFFFFFFF);
        Engine::renderer->EndPixels();
    }

    // TODO: desenhar texto com font quando disponível
    // "PEÃO REBELDE"
    // "PRESSIONE ENTER PARA JOGAR"
    // "1 - Teclado+Mouse  2 - Teclado  3 - Xbox"
}
