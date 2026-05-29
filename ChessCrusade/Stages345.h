/**********************************************************************************
// Stages345.h - Definição das Fases Finais (Bispo, Rainha e Rei)
//
// Descrição: Centraliza os cabeçalhos das fases 3, 4 e 5 em um único arquivo limpo.
**********************************************************************************/

#ifndef _PEAOREBELDE_STAGES345_H_
#define _PEAOREBELDE_STAGES345_H_

#include "Game.h"
#include "Scene.h"
#include "Audio.h"
#include "Pawn.h"
#include "Enemy.h"
#include "Platform.h"
#include "HUD.h"
#include "Bosses345.h" // Inclui BishopBoss, QueenBoss e KingBoss de uma vez só

// =============================================================================
// FASE 3: O BISPO
// =============================================================================
class Home;

class Stage3 : public Game {
protected:
    Scene* scene = nullptr;
    Audio* audio = nullptr;
    Pawn* pawn = nullptr;
    Boss* boss = nullptr;     // Alterado para Boss* genérico para aceitar no .cpp
    HUD* hud = nullptr;
    Sprite* backg = nullptr;

    bool    stageClear = false;
    float   stageClearTimer = 2.5f;

    void BuildLevel();
    void CheckWinCondition();

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

// =============================================================================
// FASE 4: A RAINHA
// =============================================================================
class Stage4 : public Game {
protected:
    Scene* scene = nullptr;
    Audio* audio = nullptr;
    Pawn* pawn = nullptr;
    Boss* boss = nullptr;     // Alterado para Boss* genérico
    HUD* hud = nullptr;
    Sprite* backg = nullptr;

    bool    stageClear = false;
    float   stageClearTimer = 2.5f;

    void BuildLevel();
    void CheckWinCondition();

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

// =============================================================================
// FASE 5: O REI (CHEFE FINAL)
// =============================================================================
class Stage5 : public Game {
protected:
    Scene* scene = nullptr;
    Audio* audio = nullptr;
    Pawn* pawn = nullptr;
    Boss* boss = nullptr;     // Alterado para Boss* genérico
    HUD* hud = nullptr;
    Sprite* backg = nullptr;

    bool    stageClear = false;
    float   stageClearTimer = 2.5f;

    void BuildLevel();
    void CheckWinCondition();

public:
    void Init()     override;
    void Update()   override;
    void Draw()     override;
    void Finalize() override;
};

#endif