/**********************************************************************************
// BossProjectile.h - Projétil disparado pelos bosses
**********************************************************************************/

#ifndef _PEAOREBELDE_BOSSPROJECTILE_H_
#define _PEAOREBELDE_BOSSPROJECTILE_H_

#include "Object.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

class BossProjectile : public Object
{
private:
    float velX, velY;
    int   damage;
    float lifetime;     // se passar de X segundos, se auto-destroi

public:
    BossProjectile(float posX, float posY, float dirX, float dirY, int dmg,
                   float speed = 450.0f);
    ~BossProjectile() = default;

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
