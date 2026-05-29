/**********************************************************************************
// Stages345.cpp - Fases 3, 4 e 5
**********************************************************************************/

#include "Stages345.h"
#include "Home.h"
#include "GameOver.h"
#include "Engine.h"
#include "Resources.h"
#include "PeaoRebelde.h"

// Definição dos bosses das fases 3-5 (incluídos via Bosses345.h)
// BishopBoss, QueenBoss e KingBoss são declarados em Bosses345.h

// =============================================================================
// STAGE 3 - O BISPO
// =============================================================================

void Stage3::Init()
{
    audio = new Audio();
    gAudio = audio;

    scene = new Scene();
    gScene = scene;
    hud = new HUD();
    stageClear = false;
    stageClearTimer = 2.5f;
    gCurrentStage = 3;
    BuildLevel();
}

void Stage3::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;

    // Chão
    scene->Add(new Platform(W * 0.5f, 850.0f, W, 100.0f), STATIC);

    // Plataformas — layout em escada, bispo usa alturas
    scene->Add(new Platform(200.0f,  640.0f, 260.0f, 30.0f), STATIC);
    scene->Add(new Platform(600.0f,  500.0f, 260.0f, 30.0f), STATIC);
    scene->Add(new Platform(1000.0f, 620.0f, 260.0f, 30.0f), STATIC);
    scene->Add(new Platform(1400.0f, 480.0f, 260.0f, 30.0f), STATIC);

    pawn = new Pawn(180.0f, 700.0f);
    scene->Add(pawn, MOVING);

    // Lacaios: mini-bispos (lentos mas HP maior)
    scene->Add(new Enemy(ENEMY_PAWN_BISHOP, 450.0f, 700.0f, 200.0f, 700.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_BISHOP, 950.0f, 700.0f, 700.0f,1250.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_BISHOP, 600.0f, 415.0f, 480.0f, 720.0f, 2, 1), MOVING);

    // Boss Bispo — posicionado numa plataforma alta
    BishopBoss* bishopBoss = new BishopBoss(1400.0f, 350.0f, pawn);
    boss = bishopBoss;
    scene->Add(boss, MOVING);
}

void Stage3::CheckWinCondition()
{
    if (!stageClear && boss && boss->IsDead())
    {
        stageClear = true;
        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

void Stage3::Update()
{
    if (window->KeyPress(VK_ESCAPE)) Engine::Next<Home>();
    if (window->KeyPress('B')) gViewBBox = !gViewBBox;

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f) Engine::Next<Stage4>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead) { Engine::Next<GameOver>(); return; }
    CheckWinCondition();
}

void Stage3::Draw()
{
    scene->Draw();
    if (gViewBBox) scene->DrawBBox();
    if (hud && pawn) hud->Draw(pawn, "Est. 3-1: O Bispo");
}

void Stage3::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    delete backg;
    delete audio;
    delete scene;
    gScene = nullptr;
    gAudio = nullptr;
}

// =============================================================================
// STAGE 4 - A RAINHA
// =============================================================================

void Stage4::Init()
{
    audio = new Audio();
    gAudio = audio;

    scene = new Scene();
    gScene = scene;
    hud = new HUD();
    stageClear = false;
    stageClearTimer = 2.5f;
    gCurrentStage = 4;
    BuildLevel();
}

void Stage4::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;

    // Chão
    scene->Add(new Platform(W * 0.5f, 850.0f, W, 100.0f), STATIC);

    // Plataformas em formato de arena central
    scene->Add(new Platform(400.0f,  580.0f, 300.0f, 30.0f), STATIC);
    scene->Add(new Platform(800.0f,  440.0f, 300.0f, 30.0f), STATIC);
    scene->Add(new Platform(1200.0f, 580.0f, 300.0f, 30.0f), STATIC);

    pawn = new Pawn(180.0f, 700.0f);
    scene->Add(pawn, MOVING);

    // Lacaios: mini-torres (lentos e robustos)
    scene->Add(new Enemy(ENEMY_PAWN_ROOK, 500.0f, 700.0f, 250.0f,  850.0f, 4, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_ROOK, 1100.0f,700.0f, 900.0f, 1400.0f, 4, 1), MOVING);

    // Boss Rainha — centro da arena
    QueenBoss* queenBoss = new QueenBoss(W * 0.5f, 700.0f, pawn);
    boss = queenBoss;
    scene->Add(boss, MOVING);
}

void Stage4::CheckWinCondition()
{
    if (!stageClear && boss && boss->IsDead())
    {
        stageClear = true;
        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

void Stage4::Update()
{
    if (window->KeyPress(VK_ESCAPE)) Engine::Next<Home>();
    if (window->KeyPress('B')) gViewBBox = !gViewBBox;

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f) Engine::Next<Stage5>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead) { Engine::Next<GameOver>(); return; }
    CheckWinCondition();
}

void Stage4::Draw()
{
    scene->Draw();
    if (gViewBBox) scene->DrawBBox();
    if (hud && pawn) hud->Draw(pawn, "Est. 4-1: A Rainha");
}

void Stage4::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    delete backg;
    delete audio;
    delete scene;
    gScene = nullptr;
    gAudio = nullptr;
}

// =============================================================================
// STAGE 5 - O REI
// =============================================================================

void Stage5::Init()
{
    audio = new Audio();
    gAudio = audio;

    scene = new Scene();
    gScene = scene;
    hud = new HUD();
    stageClear = false;
    stageClearTimer = 2.5f;
    gCurrentStage = 5;
    BuildLevel();
}

void Stage5::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;

    // Chão
    scene->Add(new Platform(W * 0.5f, 850.0f, W, 100.0f), STATIC);

    // Plataformas simétricas — arena do rei
    scene->Add(new Platform(300.0f,  600.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(800.0f,  460.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(1300.0f, 600.0f, 280.0f, 30.0f), STATIC);
    // Trono elevado ao centro
    scene->Add(new Platform(W * 0.5f, 320.0f, 240.0f, 30.0f), STATIC);

    pawn = new Pawn(180.0f, 700.0f);
    scene->Add(pawn, MOVING);

    // Lacaios: mini-reis (variados — combinação dos anteriores)
    scene->Add(new Enemy(ENEMY_PAWN_KING, 500.0f,  700.0f, 250.0f,  850.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KING, 1000.0f, 700.0f, 800.0f, 1350.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KING, 300.0f,  515.0f, 170.0f,  440.0f, 2, 1), MOVING);

    // Boss Rei — no trono elevado
    KingBoss* kingBoss = new KingBoss(W * 0.5f, 200.0f, pawn);
    boss = kingBoss;
    scene->Add(boss, MOVING);
}

void Stage5::CheckWinCondition()
{
    if (!stageClear && boss && boss->IsDead())
    {
        stageClear = true;
        gCurrentStage = 6;     // sinaliza vitória para GameOver
        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

void Stage5::Update()
{
    if (window->KeyPress(VK_ESCAPE)) Engine::Next<Home>();
    if (window->KeyPress('B')) gViewBBox = !gViewBBox;

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f) Engine::Next<GameOver>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead) { Engine::Next<GameOver>(); return; }
    CheckWinCondition();
}

void Stage5::Draw()
{
    scene->Draw();
    if (gViewBBox) scene->DrawBBox();
    if (hud && pawn) hud->Draw(pawn, "Est. 5-1: O Rei");
}

void Stage5::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    delete backg;
    delete audio;
    delete scene;
    gScene = nullptr;
    gAudio = nullptr;
}
