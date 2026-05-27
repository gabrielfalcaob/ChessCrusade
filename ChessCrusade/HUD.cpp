/**********************************************************************************
// HUD.cpp
**********************************************************************************/

#include "HUD.h"
#include "Engine.h"
#include <string>
using namespace std;

// ---------------------------------------------------------------------------------

HUD::HUD()
{
    font       = nullptr;
    heartFull  = nullptr;
    heartEmpty = nullptr;
    maxHearts  = PAWN_MAX_HEARTS;

    // TODO: carregar fontes e sprites quando disponíveis
    // font      = new Font("Resources/font.png");
    // heartFull = new Sprite("Resources/heart_full.png");
    // heartEmpty= new Sprite("Resources/heart_empty.png");
}

// ---------------------------------------------------------------------------------

HUD::~HUD()
{
    delete font;
    delete heartFull;
    delete heartEmpty;
}

// ---------------------------------------------------------------------------------

void HUD::Draw(Pawn* pawn, const string& stageName)
{
    if (!pawn) return;

    Engine::renderer->BeginPixels();

    // ---- Corações ----
    float hx = 20.0f;
    float hy = 16.0f;
    float heartSize = 20.0f;
    float gap       = 4.0f;

    for (int i = 0; i < maxHearts; ++i)
    {
        float cx = hx + i * (heartSize + gap) + heartSize * 0.5f;
        float cy = hy + heartSize * 0.5f;

        if (heartFull && i < pawn->hearts)
        {
            heartFull->Draw(cx, cy, Layer::FRONT);
        }
        else if (heartEmpty && i >= pawn->hearts)
        {
            heartEmpty->Draw(cx, cy, Layer::FRONT);
        }
        else
        {
            // Placeholder: quadrado vermelho (cheio) ou cinza (vazio)
            Rect r(cx - heartSize*0.5f, cy - heartSize*0.5f,
                   cx + heartSize*0.5f, cy + heartSize*0.5f);
            Engine::renderer->Draw(&r,
                i < pawn->hearts ? 0xFFDD2244 : 0xFF444444);
        }
    }

    // ---- Cooldown do especial ----
    if (gHasRangedAttack)
    {
        float barX = 20.0f;
        float barY = hy + heartSize + 8.0f;
        float barW = 120.0f;
        float barH = 10.0f;
        float fill = 1.0f - pawn->ShootCooldownRatio(); // 0=sem carga, 1=pronto

        Rect bg(barX, barY, barX + barW, barY + barH);
        Engine::renderer->Draw(&bg, 0xFF333333);

        if (fill > 0.0f)
        {
            Rect f(barX, barY, barX + barW * fill, barY + barH);
            Engine::renderer->Draw(&f, fill >= 1.0f ? 0xFF44AAFF : 0xFF226688);
        }
        Engine::renderer->Draw(&bg, 0xFF888888);   // borda
    }

    // ---- Pontuação ----
    // TODO: usar font quando disponível
    // font->Draw(window->Width() - 200, 16, "Score: " + to_string(gScore), ...);

    Engine::renderer->EndPixels();
}
