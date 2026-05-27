/**********************************************************************************
// Stage1.h - Fase 1: A Torre
//
// Layout:
//   - Chão principal (plataforma base)
//   - 2 plataformas intermediárias
//   - 2 peões inimigos patrulhando
//   - Boss: TowerBoss no lado direito
//
// Fluxo:
//   - Player começa à esquerda
//   - Derrota os 2 lacaios (peões inimigos)
//   - Depois o boss fica ativo (já está lá, mas pode-se controlar pelo flag)
//   - Boss morto → gHasRangedAttack = true, vai para Stage2
**********************************************************************************/

#ifndef _PEAOREBELDE_STAGE1_H_
#define _PEAOREBELDE_STAGE1_H_

#include "Game.h"
#include "Scene.h"
#include "Audio.h"
#include "Pawn.h"
#include "TowerBoss.h"
#include "Enemy.h"
#include "Platform.h"
#include "HUD.h"
#include <string>
#include <vector>

// ---------------------------------------------------------------------------------

class Stage1 : public Game
{
protected:
    Scene*      scene    = nullptr;
    Audio*      audio    = nullptr;
    Pawn*       pawn     = nullptr;
    TowerBoss*  boss     = nullptr;
    HUD*        hud      = nullptr;
    Sprite*     backg    = nullptr;

    // Rastreia inimigos vivos para saber quando liberar o boss
    int enemiesRemaining;
    bool bossDefeated;
    bool stageClear;
    float stageClearTimer;      // aguarda antes de trocar de fase

    void BuildLevel();          // cria plataformas e inimigos
    void CheckWinCondition();

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

// ---------------------------------------------------------------------------------
#endif
