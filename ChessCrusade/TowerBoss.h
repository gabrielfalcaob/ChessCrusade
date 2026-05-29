/**********************************************************************************
// TowerBoss.h - Boss Fase 1: A Torre
//
// Comportamento:
//   Fase 1 (HP cheio → metade): Atira projéteis horizontais em intervalos.
//   Fase 2 (metade → 0): Atira mais rápido + se move lateralmente.
**********************************************************************************/

#ifndef _PEAOREBELDE_TOWERBOSS_H_
#define _PEAOREBELDE_TOWERBOSS_H_

#include "Boss.h"

// ---------------------------------------------------------------------------------

class TowerBoss : public Boss
{
private:
    float shootInterval;     // intervalo entre tiros
    float moveSpeed;         // velocidade lateral (fase 2)

    void BehaviorUpdate() override;
    void Shoot();

public:
    TowerBoss(float posX, float posY);
    ~TowerBoss() = default;
};

// ---------------------------------------------------------------------------------
#endif
