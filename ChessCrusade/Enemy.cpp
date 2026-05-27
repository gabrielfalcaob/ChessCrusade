/**********************************************************************************
// Enemy.cpp
**********************************************************************************/

#include "Enemy.h"
#include "Pawn.h"
#include "HeartDrop.h"
#include "Engine.h"
#include <cmath>
#include <cstdlib>  // rand

using namespace std;

// ---------------------------------------------------------------------------------

Enemy::Enemy(EnemyVariant v, float posX, float posY,
             float pLeft, float pRight, int hp_, int contactDmg)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type        = OBJ_ENEMY;
    variant     = v;
    hp          = hp_;
    maxHp       = hp_;
    contactDamage = contactDmg;
    patrolLeft  = pLeft;
    patrolRight = pRight;
    facingRight = true;
    onGround    = false;
    iFrameTimer = 0.0f;
    velX = 0.0f;
    velY = 0.0f;

    // Tamanho depende do tipo (placeholder)
    halfW = 24.0f;
    halfH = 32.0f;

    switch (variant)
    {
    case ENEMY_PAWN:        moveSpeed = 120.0f; break;
    case ENEMY_PAWN_KNIGHT: moveSpeed = 180.0f; halfW = 28.0f; break;
    case ENEMY_PAWN_BISHOP: moveSpeed = 100.0f; halfH = 38.0f; break;
    case ENEMY_PAWN_ROOK:   moveSpeed =  80.0f; halfW = 30.0f; halfH = 36.0f; break;
    case ENEMY_PAWN_KING:   moveSpeed = 100.0f; halfW = 28.0f; halfH = 36.0f; break;
    }

    tileSet = nullptr;
    anim    = nullptr;
    // TODO: tileSet = new TileSet("Resources/enemy_pawn.png", ...);

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Enemy::~Enemy()
{
    delete anim;
    delete tileSet;
}

// ---------------------------------------------------------------------------------

void Enemy::TakeDamage(int amount)
{
    if (iFrameTimer > 0.0f) return;
    hp -= amount;
    iFrameTimer = 0.3f;     // 0.3s de invencibilidade para não tomar dano repetido

    if (hp <= 0)
    {
        hp = 0;
        if (gAudio) gAudio->Play(SND_ENEMY_DEATH);
        gScore += 200;

        // Drop de coração: 40% de chance
        if (rand() % 100 < 40)
        {
            HeartDrop* heart = new HeartDrop(x, y);
            if (gScene) gScene->Add(heart, MOVING);
        }

        if (gScene) gScene->Delete(this, MOVING);
    }
}

// ---------------------------------------------------------------------------------

void Enemy::ApplyPhysics()
{
    // Patrulha: move na direção atual
    velX = facingRight ? moveSpeed : -moveSpeed;

    // Inverte ao atingir bordas da patrulha
    if (x <= patrolLeft)  { facingRight = true;  }
    if (x >= patrolRight) { facingRight = false; }

    // Gravidade
    if (!onGround)
        velY += PAWN_GRAVITY * gameTime;

    Translate(velX * gameTime, velY * gameTime);

    // Fallback de chão
    if (y + halfH >= window->Height() - 10.0f)
    {
        MoveTo(x, window->Height() - 10.0f - halfH);
        velY    = 0.0f;
        onGround = true;
    }

    // A cada frame, assume no ar até Platform confirmar
    onGround = false;
}

// ---------------------------------------------------------------------------------

void Enemy::Update()
{
    if (iFrameTimer > 0.0f) iFrameTimer -= gameTime;
    ApplyPhysics();
}

// ---------------------------------------------------------------------------------

void Enemy::Draw()
{
    // Pisca ao tomar dano
    bool visible = true;
    if (iFrameTimer > 0.0f)
    {
        int frame = (int)(iFrameTimer * 10.0f);
        visible = (frame % 2 == 0);
    }
    if (!visible) return;

    if (anim)
    {
        anim->Draw(x, y, z);
    }
    else
    {
        // ---- PLACEHOLDER ----
        // Cor diferente por tipo
        DWORD color = 0xFF333388;   // azul escuro (peão padrão)
        switch (variant)
        {
        case ENEMY_PAWN:        color = 0xFF222266; break;
        case ENEMY_PAWN_KNIGHT: color = 0xFF224422; break;  // verde escuro
        case ENEMY_PAWN_BISHOP: color = 0xFF442222; break;  // vermelho escuro
        case ENEMY_PAWN_ROOK:   color = 0xFF444422; break;  // amarelo escuro
        case ENEMY_PAWN_KING:   color = 0xFF442244; break;  // roxo escuro
        }

        Engine::renderer->BeginPixels();
        Rect body(-halfW, -halfH, halfW, halfH);
        body.MoveTo(x, y);
        Engine::renderer->Draw(&body, color);
        Engine::renderer->EndPixels();
    }
}

// ---------------------------------------------------------------------------------

void Enemy::OnCollision(Object* obj)
{
    // Causa dano ao peão ao tocá-lo
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;
        pawn->TakeDamage(contactDamage);
    }
}
