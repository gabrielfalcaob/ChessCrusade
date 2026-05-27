/**********************************************************************************
// Platform.cpp
**********************************************************************************/

#include "Platform.h"
#include "Pawn.h"
#include "Enemy.h"
#include "Engine.h"
#include <cmath>

// ---------------------------------------------------------------------------------

Platform::Platform(float posX, float posY, float w, float h, const char* spritePath)
{
    MoveTo(posX, posY, Layer::UPPER);
    type   = OBJ_PLATFORM;
    halfW  = w * 0.5f;
    halfH  = h * 0.5f;
    sprite = nullptr;

    if (spritePath)
        sprite = new Sprite(spritePath);

    // bounding box retangular centrada na posição
    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Platform::~Platform()
{
    delete sprite;
}

// ---------------------------------------------------------------------------------

void Platform::Update()
{
    // plataforma estática — sem lógica
}

// ---------------------------------------------------------------------------------

void Platform::Draw()
{
    if (sprite)
    {
        sprite->Draw(x, y, z);
    }
    else
    {
        // Placeholder: retângulo cinza escuro desenhado via pixels
        Engine::renderer->BeginPixels();
        Rect* r = (Rect*)BBox();
        // desenha borda da plataforma em cinza
        Engine::renderer->Draw(r, 0xFF888888);
        Engine::renderer->EndPixels();
    }
}

// ---------------------------------------------------------------------------------

void Platform::OnCollision(Object* obj)
{
    // Trata colisão com o Peão
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;

        // O peão está pressionando baixo → atravessa a plataforma
        if (pawn->droppingThrough)
            return;

        // Só pousa se o peão está DESCENDO e seu fundo estava ACIMA do topo da plataforma
        if (pawn->velY > 0.0f)
        {
            float pawnBottom = pawn->Y() + pawn->halfH;
            float prevBottom = pawnBottom - pawn->velY * gameTime;

            // só colide se estava acima do topo antes
            if (prevBottom <= this->Top() + DROP_PLATFORM_THRESHOLD)
            {
                pawn->MoveTo(pawn->X(), this->Top() - pawn->halfH);
                pawn->velY    = 0.0f;
                pawn->onGround = true;
            }
        }
    }

    // Trata colisão com inimigos (para no chão)
    if (obj->Type() == OBJ_ENEMY)
    {
        Enemy* enemy = (Enemy*)obj;
        if (enemy->velY > 0.0f)
        {
            float enemyBottom = enemy->Y() + enemy->halfH;
            float prevBottom  = enemyBottom - enemy->velY * gameTime;
            if (prevBottom <= this->Top() + DROP_PLATFORM_THRESHOLD)
            {
                enemy->MoveTo(enemy->X(), this->Top() - enemy->halfH);
                enemy->velY    = 0.0f;
                enemy->onGround = true;
            }
        }
    }
}
