/**********************************************************************************
// PeaoRebelde.h - Definições Globais do Jogo (Corrigido)
**********************************************************************************/

#ifndef _PEAOREBELDE_H_
#define _PEAOREBELDE_H_

#include "Game.h"
#include "Scene.h"
#include "Audio.h"

// ---------------------------------------------------------------------------------
// 1. DEFINIÇÃO DOS ENUMS (O compilador precisa ver isso ANTES de qualquer variável)
// ---------------------------------------------------------------------------------
enum ControlMode
{
    CTRL_KEYBOARD_MOUSE,    // WASD + Mouse
    CTRL_KEYBOARD_ONLY,     // WASD apenas
    CTRL_XBOX               // Controle do Xbox
};

enum ObjectTypes
{
    OBJ_PAWN = 1,
    OBJ_ENEMY,
    OBJ_BOSS,
    OBJ_PLATFORM,
    OBJ_PROJECTILE,
    OBJ_HEART_DROP,
    OBJ_SWORD_HIT
};

enum Sounds
{
    SND_SWORD_SWING,
    SND_SWORD_HIT,
    SND_PLAYER_HURT,
    SND_PLAYER_JUMP,
    SND_ENEMY_DEATH,
    SND_BOSS_HURT,
    SND_BOSS_DEATH,
    SND_HEART_PICKUP,
    SND_STAGE_CLEAR
};

// ---------------------------------------------------------------------------------
// 2. CONFIGURAÇÕES GERAIS DO JOGO
// ---------------------------------------------------------------------------------
#define WINDOW_WIDTH            1600
#define WINDOW_HEIGHT           860

#define PAWN_GRAVITY            1500.0f
#define PAWN_MOVE_SPEED         420.0f
#define PAWN_JUMP_FORCE         850.0f
#define PAWN_FALL_FORCE         3000.0f
#define PAWN_MAX_HEARTS         10
#define PAWN_START_HEARTS       5
#define PAWN_IFRAMES            1.5f
#define PAWN_SWORD_DURATION     0.2f
#define PAWN_SWORD_COOLDOWN     0.4f
#define PAWN_SHOOT_COOLDOWN     3.0f
#define PAWN_SWORD_DAMAGE       1
#define DROP_PLATFORM_THRESHOLD 12.0f

// ---------------------------------------------------------------------------------
// 3. DECLARAÇÃO DAS VARIÁVEIS GLOBAIS (EXTERN)
// ---------------------------------------------------------------------------------
class Scene;
class Audio;

extern Scene* gScene;
extern Audio* gAudio;

extern int  gCurrentStage;
extern int  gPawnHearts;
extern bool gHasRangedAttack;
extern long gScore;

// Agora o compilador já sabe o que é ControlMode, então aceitará perfeitamente:
extern ControlMode gControlMode;
extern bool        gViewBBox;

#endif