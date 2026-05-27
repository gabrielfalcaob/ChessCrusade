/**********************************************************************************
// SwordHitbox.h - Hitbox Temporária da Espadada
//
// Descrição: Objeto invisível que existe por um curto tempo e aplica dano
//            em inimigos e no boss ao colidir. Removido automaticamente.
**********************************************************************************/

#ifndef _PEAOREBELDE_SWORDHITBOX_H_
#define _PEAOREBELDE_SWORDHITBOX_H_

#include "Object.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

class SwordHitbox : public Object
{
private:
    float duration;     // tempo restante de vida
    int   damage;       // dano que aplica
    bool  hasHit;       // já acertou (para não acertar duas vezes no mesmo frame)

public:
    SwordHitbox(float posX, float posY, int dmg, float dur);
    ~SwordHitbox() = default;

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
