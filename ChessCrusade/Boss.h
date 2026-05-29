/**********************************************************************************
// Boss.h - Classe Base para todos os Bosses
//
// Descrição: Todo boss tem HP, barra de vida, fases de comportamento e
//            padrões de ataque. Cada fase herda e sobrescreve o comportamento.
**********************************************************************************/

#ifndef _PEAOREBELDE_BOSS_H_
#define _PEAOREBELDE_BOSS_H_

#include "Object.h"
#include "Animation.h"
#include "Font.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

enum BossPhase { BOSS_PHASE1, BOSS_PHASE2 };    // muda comportamento na metade do HP

// ---------------------------------------------------------------------------------

class Boss : public Object
{
protected:
    int          hp;
    int          maxHp;
    BossPhase    bossPhase;
    bool         isDead;
    float        iFrameTimer;       // invencibilidade após dano
    float        attackTimer;       // cooldown de ataque
    float        halfW, halfH;
    bool         facingRight;

    TileSet*   tileSet;
    Animation* anim;
    Font*      font;

    // Lógica de IA (sobrescrita por cada boss)
    virtual void BehaviorUpdate() = 0;

public:
    float velX, velY;
    bool  onGround;

    Boss(float posX, float posY, int maxHp, float w, float h);
    virtual ~Boss();

    bool IsAlive()  const { return !isDead; }
    bool IsDead()   const { return isDead;  }
    int  HP()       const { return hp; }
    int  MaxHP()    const { return maxHp; }

    virtual void TakeDamage(int amount);

    // Desenha a barra de HP do boss no topo da tela
    void DrawHealthBar();

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
