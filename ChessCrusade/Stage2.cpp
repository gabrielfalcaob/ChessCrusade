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
    // TODO: adicionar sons

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
    scene->Add(new Platform(W * 0.5f, H - 50.0f, W, 100.0f), STATIC);

    // Plataformas — layout diferente da fase 1
    scene->Add(new Platform(250.0f,  H - 240.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(800.0f,  H - 380.0f, 280.0f, 30.0f), STATIC);
    scene->Add(new Platform(1350.0f, H - 260.0f, 240.0f, 30.0f), STATIC);

    // Peão
    pawn = new Pawn(180.0f, H - 150.0f);
    scene->Add(pawn, MOVING);

    // Lacaios: mini-cavaleiros (mais rápidos)
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 400.0f, H-150.0f, 200.0f, 700.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 900.0f, H-150.0f, 650.0f, 1200.0f, 3, 1), MOVING);
    scene->Add(new Enemy(ENEMY_PAWN_KNIGHT, 250.0f, H-330.0f, 120.0f, 380.0f, 2, 1), MOVING);

    // Boss
    boss = new KnightBoss(W - 180.0f, H - 170.0f, pawn);
    scene->Add(boss, MOVING);
}

void Stage2::CheckWinCondition()
{
    if (!stageClear && boss && boss->IsDead())
    {
        stageClear = true;
        // Desbloqueia poder adicional (ex: cooldown do especial reduz)
        PAWN_SHOOT_COOLDOWN;    // placeholder — pode ajustar aqui
        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

void Stage2::Update()
{
    if (window->KeyPress(VK_ESCAPE)) Engine::Next<Home>();
    if (stageClear) { stageClearTimer -= gameTime; if (stageClearTimer <= 0.0f) Engine::Next<Stage3>(); return; }
    scene->Update();
    scene->CollisionDetection();
    if (pawn && pawn->isDead) { Engine::Next<GameOver>(); return; }
    CheckWinCondition();
}

void Stage2::Draw()
{
    scene->Draw();
    if (hud && pawn) hud->Draw(pawn, "Est. 2-1: O Cavaleiro");
}

void Stage2::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud; delete backg; delete audio; delete scene;
    gScene = nullptr; gAudio = nullptr;
}
