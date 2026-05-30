/**********************************************************************************
// Boss.cpp - Classe Base dos Bosses (Versão Corrigida para HUD)
**********************************************************************************/

#include "Boss.h"
#include "Pawn.h"
#include "Engine.h"
#include <algorithm>

using namespace std;

// ---------------------------------------------------------------------------------
Boss::Boss(float posX, float posY, int maxHp_, float w, float h)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type = OBJ_BOSS;
    hp = maxHp_;
    maxHp = maxHp_;
    bossPhase = BOSS_PHASE1;
    isDead = false;
    iFrameTimer = 0.0f;
    attackTimer = 2.0f;
    halfW = w * 0.5f;
    halfH = h * 0.5f;
    facingRight = false;
    velX = 0.0f;
    velY = 0.0f;
    onGround = false;
    tileSet = nullptr;
    anim = nullptr;
    font = nullptr;

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------
Boss::~Boss()
{
    delete anim;
    delete tileSet;
    delete font;
}

// ---------------------------------------------------------------------------------
void Boss::TakeDamage(int amount)
{
    if (iFrameTimer > 0.0f || isDead) return;

    hp -= amount;
    iFrameTimer = 0.5f;

    if (gAudio) gAudio->Play(SND_BOSS_HURT);

    if (hp <= maxHp / 2 && bossPhase == BOSS_PHASE1)
        bossPhase = BOSS_PHASE2;

    if (hp <= 0)
    {
        hp = 0;
        isDead = true;
        if (gAudio) gAudio->Play(SND_BOSS_DEATH);
        gScore += 1000;
    }
}

// ---------------------------------------------------------------------------------
void Boss::Update()
{
    if (iFrameTimer > 0.0f) iFrameTimer -= gameTime;
    if (attackTimer > 0.0f) attackTimer -= gameTime;

    if (!isDead)
    {
        if (!onGround)
            velY += PAWN_GRAVITY * 0.8f * gameTime;

        Translate(velX * gameTime, velY * gameTime);

        // Trava de segurança usando Engine::window
        if (y + halfH >= Engine::window->Height() - 10.0f)
        {
            MoveTo(x, Engine::window->Height() - 10.0f - halfH);
            velY = 0.0f;
            onGround = true;
        }

        onGround = false;   // Platform confirma via OnCollision

        BehaviorUpdate();
    }
}

// ---------------------------------------------------------------------------------
void Boss::Draw()
{
    if (isDead) return;

    // Pisca se estiver invencível
    bool visible = !(iFrameTimer > 0.0f && (int)(iFrameTimer * 10.0f) % 2 == 1);
    if (!visible) return;

    if (anim)
    {
        anim->Draw(x, y, z);
    }
    else
    {
        // Placeholder geométrico do corpo
        Rect body(-halfW, -halfH, halfW, halfH);
        body.MoveTo(x, y);
        Engine::renderer->Draw(&body, bossPhase == BOSS_PHASE1 ? 0xFFAAAAAA : 0xFFFF6644);
    }

    // A Barra de Vida NÃO é desenhada aqui. A HUD cuida disso textualmente!
}

// ---------------------------------------------------------------------------------
void Boss::OnCollision(Object* obj)
{
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;
        pawn->TakeDamage(1);
    }
}