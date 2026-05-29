/**********************************************************************************
// GameOver.cpp
**********************************************************************************/

#include "Engine.h"
#include "GameOver.h"
#include "Home.h"
#include "Stage1.h"

void GameOver::Init()
{
    // Vitória = chegou aqui após Stage5; derrota = pawn.isDead
    // Por ora simplificado: se chegar via Stage5 é vitória
    victory = (gCurrentStage > 5);
    // backg = new Sprite(victory ? "Resources/victory.png" : "Resources/gameover.png");
}

void GameOver::Finalize()
{
    delete backg;
    delete font;
}

void GameOver::Update()
{
    if (window->KeyPress(VK_RETURN))
        Engine::Next<Home>();

    if (window->KeyPress('R'))
    {
        // Reinicia do começo
        gCurrentStage    = 1;
        gPawnHearts      = PAWN_START_HEARTS;
        gHasRangedAttack = false;
        gScore           = 0;
        Engine::Next<Stage1>();
    }
}

void GameOver::Draw()
{
    if (backg)
        backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    else
    {
        Engine::renderer->BeginPixels();
        Rect box(window->CenterX() - 350.0f, 200.0f,
                 window->CenterX() + 350.0f, 380.0f);
        Engine::renderer->Draw(&box, victory ? 0xFF224422 : 0xFF441111);
        Engine::renderer->EndPixels();
    }
    // TODO: font->Draw(...)
    // "VOCÊ VENCEU! / GAME OVER"
    // "Pontuação: " + gScore
    // "ENTER - Menu  R - Reiniciar"
}
