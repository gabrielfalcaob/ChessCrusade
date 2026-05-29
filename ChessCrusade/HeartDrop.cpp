/**********************************************************************************
// HeartDrop.cpp
**********************************************************************************/

#include "HeartDrop.h"
#include "Platform.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

HeartDrop::HeartDrop(float posX, float posY)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type      = OBJ_HEART_DROP;
    velY      = -200.0f;
    lifetime  = 8.0f;
    collected = false;
    onGround  = false;

    BBox(new Circle(12.0f));
}

// ---------------------------------------------------------------------------------

void HeartDrop::Update()
{
    if (collected) return;

    // Gravidade
    if (!onGround)
        velY += 500.0f * gameTime;

    Translate(0.0f, velY * gameTime);

    // Fallback: limite inferior da tela
    float floor = window->Height() - 60.0f;
    if (y + 12.0f >= floor)
    {
        MoveTo(x, floor - 12.0f);
        velY     = 0.0f;
        onGround = true;
    }

    // A cada frame assume no ar, Platform confirma
    onGround = false;

    lifetime -= gameTime;
    if (lifetime <= 0.0f)
        gScene->Delete(this, MOVING);
}

// ---------------------------------------------------------------------------------

void HeartDrop::Draw()
{
    if (collected) return;

    // Pisca quando está para sumir
    if (lifetime < 2.0f)
    {
        int frame = (int)(lifetime * 8.0f);
        if (frame % 2 == 0) return;
    }

    Engine::renderer->BeginPixels();
    Circle c(12.0f);
    c.MoveTo(x, y);
    Engine::renderer->Draw(&c, 0xFFFF2266);
    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void HeartDrop::OnCollision(Object* obj)
{
    // Pousa sobre plataformas
    if (obj->Type() == OBJ_PLATFORM)
    {
        Platform* plat = (Platform*)obj;
        if (velY > 0.0f)
        {
            float prevBottom = (y + 12.0f) - velY * gameTime;
            if (prevBottom <= plat->Top() + DROP_PLATFORM_THRESHOLD)
            {
                MoveTo(x, plat->Top() - 12.0f);
                velY     = 0.0f;
                onGround = true;
            }
        }
    }
}
