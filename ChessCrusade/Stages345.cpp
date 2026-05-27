#include "Stages345.h"
#include "Home.h"
#include "GameOver.h"
#include "Engine.h"

// =============================================================================
// IMPLEMENTAÇÃO STAGE 3
// =============================================================================
void Stage3::Init()
{
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
    pawn = new Pawn(180.0f, (float)WINDOW_HEIGHT - 150.0f);
    scene->Add(pawn, MOVING);
}

void Stage3::Update()
{
    if (window->KeyPress(VK_ESCAPE))
        Engine::Next<Home>();

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f)
            Engine::Next<Stage4>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead)
        Engine::Next<GameOver>();

    CheckWinCondition();
}

void Stage3::Draw()
{
    if (hud && pawn)
        hud->Draw(pawn, "Est. 3-1: O Bispo");
    scene->Draw();
}

void Stage3::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    delete scene;
}

void Stage3::CheckWinCondition()
{
    if (boss && boss->IsDead())
        stageClear = true;
}

// =============================================================================
// IMPLEMENTAÇÃO STAGE 4
// =============================================================================
void Stage4::Init()
{
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
    pawn = new Pawn(180.0f, (float)WINDOW_HEIGHT - 150.0f);
    scene->Add(pawn, MOVING);
}

void Stage4::Update()
{
    if (window->KeyPress(VK_ESCAPE))
        Engine::Next<Home>();

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f)
            Engine::Next<Stage5>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead)
        Engine::Next<GameOver>();

    CheckWinCondition();
}

void Stage4::Draw()
{
    if (hud && pawn)
        hud->Draw(pawn, "Est. 4-1: A Rainha");
    scene->Draw();
}

void Stage4::Finalize()
{
    if (pawn) gPawnHearts = pawn->hearts;
    delete hud;
    delete scene;
}

void Stage4::CheckWinCondition()
{
    if (boss && boss->IsDead())
        stageClear = true;
}

// =============================================================================
// IMPLEMENTAÇÃO STAGE 5
// =============================================================================
void Stage5::Init()
{
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
    pawn = new Pawn(180.0f, (float)WINDOW_HEIGHT - 150.0f);
    scene->Add(pawn, MOVING);
}

void Stage5::Update()
{
    if (window->KeyPress(VK_ESCAPE))
        Engine::Next<Home>();

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f)
            Engine::Next<GameOver>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead)
        Engine::Next<GameOver>();

    CheckWinCondition();
}

void Stage5::Draw()
{
    if (hud && pawn)
        hud->Draw(pawn, "Est. 5-1: O Rei");
    scene->Draw();
}

void Stage5::Finalize()
{
    delete hud;
    delete scene;
}

void Stage5::CheckWinCondition()
{
    if (boss && boss->IsDead())
        stageClear = true;
}