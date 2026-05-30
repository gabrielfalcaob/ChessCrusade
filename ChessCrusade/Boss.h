#ifndef _PEAOREBELDE_BOSS_H_
#define _PEAOREBELDE_BOSS_H_

#include "Object.h"
#include "Animation.h"
#include "Sprite.h"
#include "Font.h"
#include "PeaoRebelde.h"

enum BossPhase {
    BOSS_PHASE1,
    BOSS_PHASE2
};

class Boss : public Object {
protected:
    int       hp;
    int       maxHp;
    BossPhase bossPhase;

    float     iFrameTimer;
    float     attackTimer;
    float     halfW, halfH;

    bool      facingRight;

    TileSet* tileSet;
    Animation* anim;
    Font* font;

    virtual void BehaviorUpdate() = 0;

public:
    // Variáveis físicas que KnightBoss e TowerBoss alteram diretamente
    float velX, velY;
    bool  onGround;
    bool  isDead;

    Boss(float posX, float posY, int maxHp_, float w, float h);
    virtual ~Boss();

    // Métodos virtuais com as maiúsculas exatas exigidas pelas fases e HUD
    virtual void TakeDamage(int amount);

    int  HP() const { return hp; }
    int  MaxHP() const { return maxHp; }
    bool IsDead() const { return isDead; }
    bool IsAlive() const { return !isDead; }

    void Update()     override;
    void Draw()       override;
    void OnCollision(Object* obj) override;
};

#endif