/**********************************************************************************
// BishopBoss.h - Boss Fase 3: O Bispo
//
// Comportamento:
//   Fase 1: Teleporta para uma das plataformas e dispara projéteis diagonais.
//   Fase 2: Dispara em 3 direções simultâneas (spray diagonal).
**********************************************************************************/
#ifndef _PEAOREBELDE_BISHOPBOSS_H_
#define _PEAOREBELDE_BISHOPBOSS_H_
#include "Boss.h"
class BishopBoss : public Boss {
private:
    float teleportTimer;
    void BehaviorUpdate() override;
    void Shoot(int pattern);    // 0=single, 1=triple spread
    void Teleport();
    Object* pawnRef;
public:
    BishopBoss(float posX, float posY, Object* pawn);
    ~BishopBoss() = default;
};
#endif

/**********************************************************************************
// QueenBoss.h - Boss Fase 4: A Rainha
//
// Comportamento:
//   Fase 1: Move-se lateralmente rápido + dispara 4 direções (cruz).
//   Fase 2: Dispara 8 direções (estrela) + dash em direção ao peão.
**********************************************************************************/
#ifndef _PEAOREBELDE_QUEENBOSS_H_
#define _PEAOREBELDE_QUEENBOSS_H_
#include "Boss.h"
class QueenBoss : public Boss {
private:
    float moveSpeed;
    bool  isDashing;
    float dashTimer;
    Object* pawnRef;
    void BehaviorUpdate() override;
    void ShootCross();
    void ShootStar();
    void Dash();
public:
    QueenBoss(float posX, float posY, Object* pawn);
    ~QueenBoss() = default;
};
#endif

/**********************************************************************************
// KingBoss.h - Boss Fase 5: O Rei (chefe final)
//
// Comportamento:
//   Fase 1: Invoca 2 lacaios + move devagar + golpe de bastão próximo.
//   Fase 2: Invoca 3 lacaios + faz dash curto + dispara projéteis em leque.
//           Fica imune enquanto houver lacaios vivos.
**********************************************************************************/
#ifndef _PEAOREBELDE_KINGBOSS_H_
#define _PEAOREBELDE_KINGBOSS_H_
#include "Boss.h"
#include <vector>
using std::vector;
class KingBoss : public Boss {
private:
    float summonTimer;
    int   maxMinions;
    int   activeMinions;
    bool  isImmune;         // imune enquanto tiver lacaios
    float scepterTimer;     // ataque de bastão (área próxima)
    Object* pawnRef;
    void BehaviorUpdate() override;
    void SummonMinion();
    void ScepterAttack();
    void UpdateImmunity();
public:
    int  GetActiveMinions() const { return activeMinions; }
    void MinionDied() { if (activeMinions > 0) activeMinions--; }
    void TakeDamage(int amount) override;
    KingBoss(float posX, float posY, Object* pawn);
    ~KingBoss() = default;
};
#endif
