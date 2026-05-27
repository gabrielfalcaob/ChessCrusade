/**********************************************************************************
// Boss.cpp
**********************************************************************************/

#include "Boss.h"
#include "Pawn.h"
#include "Engine.h"
#include <algorithm>
using namespace std;

// ---------------------------------------------------------------------------------

Boss::Boss(float posX, float posY, int maxHp_, float w, float h)
{
    MoveTo(posX, posY, Layer::MIDDLE);
    type       = OBJ_BOSS;
    hp         = maxHp_;
    maxHp      = maxHp_;
    bossPhase  = BOSS_PHASE1;
    isDead     = false;
    iFrameTimer= 0.0f;
    attackTimer= 2.0f;      // espera 2s antes do primeiro ataque
    halfW      = w * 0.5f;
    halfH      = h * 0.5f;
    facingRight= false;     // boss geralmente olha para a esquerda (jogador à esquerda)
    velX       = 0.0f;
    velY       = 0.0f;
    onGround   = false;
    tileSet    = nullptr;
    anim       = nullptr;
    font       = nullptr;

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Boss::~Boss()
{
    delete anim;
    delete tileSet;
    delete font;
}

// ---------------------------------------------------------------------------------

void Boss::TakeDamage(int amount)
{
    if (iFrameTimer > 0.0f || isDead) return;

    hp -= amount;
    iFrameTimer = 0.5f;

    if (gAudio) gAudio->Play(SND_BOSS_HURT);

    // Muda para fase 2 na metade do HP
    if (hp <= maxHp / 2 && bossPhase == BOSS_PHASE1)
        bossPhase = BOSS_PHASE2;

    if (hp <= 0)
    {
        hp     = 0;
        isDead = true;
        if (gAudio) gAudio->Play(SND_BOSS_DEATH);
        gScore += 1000;
    }
}

// ---------------------------------------------------------------------------------

void Boss::DrawHealthBar()
{
    // Barra de HP do boss no topo da tela (centralizada)
    float barW    = 600.0f;
    float barH    = 20.0f;
    float barX    = window->CenterX() - barW * 0.5f;
    float barY    = 16.0f;
    float fillRatio = (float)hp / (float)maxHp;

    Engine::renderer->BeginPixels();

    // Fundo cinza
    Rect bg(barX, barY, barX + barW, barY + barH);
    Engine::renderer->Draw(&bg, 0xFF444444);

    // Preenchimento vermelho
    if (fillRatio > 0.0f)
    {
        Rect fill(barX, barY, barX + barW * fillRatio, barY + barH);
        DWORD color = fillRatio > 0.5f ? 0xFF22CC22 : 0xFFCC2222; // verde/vermelho
        Engine::renderer->Draw(&fill, color);
    }

    // Borda branca
    Engine::renderer->Draw(&bg, 0xFFFFFFFF);   // apenas borda (sem preenchimento)

    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void Boss::Update()
{
    if (iFrameTimer > 0.0f) iFrameTimer -= gameTime;
    if (attackTimer  > 0.0f) attackTimer  -= gameTime;

    if (!isDead)
    {
        // Gravidade básica
        if (!onGround)
            velY += PAWN_GRAVITY * 0.8f * gameTime;

        Translate(velX * gameTime, velY * gameTime);

        // Fallback chão
        if (y + halfH >= window->Height() - 10.0f)
        {
            MoveTo(x, window->Height() - 10.0f - halfH);
            velY    = 0.0f;
            onGround = true;
        }
        onGround = false;   // Platform confirma

        BehaviorUpdate();
    }
}

// ---------------------------------------------------------------------------------

void Boss::Draw()
{
    if (isDead) return;

    bool visible = true;
    if (iFrameTimer > 0.0f)
    {
        int frame = (int)(iFrameTimer * 10.0f);
        visible = (frame % 2 == 0);
    }

    if (visible)
    {
        if (anim)
        {
            anim->Draw(x, y, z);
        }
        else
        {
            // Placeholder: quadrado grande cinza claro
            Engine::renderer->BeginPixels();
            Rect body(-halfW, -halfH, halfW, halfH);
            body.MoveTo(x, y);
            Engine::renderer->Draw(&body,
                bossPhase == BOSS_PHASE1 ? 0xFFAAAAAA : 0xFFFF6644);
            Engine::renderer->EndPixels();
        }
    }

    DrawHealthBar();
}

// ---------------------------------------------------------------------------------

void Boss::OnCollision(Object* obj)
{
    // Causa dano ao peão ao encostar (contato direto)
    if (obj->Type() == OBJ_PAWN)
    {
        Pawn* pawn = (Pawn*)obj;
        pawn->TakeDamage(1);
    }
}
