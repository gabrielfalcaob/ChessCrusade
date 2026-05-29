/**********************************************************************************
// Bosses345.cpp - Implementação dos Bosses: Bispo, Rainha e Rei
**********************************************************************************/

#include "Bosses345.h"
#include "BossProjectile.h"
#include "Enemy.h"
#include "Engine.h"
#include <cmath>
#include <cstdlib>

// =============================================================================
// BISHOP BOSS - Teleporta e dispara diagonais
// =============================================================================

BishopBoss::BishopBoss(float posX, float posY, Object* pawn)
    : Boss(posX, posY, 12, 70.0f, 90.0f)
{
    pawnRef       = pawn;
    teleportTimer = 3.0f;
    attackTimer   = 2.0f;
}

void BishopBoss::Teleport()
{
    if (!pawnRef) return;

    // Teleporta para o lado oposto ao peão
    float newX = (pawnRef->X() < window->CenterX())
        ? window->Width()  - 150.0f     // peão à esquerda → boss vai pra direita
        : 150.0f;                        // peão à direita  → boss vai pra esquerda

    // Mantém altura atual (ou pousa no chão se estiver voando)
    float newY = window->Height() - 60.0f - halfH;
    MoveTo(newX, newY);
    velX = 0.0f;
    velY = 0.0f;
}

void BishopBoss::Shoot(int pattern)
{
    if (!pawnRef) return;

    // Direção base em diagonal em direção ao peão
    float dx = pawnRef->X() - x;
    float dy = pawnRef->Y() - y;
    float len = sqrtf(dx*dx + dy*dy);
    if (len < 1.0f) return;
    dx /= len;
    dy /= len;

    if (pattern == 0)
    {
        // Tiro único diagonal
        gScene->Add(new BossProjectile(x, y, dx, dy, 1, 380.0f), MOVING);
    }
    else
    {
        // Spray de 3 direções (fase 2)
        gScene->Add(new BossProjectile(x, y, dx, dy, 1, 400.0f), MOVING);

        // +30° e -30°
        float angle1 =  0.524f;   // 30° em radianos
        float angle2 = -0.524f;
        float cos1 = cosf(angle1), sin1 = sinf(angle1);
        float cos2 = cosf(angle2), sin2 = sinf(angle2);
        gScene->Add(new BossProjectile(x, y, dx*cos1-dy*sin1, dx*sin1+dy*cos1, 1, 380.0f), MOVING);
        gScene->Add(new BossProjectile(x, y, dx*cos2-dy*sin2, dx*sin2+dy*cos2, 1, 380.0f), MOVING);
    }
}

void BishopBoss::BehaviorUpdate()
{
    velX = 0.0f;    // bispo não se move, teleporta

    // Teleporte periódico
    teleportTimer -= gameTime;
    if (teleportTimer <= 0.0f)
    {
        Teleport();
        teleportTimer = (bossPhase == BOSS_PHASE2) ? 2.0f : 3.5f;
    }

    // Ataque
    if (attackTimer <= 0.0f)
    {
        Shoot(bossPhase == BOSS_PHASE2 ? 1 : 0);
        attackTimer = (bossPhase == BOSS_PHASE2) ? 1.2f : 2.0f;
    }
}

// =============================================================================
// QUEEN BOSS - Se move rápido + dispara em padrão de cruz/estrela
// =============================================================================

QueenBoss::QueenBoss(float posX, float posY, Object* pawn)
    : Boss(posX, posY, 18, 85.0f, 100.0f)
{
    pawnRef    = pawn;
    moveSpeed  = 160.0f;
    isDashing  = false;
    dashTimer  = 0.0f;
    attackTimer = 2.0f;
    facingRight = false;
}

void QueenBoss::ShootCross()
{
    // 4 direções: cima, baixo, esquerda, direita
    gScene->Add(new BossProjectile(x, y,  1.0f,  0.0f, 1, 350.0f), MOVING);
    gScene->Add(new BossProjectile(x, y, -1.0f,  0.0f, 1, 350.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  0.0f,  1.0f, 1, 350.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  0.0f, -1.0f, 1, 350.0f), MOVING);
}

void QueenBoss::ShootStar()
{
    // 8 direções
    float d = 0.707f;   // cos/sin de 45°
    gScene->Add(new BossProjectile(x, y,  1.0f,  0.0f, 1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y, -1.0f,  0.0f, 1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  0.0f,  1.0f, 1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  0.0f, -1.0f, 1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  d,     d,    1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y, -d,     d,    1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y,  d,    -d,    1, 380.0f), MOVING);
    gScene->Add(new BossProjectile(x, y, -d,    -d,    1, 380.0f), MOVING);
}

void QueenBoss::Dash()
{
    if (!pawnRef) return;
    float dir = (pawnRef->X() < x) ? -1.0f : 1.0f;
    velX = dir * 600.0f;
    isDashing = true;
    dashTimer = 0.35f;
    facingRight = (dir > 0);
}

void QueenBoss::BehaviorUpdate()
{
    // Fase 2: mais rápida
    if (bossPhase == BOSS_PHASE2)
        moveSpeed = 260.0f;

    if (isDashing)
    {
        dashTimer -= gameTime;
        if (dashTimer <= 0.0f)
        {
            velX = 0.0f;
            isDashing = false;
            attackTimer = 1.5f;
        }
        // Bate nas bordas
        if (x - halfW < 20.0f || x + halfW > window->Width() - 20.0f)
            velX = -velX;
        return;
    }

    // Movimento de patrulha lateral normal
    velX = facingRight ? moveSpeed : -moveSpeed;
    if (x - halfW < 20.0f)              facingRight = true;
    if (x + halfW > window->Width()-20.0f) facingRight = false;

    // Ataque
    if (attackTimer <= 0.0f)
    {
        if (bossPhase == BOSS_PHASE2)
        {
            if (rand() % 2 == 0) { Dash(); return; }
            ShootStar();
            attackTimer = 2.0f;
        }
        else
        {
            ShootCross();
            attackTimer = 2.5f;
        }
    }
}

// =============================================================================
// KING BOSS - Convoca lacaios + fica imune + golpe de bastão
// =============================================================================

KingBoss::KingBoss(float posX, float posY, Object* pawn)
    : Boss(posX, posY, 25, 95.0f, 115.0f)
{
    pawnRef       = pawn;
    summonTimer   = 4.0f;
    maxMinions    = 2;
    activeMinions = 0;
    isImmune      = false;
    scepterTimer  = 3.0f;
    attackTimer   = 2.5f;
    velX          = 0.0f;
}

void KingBoss::SummonMinion()
{
    if (!gScene) return;

    // Convoca um mini-rei ao lado do boss
    float spawnX = (rand() % 2 == 0)
        ? x - 200.0f
        : x + 200.0f;
    float spawnX2 = (spawnX < x) ? x + 200.0f : x - 200.0f;

    spawnX  = max(100.0f, min(spawnX,  (float)WINDOW_WIDTH - 100.0f));
    spawnX2 = max(100.0f, min(spawnX2, (float)WINDOW_WIDTH - 100.0f));

    Enemy* m1 = new Enemy(ENEMY_PAWN_KING, spawnX,  y + 50.0f,
                          spawnX  - 200.0f, spawnX  + 200.0f, 2, 1);
    gScene->Add(m1, MOVING);
    activeMinions++;

    if (bossPhase == BOSS_PHASE2)
    {
        Enemy* m2 = new Enemy(ENEMY_PAWN_KING, spawnX2, y + 50.0f,
                              spawnX2 - 200.0f, spawnX2 + 200.0f, 2, 1);
        gScene->Add(m2, MOVING);
        activeMinions++;
    }
}

void KingBoss::ScepterAttack()
{
    // Golpe de área próxima — cria projétil de curto alcance em ambos os lados
    gScene->Add(new BossProjectile(x - halfW - 30.0f, y, -1.0f, 0.0f, 2, 150.0f), MOVING);
    gScene->Add(new BossProjectile(x + halfW + 30.0f, y,  1.0f, 0.0f, 2, 150.0f), MOVING);
}

void KingBoss::UpdateImmunity()
{
    // Imune apenas na fase 2 quando tiver lacaios vivos
    isImmune = (bossPhase == BOSS_PHASE2 && activeMinions > 0);
}

void KingBoss::TakeDamage(int amount)
{
    if (isImmune) return;
    Boss::TakeDamage(amount);
}

void KingBoss::BehaviorUpdate()
{
    UpdateImmunity();

    // Rei se move devagar em direção ao peão
    if (pawnRef)
    {
        float dir = (pawnRef->X() < x) ? -1.0f : 1.0f;
        velX = dir * (bossPhase == BOSS_PHASE2 ? 80.0f : 50.0f);
        facingRight = (dir > 0);

        // Para em bordas
        if (x - halfW < 20.0f)                  velX =  50.0f;
        if (x + halfW > window->Width() - 20.0f) velX = -50.0f;
    }

    // Convocar lacaios periodicamente
    summonTimer -= gameTime;
    if (summonTimer <= 0.0f)
    {
        SummonMinion();
        summonTimer = (bossPhase == BOSS_PHASE2) ? 6.0f : 8.0f;
    }

    // Golpe de bastão
    scepterTimer -= gameTime;
    if (scepterTimer <= 0.0f)
    {
        ScepterAttack();
        scepterTimer = (bossPhase == BOSS_PHASE2) ? 2.0f : 3.0f;
    }

    // Fase 2: também dispara projéteis em leque
    if (bossPhase == BOSS_PHASE2 && attackTimer <= 0.0f)
    {
        float d = 0.707f;
        gScene->Add(new BossProjectile(x, y,  1.0f, 0.0f, 1, 300.0f), MOVING);
        gScene->Add(new BossProjectile(x, y, -1.0f, 0.0f, 1, 300.0f), MOVING);
        gScene->Add(new BossProjectile(x, y,  d,   -d,    1, 300.0f), MOVING);
        gScene->Add(new BossProjectile(x, y, -d,   -d,    1, 300.0f), MOVING);
        attackTimer = 3.0f;
    }
}
