/**********************************************************************************
// HUD.h - Interface do Jogador
//
// Desenha: corações do peão, pontuação, cooldown do especial, nome da fase.
// Não é um Object — é chamado diretamente pelo Stage no Draw().
**********************************************************************************/

#ifndef _PEAOREBELDE_HUD_H_
#define _PEAOREBELDE_HUD_H_

#include "Sprite.h"
#include "Font.h"
#include "Pawn.h"
#include "PeaoRebelde.h"
#include <string>
using std::string;

// ---------------------------------------------------------------------------------

class HUD
{
private:
    Font*   font;
    Sprite* heartFull;      // coração cheio
    Sprite* heartEmpty;     // coração vazio
    int     maxHearts;

public:
    HUD();
    ~HUD();

    void Draw(Pawn* pawn, const string& stageName);
};

// ---------------------------------------------------------------------------------
#endif
