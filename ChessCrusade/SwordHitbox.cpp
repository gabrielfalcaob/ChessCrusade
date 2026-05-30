/**********************************************************************************
// SwordHitbox.cpp
**********************************************************************************/

#include "SwordHitbox.h"
#include "Enemy.h"
#include "Boss.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

SwordHitbox::SwordHitbox(float posX, float posY, int dmg, float dur)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type     = OBJ_SWORD_HIT;
    damage   = dmg;
    duration = dur;
    hasHit   = false;

    // Hitbox da espada: 50x40 pixels
    BBox(new Rect(-25.0f, -20.0f, 25.0f, 20.0f));
}

// ---------------------------------------------------------------------------------

void SwordHitbox::Update()
{
    duration -= gameTime;
    if (duration <= 0.0f)
        gScene->Delete(this, MOVING);
}

// ---------------------------------------------------------------------------------

void SwordHitbox::Draw()
{
    // Desenha a hitbox da espada em amarelo semi-transparente (sempre visível no modo debug)
    Rect r(-25.0f, -20.0f, 25.0f, 20.0f);
    r.MoveTo(x, y);
    Engine::renderer->Draw(&r, 0xFFFFFF00);  // amarelo
}

// ---------------------------------------------------------------------------------

void SwordHitbox::OnCollision(Object* obj)
{
    if (hasHit) return;

    if (obj->Type() == OBJ_ENEMY)
    {
        Enemy* e = (Enemy*)obj;
        e->TakeDamage(damage);
        hasHit = true;
        if (gAudio) gAudio->Play(SND_SWORD_HIT);
        gScore += 50;
    }

    if (obj->Type() == OBJ_BOSS)
    {
        Boss* b = (Boss*)obj;
        b->TakeDamage(damage);
        hasHit = true;
        if (gAudio) gAudio->Play(SND_SWORD_HIT);
        gScore += 100;
    }
}
