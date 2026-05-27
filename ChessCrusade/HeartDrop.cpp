/**********************************************************************************
// HeartDrop.cpp
**********************************************************************************/

#include "HeartDrop.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

HeartDrop::HeartDrop(float posX, float posY)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type      = OBJ_HEART_DROP;
    velY      = -200.0f;    // sobe um pouco ao dropar
    lifetime  = 8.0f;       // some em 8 segundos
    collected = false;

    BBox(new Circle(12.0f));
}

// ---------------------------------------------------------------------------------

void HeartDrop::Update()
{
    // Física simples: queda com gravidade leve
    velY += 400.0f * gameTime;
    Translate(0.0f, velY * gameTime);

    // Para no "chão" aproximado (simplificado)
    if (y + 12.0f >= window->Height() - 100.0f)
    {
        MoveTo(x, window->Height() - 100.0f - 12.0f);
        velY = 0.0f;
    }

    lifetime -= gameTime;
    if (lifetime <= 0.0f)
        gScene->Delete(this, MOVING);
}

// ---------------------------------------------------------------------------------

void HeartDrop::Draw()
{
    // Pisca quando está para sumir
    if (lifetime < 2.0f)
    {
        int frame = (int)(lifetime * 6.0f);
        if (frame % 2 == 0) return;
    }

    Engine::renderer->BeginPixels();
    Circle c(12.0f);
    c.MoveTo(x, y);
    Engine::renderer->Draw(&c, 0xFFFF2266);   // rosa/vermelho
    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void HeartDrop::OnCollision(Object* obj)
{
    // Coleta tratada no Pawn::OnCollision
}
