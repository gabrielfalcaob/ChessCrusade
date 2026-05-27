/**********************************************************************************
// Stage2.h - Fase 2: O Cavaleiro
**********************************************************************************/
#ifndef _PEAOREBELDE_STAGE2_H_
#define _PEAOREBELDE_STAGE2_H_

#include "Game.h"
#include "Scene.h"
#include "Audio.h"
#include "Pawn.h"
#include "KnightBoss.h"
#include "Enemy.h"
#include "Platform.h"
#include "HUD.h"

class Stage2 : public Game
{
protected:
    Scene*      scene  = nullptr;
    Audio*      audio  = nullptr;
    Pawn*       pawn   = nullptr;
    KnightBoss* boss   = nullptr;
    HUD*        hud    = nullptr;
    Sprite*     backg  = nullptr;
    bool        stageClear = false;
    float       stageClearTimer = 2.5f;

    void BuildLevel();
    void CheckWinCondition();

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

#endif
