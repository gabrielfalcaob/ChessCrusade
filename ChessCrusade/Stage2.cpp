/**********************************************************************************
// Stage2.cpp - Fase 2: O Cavaleiro
**********************************************************************************/

#include "Engine.h"
#include "Stage2.h"
#include "Stages345.h"
#include "GameOver.h"
#include "Home.h"
#include "PeaoRebelde.h"
#include "Resources.h"

void Stage2::Init()
{
    audio = new Audio();
    gAudio = audio;


    scene = new Scene();
    gScene = scene;
    hud = new HUD();
    stageClear = false;
    stageClearTimer = 2.5f;
    gCurrentStage = 2;
    BuildLevel();
}

void Stage2::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;
    float H = (float)WINDOW_HEIGHT;

    // Chão
    scene->Add(new Platform(W * 0.5f, 850.0f, W, 100.0f), STATIC);

    // Plataformas — layout diferente da fase 1
    scene->Add(new Platform(250.0f,  580.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(850.0f,  470.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(1350.0f, 610.0f, 240.0f, 30.0f), STATIC);

    // Peão (usa corações persistentes da fase anterior)
    pawn = new Pawn(180.0f, 700.0f);
    scene->Add(pawn, MOVING);

    // Lacaios: mini-cavaleiros
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 500.0f,  700.0f, 250.0f,  780.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 950.0f,  700.0f, 700.0f, 1200.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 250.0f,  490.0f, 120.0f,  380.0f, 2, 1), MOVING);

    // Boss Cavaleiro
    boss = new KnightBoss(W - 200.0f, 700.0f, pawn);
    scene->Add(boss, MOVING);
}

void Stage2::CheckWinCondition()
{
    if (!stageClear && boss && boss->IsDead())
    {
        stageClear = true;
        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

void Stage2::Update()
{
    if (window->KeyPress(VK_ESCAPE)) Engine::Next<Home>();
    if (window->KeyPress('B')) gViewBBox = !gViewBBox;

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f) Engine::Next<Stage3>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead) { Engine::Next<GameOver>(); return; }
    CheckWinCondition();
}

void Stage2::Draw()
{
    scene->Draw();
    if (gViewBBox) scene->DrawBBox();
    if (hud && pawn) hud->Draw(pawn, "Est. 2-1: O Cavaleiro");
}

void Stage2::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    //delete backg;
    delete audio;
    delete scene;
    gScene = nullptr;
    gAudio = nullptr;
}
