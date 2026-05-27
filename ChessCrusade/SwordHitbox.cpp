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

    // hitbox retangular da espada: 50x40 pixels
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
    // Debug: desenha hitbox visível (remover ou deixar opção)
    // Engine::renderer->BeginPixels();
    // Rect* r = (Rect*)BBox(); r->MoveTo(x,y);
    // Engine::renderer->Draw(r, 0x88FFFF00);
    // Engine::renderer->EndPixels();
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
