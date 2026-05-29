/**********************************************************************************
// Platform.cpp
**********************************************************************************/

#include "Platform.h"
#include "Pawn.h"
#include "Enemy.h"
#include "Boss.h"
#include "Engine.h"
#include <cmath>

// ---------------------------------------------------------------------------------

Platform::Platform(float posX, float posY, float width, float height, const char* spritePath)
{
    MoveTo(posX, posY, Layer::UPPER);
    type = OBJ_PLATFORM;
    halfW = width * 0.5f;
    halfH = height * 0.5f;
    sprite = spritePath ? new Sprite(spritePath) : nullptr;

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Platform::~Platform()
{
    delete sprite;
}

// ---------------------------------------------------------------------------------

void Platform::Update() {}

// ---------------------------------------------------------------------------------

void Platform::Draw()
{
    if (sprite)
    {
        sprite->Draw(x, y, z);
        return;
    }
    // Sem sprite: não desenha nada aqui.
    // O Stage chama DrawAll() que faz um único BeginPixels/EndPixels para tudo.
}

// ---------------------------------------------------------------------------------

void Platform::OnCollision(Object* obj)
{
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;
        if (pawn->droppingThrough) return;
        if (pawn->velY > 0.0f)
        {
            float prevBottom = (pawn->Y() + pawn->halfH) - pawn->velY * gameTime;
            if (prevBottom <= this->Top() + DROP_PLATFORM_THRESHOLD)
            {
                pawn->MoveTo(pawn->X(), this->Top() - pawn->halfH);
                pawn->velY = 0.0f;
                pawn->onGround = true;
            }
        }
        return;
    }

    if (obj->Type() == OBJ_ENEMY)
    {
        Enemy* enemy = (Enemy*)obj;
        if (enemy->velY > 0.0f)
        {
            float prevBottom = (enemy->Y() + enemy->halfH) - enemy->velY * gameTime;
            if (prevBottom <= this->Top() + DROP_PLATFORM_THRESHOLD)
            {
                enemy->MoveTo(enemy->X(), this->Top() - enemy->halfH);
                enemy->velY = 0.0f;
                enemy->onGround = true;
            }
        }
        return;
    }

    if (obj->Type() == OBJ_BOSS)
    {
        Boss* boss = (Boss*)obj;
        if (boss->velY > 0.0f)
        {
            Rect* bbox = (Rect*)boss->BBox();
            if (!bbox) return;
            float bossHalfH = (bbox->Bottom() - bbox->Top()) * 0.5f;
            float prevBottom = (boss->Y() + bossHalfH) - boss->velY * gameTime;
            if (prevBottom <= this->Top() + DROP_PLATFORM_THRESHOLD)
            {
                boss->MoveTo(boss->X(), this->Top() - bossHalfH);
                boss->velY = 0.0f;
                boss->onGround = true;
            }
        }
        return;
    }
}