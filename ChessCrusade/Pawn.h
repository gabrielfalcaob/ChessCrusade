/**********************************************************************************
// Pawn.h - Jogador (Peão de Xadrez)
//
// Descrição: Controla movimento, pulo, ataque corpo a corpo e especial.
//            Suporta 3 modos de controle: Teclado+Mouse, Teclado, Xbox.
//            Tem sistema de corações com invencibilidade temporária após dano.
**********************************************************************************/

#ifndef _PEAOREBELDE_PAWN_H_
#define _PEAOREBELDE_PAWN_H_

#include "Object.h"
#include "Animation.h"
#include "Sprite.h"
#include "PeaoRebelde.h"
#include "Controller.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------
// Estados de animação do peão

enum PawnState
{
    PAWN_IDLE,
    PAWN_WALK,
    PAWN_JUMP,
    PAWN_FALL,
    PAWN_ATTACK,
    PAWN_HURT,
    PAWN_DEAD
};

// ---------------------------------------------------------------------------------

class SwordHitbox;   // forward declaration

class Pawn : public Object
{
private:
    // --- Animação ---
    TileSet*   tileSet;
    Animation* anim;
    PawnState  state;

    Controller* gamepad;    // Ponteiro para o controle da Engine
    bool        gamepadOn;

    // --- Cooldowns e timers ---
    float iFrameTimer;          // invencibilidade após dano
    float swordTimer;           // duração do hitbox da espadada ativa
    float swordCooldownTimer;   // cooldown para próxima espadada
    float shootCooldownTimer;   // cooldown do ataque especial
    float hurtTimer;            // tempo no estado HURT

    // --- Hitbox da espada (objeto temporário) ---
    SwordHitbox* swordHitbox;

    // --- Lógica interna ---
    void HandleInput();
    void ApplyPhysics();
    void UpdateAnimation();
    void TrySwordAttack();
    void TryRangedAttack();

    bool facingRight;           // direção que o peão está olhando

public:
    // --- Física (públicos para Platform e Enemy acessarem) ---
    float velX, velY;
    bool  onGround;
    bool  droppingThrough;      // está tentando cair por plataforma
    float halfW, halfH;         // metade das dimensões (para cálculo de colisão)

    // --- Estado de vida ---
    int   hearts;               // corações atuais
    bool  isDead;
    bool  isInvincible() const { return iFrameTimer > 0.0f; }
    bool  isSwordActive() const { return swordTimer > 0.0f; }
    bool  canShoot() const { return shootCooldownTimer <= 0.0f && gHasRangedAttack; }
    float ShootCooldownRatio() const; // 0..1 para barra de cooldown

    Pawn(float posX, float posY);
    ~Pawn();

    void TakeDamage(int amount = 1);
    void Heal(int amount = 1);

    void OnCollision(Object* obj) override;
    void Update()                 override;
    void Draw()                   override;
};

// ---------------------------------------------------------------------------------
#endif
