/**********************************************************************************
// Stage1.cpp
**********************************************************************************/

#include "Engine.h"
#include "Home.h"
#include "Stage1.h"
#include "Stage2.h"
#include "GameOver.h"
#include "PeaoRebelde.h"
#include "Resources.h"

// ---------------------------------------------------------------------------------

void Stage1::Init()
{
    // Sistema de áudio
    audio = new Audio();
    gAudio = audio;
    audio->Add(SND_SWORD_SWING,  "Resources/sword_swing.wav");
    audio->Add(SND_SWORD_HIT,    "Resources/sword_hit.wav");
    audio->Add(SND_PLAYER_HURT,  "Resources/player_hurt.wav");
    audio->Add(SND_PLAYER_JUMP,  "Resources/jump.wav");
    audio->Add(SND_ENEMY_DEATH,  "Resources/enemy_death.wav");
    audio->Add(SND_BOSS_HURT,    "Resources/boss_hurt.wav");
    audio->Add(SND_BOSS_DEATH,   "Resources/boss_death.wav");
    audio->Add(SND_HEART_PICKUP, "Resources/heart_pickup.wav");
    audio->Add(SND_STAGE_CLEAR,  "Resources/stage_clear.wav");

    // Cena
    scene = new Scene();
    gScene = scene;

    // HUD
    hud = new HUD();

    // Fundo (placeholder: cor de fundo definida no WinMain)
    // backg = new Sprite("Resources/bg_stage1.png");

    // Estado
    bossDefeated    = false;
    stageClear      = false;
    stageClearTimer = 2.5f;
    enemiesRemaining = 0;

    // Constrói o nível
    BuildLevel();

    gCurrentStage = 1;
}

// ---------------------------------------------------------------------------------

void Stage1::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;
    float H = (float)WINDOW_HEIGHT;

    // ---- Chão principal ----
    // Plataforma sólida cobrindo toda a largura, no fundo
    Platform* ground = new Platform(W * 0.5f, H - 50.0f, W, 100.0f);
    scene->Add(ground, STATIC);

    // ---- Plataformas intermediárias ----
    // Plataforma esquerda (média altura)
    Platform* platLeft = new Platform(300.0f, H - 260.0f, 300.0f, 30.0f);
    scene->Add(platLeft, STATIC);

    // Plataforma direita (alta)
    Platform* platRight = new Platform(1100.0f, H - 400.0f, 260.0f, 30.0f);
    scene->Add(platRight, STATIC);

    // Plataforma central (mais baixa)
    Platform* platCenter = new Platform(700.0f, H - 310.0f, 220.0f, 30.0f);
    scene->Add(platCenter, STATIC);

    // ---- Peão jogador ----
    gPawnHearts = PAWN_START_HEARTS;   // reinicia na primeira fase
    pawn = new Pawn(180.0f, H - 150.0f);
    scene->Add(pawn, MOVING);

    // ---- Lacaios: 2 peões inimigos ----
    // Inimigo 1: patrulha na plataforma esquerda
    Enemy* e1 = new Enemy(ENEMY_PAWN, 250.0f, H - 350.0f,
                          120.0f, 450.0f,   // patrulha entre x=120 e x=450
                          2, 1);
    scene->Add(e1, MOVING);
    enemiesRemaining++;

    // Inimigo 2: patrulha no chão, centro
    Enemy* e2 = new Enemy(ENEMY_PAWN, 700.0f, H - 150.0f,
                          500.0f, 950.0f,
                          2, 1);
    scene->Add(e2, MOVING);
    enemiesRemaining++;

    // ---- Boss: Torre ----
    // Fica no lado direito, já presente desde o início
    boss = new TowerBoss(W - 150.0f, H - 160.0f);
    scene->Add(boss, MOVING);
}

// ---------------------------------------------------------------------------------

void Stage1::CheckWinCondition()
{
    if (bossDefeated) return;

    if (boss && boss->IsDead())
    {
        bossDefeated = true;
        stageClear   = true;

        // Desbloqueia ataque à distância ao derrotar o primeiro boss!
        gHasRangedAttack = true;

        if (gAudio) gAudio->Play(SND_STAGE_CLEAR);
    }
}

// ---------------------------------------------------------------------------------

void Stage1::Update()
{
    // Tecla ESC volta ao menu (ou pausa — simplificado aqui como saída)
    if (window->KeyPress(VK_ESCAPE))
        Engine::Next<Home>();

    // Tecla B: toggle debug BBox
    if (window->KeyPress('B'))
    {
        gViewBBox = !gViewBBox; // Inverte o estado (liga/desliga)
    }

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f)
            Engine::Next<Stage2>();
        return;
    }

    scene->Update();
    scene->CollisionDetection();

    // Peão morreu?
    if (pawn && pawn->isDead)
    {
        Engine::Next<GameOver>();
        return;
    }

    // Sincroniza contador de inimigos (conta objetos OBJ_ENEMY vivos)
    // Simplificado: CheckWinCondition só verifica o boss
    CheckWinCondition();
}

// ---------------------------------------------------------------------------------

void Stage1::Draw()
{
    // Fundo
    if (backg)
        backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);

    // Objetos da cena
    scene->Draw();

    // HUD
    if (hud && pawn)
        hud->Draw(pawn, "Est. 1-1: A Torre");

    // Mensagem de fase concluída
    if (stageClear)
    {
        // TODO: desenhar texto com fonte
        // font->Draw(center, center, "FASE CONCLUÍDA!", ...);
    }
}

// ---------------------------------------------------------------------------------

void Stage1::Finalize()
{
    // Persiste corações antes de trocar de fase
    if (pawn) gPawnHearts = pawn->hearts;

    delete hud;
    delete backg;
    delete audio;
    delete scene;

    gScene = nullptr;
    gAudio = nullptr;
}
