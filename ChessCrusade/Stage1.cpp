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

    // Sistema de áudio
    audio = new Audio();
    gAudio = audio;
    audio->Add(SND_SWORD_SWING, "Resources/sword_swing.wav");
    audio->Add(SND_SWORD_HIT, "Resources/sword_hit.wav");
    audio->Add(SND_PLAYER_HURT, "Resources/player_hurt.wav");
    audio->Add(SND_PLAYER_JUMP, "Resources/jump.wav");
    audio->Add(SND_ENEMY_DEATH, "Resources/enemy_death.wav");
    audio->Add(SND_BOSS_HURT, "Resources/boss_hurt.wav");
    audio->Add(SND_BOSS_DEATH, "Resources/boss_death.wav");
    audio->Add(SND_HEART_PICKUP, "Resources/heart_pickup.wav");
    audio->Add(SND_STAGE_CLEAR, "Resources/stage_clear.wav");

    gCurrentStage = 1;
}

// ---------------------------------------------------------------------------------

void Stage1::BuildLevel()
{
    float W = (float)WINDOW_WIDTH;  // 1600
    float H = (float)WINDOW_HEIGHT; // 900

    // =========================================================================
    // 1. O CHÃO PRINCIPAL
    // =========================================================================
    // Centro em Y = 850, com 100 pixels de altura.
    // Isso significa que o TOPO do chão sólido vai ficar exatamente em Y = 800!
    Platform* ground = new Platform(W * 0.5f, 850.0f, W, 100.0f);
    scene->Add(ground, STATIC);

    // =========================================================================
    // 2. PLATAFORMAS FLUTUANTES INTERMEDIÁRIAS (Ajustadas para a nova altura)
    // =========================================================================
    // Plataforma esquerda (Média altura)
    Platform* platLeft = new Platform(300.0f, 600.0f, 300.0f, 30.0f);
    scene->Add(platLeft, STATIC);

    // Plataforma direita (Mais alta, perto do Boss)
    Platform* platRight = new Platform(1100.0f, 450.0f, 260.0f, 30.0f);
    scene->Add(platRight, STATIC);

    // Plataforma central 
    Platform* platCenter = new Platform(700.0f, 550.0f, 220.0f, 30.0f);
    scene->Add(platCenter, STATIC);

    // =========================================================================
    // 3. PERSONAGENS (Nascendo acima do chão de Y=800)
    // =========================================================================
    gPawnHearts = PAWN_START_HEARTS;

    // Peão: Nasce em X=180 e Y=700 (cai um pouquinho e pousa firme no chão em Y=800)
    pawn = new Pawn(180.0f, 700.0f);
    scene->Add(pawn, MOVING);

    // Inimigo 1: Patrulha em cima da plataforma esquerda (Y=600, nasce em Y=550)
    Enemy* e1 = new Enemy(ENEMY_PAWN, 250.0f, 550.0f, 120.0f, 450.0f, 2, 1);
    scene->Add(e1, MOVING);
    enemiesRemaining++;

    // Inimigo 2: Patrulha no chão principal (Y=800, nasce em Y=750)
    Enemy* e2 = new Enemy(ENEMY_PAWN, 700.0f, 750.0f, 500.0f, 950.0f, 2, 1);
    scene->Add(e2, MOVING);
    enemiesRemaining++;

    //// Boss Torre: No canto direito, apoiada no chão (nasce em Y=740 devido à altura maior dela)
    boss = new TowerBoss(W - 150.0f, 740.0f);
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
    if (window->KeyPress(VK_ESCAPE)) {
        Engine::Next<Home>();
        return;
    }
        

    // Ativa/Desativa as caixas de colisão
    if (window->KeyPress('B'))
    {
        gViewBBox = !gViewBBox;
    }

    if (stageClear)
    {
        stageClearTimer -= gameTime;
        if (stageClearTimer <= 0.0f)
            Engine::Next<Stage2>();
        return;
    }

    // CORREÇÃO: Garante a ordem correta de atualização da física da Inertia
    scene->Update();
    scene->CollisionDetection();

    if (pawn && pawn->isDead)
    {
        Engine::Next<GameOver>();
        return;
    }

    CheckWinCondition();
}

// ---------------------------------------------------------------------------------

void Stage1::Draw()
{

    // Se tiver um sprite de fundo real mais para a frente, ele entra aqui.
    if (backg)
        backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);


    // scene->Draw() vai passar pelas plataformas e não vai desenhar nada (já que o sprite é nulo)
    scene->Draw();

    // ====== DESENHO PURE DE BBOX ======
    // Se você apertar B, a engine varre todos os objetos e desenha a linha de colisão deles!
    if (gViewBBox)
    {
        scene->DrawBBox();
    }

    // Desenha a interface (HUD)
    /*if (hud && pawn)
        hud->Draw(pawn, "Est. 1-1: A Torre");*/

}

// ---------------------------------------------------------------------------------

void Stage1::Finalize()
{
    // Persiste corações antes de trocar de fase
    if (pawn) gPawnHearts = pawn->hearts;

    delete scene;
    gScene = nullptr;

    delete audio;
    gAudio = nullptr;

    delete hud;
}
