/**********************************************************************************
// PawnProjectile.h - Projétil do peão (ataque especial desbloqueável)
**********************************************************************************/

#ifndef _PEAOREBELDE_PAWNPROJECTILE_H_
#define _PEAOREBELDE_PAWNPROJECTILE_H_

#include "Object.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

class PawnProjectile : public Object
{
private:
    float velX;
    float lifetime;

public:
    PawnProjectile(float posX, float posY, float dirX, float speed = 650.0f);
    ~PawnProjectile() = default;

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
