/**********************************************************************************
// KnightBoss.h - Boss Fase 2: O Cavaleiro
//
// Comportamento:
//   Fase 1: Corre em direção ao peão e dá uma rasteira (hitbox larga no chão)
//   Fase 2: Intercala corrida com saltos diagonais (em "L", como o cavalo no xadrez)
**********************************************************************************/

#ifndef _PEAOREBELDE_KNIGHTBOSS_H_
#define _PEAOREBELDE_KNIGHTBOSS_H_

#include "Boss.h"

class KnightBoss : public Boss
{
private:
    float chargeSpeed;
    bool  isCharging;
    float chargeTimer;
    float idleTimer;

    Object* pawnRef;    // referência ao peão para perseguir

    void BehaviorUpdate() override;
    void StartCharge();
    void LeapAttack();  // salto em "L" (fase 2)

public:
    KnightBoss(float posX, float posY, Object* pawn);
    ~KnightBoss() = default;
};

#endif
