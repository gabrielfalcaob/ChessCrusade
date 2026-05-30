/**********************************************************************************
// PawnProjectile.cpp
**********************************************************************************/

#include "PawnProjectile.h"
#include "Enemy.h"
#include "Boss.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

PawnProjectile::PawnProjectile(float posX, float posY, float dirX, float speed)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type     = OBJ_PROJECTILE;
    velX     = dirX * speed;
    lifetime = 2.5f;

    BBox(new Circle(8.0f));
}

// ---------------------------------------------------------------------------------

void PawnProjectile::Update()
{
    Translate(velX * gameTime, 0.0f);
    lifetime -= gameTime;

    if (lifetime <= 0.0f ||
        x < -20.0f || x > window->Width() + 20.0f)
    {
        gScene->Delete(this, MOVING);
    }
}

// ---------------------------------------------------------------------------------

void PawnProjectile::Draw()
{
    Circle c(8.0f);
    c.MoveTo(x, y);
    Engine::renderer->Draw(&c, 0xFFFFDD00);   // amarelo dourado
}

// ---------------------------------------------------------------------------------

void PawnProjectile::OnCollision(Object* obj)
{
    if (obj->Type() == OBJ_ENEMY)
    {
        Enemy* e = (Enemy*)obj;
        e->TakeDamage(1);
        gScore += 50;
        gScene->Delete(this, MOVING);
    }

    if (obj->Type() == OBJ_BOSS)
    {
        Boss* b = (Boss*)obj;
        b->TakeDamage(1);
        gScore += 100;
        gScene->Delete(this, MOVING);
    }

    if (obj->Type() == OBJ_PLATFORM)
    {
        gScene->Delete(this, MOVING);
    }
}
