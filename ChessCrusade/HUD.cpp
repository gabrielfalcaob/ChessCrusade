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
    maxHearts = PAWN_MAX_HEARTS;
    heartFull = nullptr;
    heartEmpty = nullptr;

    // Fonte já disponível nos Resources do projeto
    font = new Font("Resources/Tahoma14.png");
    font->Spacing("Resources/Tahoma14.dat");
}

// ---------------------------------------------------------------------------------
HUD::~HUD()
{
    delete font;
    delete heartFull;
    delete heartEmpty;
}

// ---------------------------------------------------------------------------------
// DrawOnly — apenas retângulos, chamar DENTRO de BeginPixels/EndPixels
void HUD::DrawOnly(Pawn* pawn, Boss* boss)
{
    if (!pawn) return;

    // ---- Corações ----
    const float heartSize = 20.0f;
    const float gap = 4.0f;
    const float hx = 20.0f;
    const float hy = 16.0f;

    for (int i = 0; i < maxHearts; ++i)
    {
        float cx = hx + i * (heartSize + gap) + heartSize * 0.5f;
        float cy = hy + heartSize * 0.5f;

        if (heartFull && i < pawn->hearts)
            heartFull->Draw(cx, cy, Layer::FRONT);
        else if (heartEmpty && i >= pawn->hearts)
            heartEmpty->Draw(cx, cy, Layer::FRONT);
        else
        {
            Rect r(cx - heartSize * 0.5f, cy - heartSize * 0.5f,
                cx + heartSize * 0.5f, cy + heartSize * 0.5f);
            Engine::renderer->Draw(&r, i < pawn->hearts ? 0xFFDD2244 : 0xFF333333);
        }
    }

    // ---- Cooldown do ataque especial ----
    if (gHasRangedAttack)
    {
        const float barX = 20.0f;
        const float barY = hy + heartSize + 8.0f;
        const float barW = 160.0f;
        const float barH = 12.0f;
        float fill = 1.0f - pawn->ShootCooldownRatio();

        Rect bg(barX, barY, barX + barW, barY + barH);
        Engine::renderer->Draw(&bg, 0xFF222222);

        if (fill > 0.0f)
        {
            Rect f(barX, barY, barX + barW * fill, barY + barH);
            Engine::renderer->Draw(&f, fill >= 1.0f ? 0xFF44AAFF : 0xFF1C5C8A);
        }

        // Borda
        Engine::renderer->Draw(&bg, 0xFF666688);
    }

    // ---- Barra de HP do boss ----
    if (boss && boss->IsAlive())
    {
        const float barW = 500.0f;
        const float barH = 22.0f;
        // CORREÇÃO: Alterado de window-> para Engine::window->
        const float barX = Engine::window->CenterX() - barW * 0.5f;
        const float barY = 14.0f;
        float ratio = (float)boss->HP() / (float)boss->MaxHP();

        // Fundo
        Rect bg(barX, barY, barX + barW, barY + barH);
        Engine::renderer->Draw(&bg, 0xFF1A1A1A);

        // Preenchimento — verde acima de 50%, vermelho abaixo
        if (ratio > 0.0f)
        {
            Rect fill(barX, barY, barX + barW * ratio, barY + barH);
            DWORD color = ratio > 0.5f ? 0xFF22AA22 : 0xFFAA2222;
            Engine::renderer->Draw(&fill, color);
        }

        // Borda
        Engine::renderer->Draw(&bg, 0xFFAAAAAA);
    }
}

// ---------------------------------------------------------------------------------
// DrawText — apenas texto via Font, chamar FORA de BeginPixels/EndPixels
void HUD::DrawText(const string& stageName, Boss* boss)
{
    if (!font) return;

    const float hy = 16.0f;

    // ---- Nome da fase (canto superior esquerdo, abaixo dos corações) ----
    float stageY = hy + 20.0f + 8.0f + 14.0f;
    font->Draw(20.0f, stageY, stageName,
        { 1.0f, 1.0f, 0.8f, 1.0f },         // branco-amarelado
        Layer::FRONT);

    // ---- Pontuação (canto superior direito) ----
    string scoreStr = "Score: " + to_string(gScore);
    // CORREÇÃO: Alterado de window-> para Engine::window->
    font->Draw(Engine::window->Width() - 220.0f, hy, scoreStr,
        { 1.0f, 1.0f, 1.0f, 1.0f },
        Layer::FRONT);

    // ---- HP numérico do boss (centralizado, sobreposto à barra) ----
    if (boss && boss->IsAlive())
    {
        string hpStr = to_string(boss->HP()) + " / " + to_string(boss->MaxHP());

        // CORREÇÃO: Centralização horizontal com Engine::window->CenterX()
        // Ajustado o Y para 16.0f para o texto ficar perfeitamente alinhado dentro ou colado à barra
        float textX = Engine::window->CenterX() - (float)(hpStr.size() * 7) * 0.5f;
        font->Draw(textX, 16.0f, hpStr,
            { 1.0f, 1.0f, 1.0f, 1.0f },
            Layer::FRONT);

        // Nome do boss (à esquerda da barra)
        size_t sep = stageName.find(": ");
        if (sep != string::npos)
        {
            string bossName = stageName.substr(sep + 2);
            // CORREÇÃO: Alterado para Engine::window->CenterX()
            font->Draw(Engine::window->CenterX() - 270.0f - (float)(bossName.size() * 7),
                16.0f, bossName,
                { 1.0f, 0.6f, 0.6f, 1.0f },   // rosado
                Layer::FRONT);
        }
    }
}

// ---------------------------------------------------------------------------------
// Draw — wrapper completo (compatibilidade)
void HUD::Draw(Pawn* pawn, const string& stageName, Boss* boss)
{
    Engine::renderer->BeginPixels();
    DrawOnly(pawn, boss);
    Engine::renderer->EndPixels();
    DrawText(stageName, boss);
}