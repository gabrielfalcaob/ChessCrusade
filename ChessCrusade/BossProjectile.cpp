/**********************************************************************************
// BossProjectile.cpp
**********************************************************************************/

#include "BossProjectile.h"
#include "Pawn.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

BossProjectile::BossProjectile(float posX, float posY,
                               float dirX, float dirY,
                               int dmg, float speed)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type     = OBJ_PROJECTILE;
    damage   = dmg;
    lifetime = 4.0f;
    velX     = dirX * speed;
    velY     = dirY * speed;

    BBox(new Circle(10.0f));
}

// ---------------------------------------------------------------------------------

void BossProjectile::Update()
{
    Translate(velX * gameTime, velY * gameTime);
    lifetime -= gameTime;

    // Destrói ao sair da tela ou expirar
    if (lifetime <= 0.0f ||
        x < -20.0f || x > window->Width() + 20.0f ||
        y < -20.0f || y > window->Height() + 20.0f)
    {
        gScene->Delete(this, MOVING);
    }
}

// ---------------------------------------------------------------------------------

void BossProjectile::Draw()
{
    Engine::renderer->BeginPixels();
    Circle c(10.0f);
    c.MoveTo(x, y);
    Engine::renderer->Draw(&c, 0xFFFF4400);   // laranja
    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void BossProjectile::OnCollision(Object* obj)
{
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;
        pawn->TakeDamage(damage);
        gScene->Delete(this, MOVING);
    }

    if (obj->Type() == OBJ_PLATFORM)
    {
        gScene->Delete(this, MOVING);
    }
}
