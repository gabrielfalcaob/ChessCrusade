/**********************************************************************************
// HeartDrop.h - Coração dropado por inimigo ao morrer
**********************************************************************************/

#ifndef _PEAOREBELDE_HEARTDROP_H_
#define _PEAOREBELDE_HEARTDROP_H_

#include "Object.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

class HeartDrop : public Object
{
private:
    float velY;
    float lifetime;
    bool  onGround;

public:
    bool collected;

    HeartDrop(float posX, float posY);
    ~HeartDrop() = default;

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
