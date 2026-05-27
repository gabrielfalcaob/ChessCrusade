/**********************************************************************************
// KnightBoss.cpp
**********************************************************************************/

#include "KnightBoss.h"
#include "BossProjectile.h"
#include "Engine.h"
#include <cmath>

// ---------------------------------------------------------------------------------

KnightBoss::KnightBoss(float posX, float posY, Object* pawn)
    : Boss(posX, posY, 15, 90.0f, 110.0f)
{
    pawnRef      = pawn;
    chargeSpeed  = 320.0f;
    isCharging   = false;
    chargeTimer  = 0.0f;
    idleTimer    = 1.5f;     // espera 1.5s antes de carregar
    attackTimer  = idleTimer;
}

// ---------------------------------------------------------------------------------

void KnightBoss::StartCharge()
{
    if (!pawnRef) return;
    float dir = (pawnRef->X() < x) ? -1.0f : 1.0f;
    velX       = dir * chargeSpeed;
    facingRight = (dir > 0);
    isCharging  = true;
    chargeTimer = 0.8f;     // carrega por 0.8s
}

// ---------------------------------------------------------------------------------

void KnightBoss::LeapAttack()
{
    if (!pawnRef) return;
    // Salto diagonal em direção ao peão (simula movimento em "L")
    float dir = (pawnRef->X() < x) ? -1.0f : 1.0f;
    velX = dir * 500.0f;
    velY = -600.0f;         // salta para cima
    onGround = false;
}

// ---------------------------------------------------------------------------------

void KnightBoss::BehaviorUpdate()
{
    // Fase 2: velocidade de carga aumenta
    if (bossPhase == BOSS_PHASE2)
        chargeSpeed = 480.0f;

    if (isCharging)
    {
        chargeTimer -= gameTime;

        // Limita nas bordas da tela
        if (x - halfW < 0.0f || x + halfW > window->Width())
            velX = -velX;

        if (chargeTimer <= 0.0f)
        {
            velX       = 0.0f;
            isCharging = false;
            attackTimer = (bossPhase == BOSS_PHASE2) ? 1.0f : 1.8f;
        }
    }
    else
    {
        velX = 0.0f;

        if (attackTimer <= 0.0f)
        {
            if (bossPhase == BOSS_PHASE2 && (rand() % 3 == 0))
                LeapAttack();
            else
                StartCharge();
        }
    }
}
