/**********************************************************************************
// TowerBoss.cpp
**********************************************************************************/

#include "TowerBoss.h"
#include "BossProjectile.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

TowerBoss::TowerBoss(float posX, float posY)
    : Boss(posX, posY, 10, 80.0f, 100.0f)   // 10 HP, 80x100 pixels
{
    shootInterval = 2.5f;
    moveSpeed     = 0.0f;   // estática na fase 1
    attackTimer   = shootInterval;
}

// ---------------------------------------------------------------------------------

void TowerBoss::Shoot()
{
    // Atira na direção do jogador (sempre à esquerda nessa fase)
    float dirX = -1.0f;
    BossProjectile* p = new BossProjectile(x - halfW, y, dirX, 0.0f, 1);
    if (gScene) gScene->Add(p, MOVING);
}

// ---------------------------------------------------------------------------------

void TowerBoss::BehaviorUpdate()
{
    // Fase 2: começa a se mover lateralmente
    if (bossPhase == BOSS_PHASE2)
    {
        moveSpeed = 80.0f;
        shootInterval = 1.5f;   // atira mais rápido

        velX = facingRight ? moveSpeed : -moveSpeed;

        // Inverte nas bordas (limita entre centro e borda direita da tela)
        if (x - halfW < window->CenterX() + 20.0f) facingRight = true;
        if (x + halfW > window->Width() - 40.0f)   facingRight = false;
    }
    else
    {
        velX = 0.0f;
    }

    // Atirar em intervalo
    if (attackTimer <= 0.0f)
    {
        Shoot();
        attackTimer = shootInterval;
    }
}
