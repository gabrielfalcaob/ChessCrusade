/**********************************************************************************
// GameOver.h - Tela de Game Over / Vitória
**********************************************************************************/
#ifndef _PEAOREBELDE_GAMEOVER_H_
#define _PEAOREBELDE_GAMEOVER_H_

#include "Game.h"
#include "Sprite.h"
#include "Font.h"
#include "PeaoRebelde.h"

class GameOver : public Game
{
private:
    Sprite* backg = nullptr;
    Font*   font  = nullptr;
    bool    victory;        // true = ganhou, false = morreu

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

#endif
