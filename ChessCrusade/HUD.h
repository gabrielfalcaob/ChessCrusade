/**********************************************************************************
// HUD.h - Interface do Jogador
//
// Desenha:
//   - Corações do peão (placeholder ou sprite)
//   - Nome da fase atual
//   - Pontuação
//   - Cooldown do ataque especial (quando desbloqueado)
//   - Barra de HP do boss + valor numérico (quando boss != nullptr)
//
// IMPORTANTE: Font::Draw() usa sprites e não precisa de BeginPixels.
//             DrawOnly() desenha apenas os retângulos (corações, barras).
//             O Stage chama as duas dentro de um único BeginPixels/EndPixels.
**********************************************************************************/

#ifndef _PEAOREBELDE_HUD_H_
#define _PEAOREBELDE_HUD_H_

#include "Sprite.h"
#include "Font.h"
#include "Pawn.h"
#include "Boss.h"
#include "PeaoRebelde.h"
#include <string>
using std::string;

// ---------------------------------------------------------------------------------

class HUD
{
private:
    Font*   font;
    Sprite* heartFull;
    Sprite* heartEmpty;
    int     maxHearts;

public:
    HUD();
    ~HUD();

    // Retângulos (corações, barras) — chamar DENTRO de BeginPixels/EndPixels
    void DrawOnly(Pawn* pawn, Boss* boss = nullptr);

    // Texto (nome da fase, score, HP numérico) — chamar FORA de BeginPixels/EndPixels
    void DrawText(const string& stageName, Boss* boss = nullptr);

    // Wrapper completo (para quem não precisa mixar com outros pixels)
    void Draw(Pawn* pawn, const string& stageName, Boss* boss = nullptr);
};

// ---------------------------------------------------------------------------------
#endif
