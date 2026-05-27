/**********************************************************************************
// Home.h - Tela Inicial do Peão Rebelde
**********************************************************************************/
#ifndef _PEAOREBELDE_HOME_H_
#define _PEAOREBELDE_HOME_H_

#include "Game.h"
#include "Sprite.h"
#include "Font.h"
#include "PeaoRebelde.h"

class Home : public Game
{
private:
    Sprite* backg  = nullptr;
    Font*   font   = nullptr;
    float   blinkTimer = 0.0f;
    bool    showPrompt = true;

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

#endif
