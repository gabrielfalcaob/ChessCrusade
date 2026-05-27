/**********************************************************************************
// Enemy.h - Lacaio Inimigo (base para todas as variações)
//
// Descrição: Inimigo simples que patrulha a plataforma e causa dano
//            ao tocar no peão. Ao morrer, pode dropar coração.
//            Cada fase instancia o subtipo correto com sprites diferentes.
**********************************************************************************/

#ifndef _PEAOREBELDE_ENEMY_H_
#define _PEAOREBELDE_ENEMY_H_

#include "Object.h"
#include "Animation.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

enum EnemyVariant
{
    ENEMY_PAWN,       // Fase 1 – peão inimigo (preto)
    ENEMY_PAWN_KNIGHT,// Fase 2 – mini-cavaleiro
    ENEMY_PAWN_BISHOP,// Fase 3 – mini-bispo
    ENEMY_PAWN_ROOK,  // Fase 4 – mini-torre
    ENEMY_PAWN_KING,  // Fase 5 – mini-rei
};

// ---------------------------------------------------------------------------------

class Enemy : public Object
{
protected:
    EnemyVariant variant;
    int          hp;
    int          maxHp;
    int          contactDamage;     // dano ao tocar no peão
    float        moveSpeed;
    bool         facingRight;
    float        patrolLeft;        // limites de patrulha
    float        patrolRight;

    float        iFrameTimer;       // invencibilidade após dano (evita dano por frame)

    TileSet*     tileSet;
    Animation*   anim;

    void ApplyPhysics();

public:
    // Exposto para Platform acessar
    float velX, velY;
    bool  onGround;
    float halfW, halfH;

    Enemy(EnemyVariant v, float posX, float posY,
          float patrolLeft, float patrolRight,
          int hp = 2, int contactDmg = 1);
    virtual ~Enemy();

    bool IsAlive() const { return hp > 0; }
    virtual void TakeDamage(int amount);

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
