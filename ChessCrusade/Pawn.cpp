/**********************************************************************************
// Pawn.cpp - Jogador Peão
**********************************************************************************/

#include "Pawn.h"
#include "SwordHitbox.h"
#include "PawnProjectile.h"
#include "Enemy.h"
#include "HeartDrop.h"
#include "Engine.h"
#include <cmath>
#include <algorithm>

using namespace std;

// Dimensões do placeholder do peão
static const float PAWN_W = 48.0f;
static const float PAWN_H = 64.0f;

// ---------------------------------------------------------------------------------

Pawn::Pawn(float posX, float posY)
{
    // Inicialização nativa do controle do Xbox
    gamepad = new Controller();
    gamepadOn = gamepad->Initialize();

    MoveTo(posX, posY, Layer::MIDDLE);
    type = OBJ_PAWN;
    halfW = PAWN_W * 0.5f;
    halfH = PAWN_H * 0.5f;

    velX = 0.0f;
    velY = 0.0f;
    onGround = false;
    droppingThrough = false;
    facingRight = true;
    isDead = false;

    hearts = gPawnHearts;  // persiste entre fases
    iFrameTimer = 0.0f;
    swordTimer = 0.0f;
    swordCooldownTimer = 0.0f;
    shootCooldownTimer = 0.0f;
    hurtTimer = 0.0f;
    swordHitbox = nullptr;

    state = PAWN_IDLE;
    tileSet = nullptr;
    anim = nullptr;

    // TODO: carregar spritesheet quando disponível
    // tileSet = new TileSet("Resources/pawn_sheet.png", 64, 64, 8, 40);
    // anim    = new Animation(tileSet, 0.1f, true);

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Pawn::~Pawn()
{
    delete gamepad;
    delete anim;
    delete tileSet;
    // swordHitbox é deletado pela cena automaticamente
}

// ---------------------------------------------------------------------------------

void Pawn::TakeDamage(int amount)
{
    if (isInvincible() || isDead) return;

    hearts -= amount;
    iFrameTimer = PAWN_IFRAMES;
    hurtTimer = 0.25f;
    state = PAWN_HURT;

    if (gAudio) gAudio->Play(SND_PLAYER_HURT);

    if (hearts <= 0)
    {
        hearts = 0;
        isDead = true;
        state = PAWN_DEAD;
    }

    // Atualiza global para persistir entre fases
    gPawnHearts = hearts;
}

// ---------------------------------------------------------------------------------

void Pawn::Heal(int amount)
{
    hearts = min(hearts + amount, PAWN_MAX_HEARTS);
    gPawnHearts = hearts;
    if (gAudio) gAudio->Play(SND_HEART_PICKUP);
}

// ---------------------------------------------------------------------------------

float Pawn::ShootCooldownRatio() const
{
    return max(0.0f, shootCooldownTimer / PAWN_SHOOT_COOLDOWN);
}

// ---------------------------------------------------------------------------------

void Pawn::HandleInput()
{
    velX = 0.0f;

    // Se o controle do Xbox estiver ativo, atualiza o estado interno dele
    if (gamepadOn && gamepad)
    {
        gamepad->UpdateState();
    }

    // -------------------------------------------------------------------------
    // MOVIMENTAÇÃO LATERAL (EIXO X)
    // -------------------------------------------------------------------------
    if (gControlMode == CTRL_KEYBOARD_MOUSE || gControlMode == CTRL_KEYBOARD_ONLY)
    {
        if (window->KeyDown('D') || window->KeyDown(VK_RIGHT))  velX = PAWN_MOVE_SPEED;
        if (window->KeyDown('A') || window->KeyDown(VK_LEFT))   velX = -PAWN_MOVE_SPEED;
    }
    else if (gControlMode == CTRL_XBOX && gamepadOn && gamepad)
    {
        // Correção de escopo baseada nas constantes nativas do seu Controller.h
        if (gamepad->XboxAnalog(LeftThumb) > 8000 || gamepad->XboxButton(DpadRight))  velX = PAWN_MOVE_SPEED;
        if (gamepad->XboxAnalog(LeftThumb) < -8000 || gamepad->XboxButton(DpadLeft)) velX = -PAWN_MOVE_SPEED;
    }

    if (velX > 0.0f) facingRight = true;
    if (velX < 0.0f) facingRight = false;

    // -------------------------------------------------------------------------
    // PULO E DESCIDA DE PLATAFORMA (EIXO Y)
    // -------------------------------------------------------------------------
    droppingThrough = false;

    if (gControlMode == CTRL_KEYBOARD_MOUSE || gControlMode == CTRL_KEYBOARD_ONLY)
    {
        if (window->KeyDown('S') || window->KeyDown(VK_DOWN))
        {
            droppingThrough = true;
            if (onGround) velY = PAWN_FALL_FORCE * gameTime;
        }

        if (onGround && (window->KeyPress(VK_SPACE) || window->KeyPress('W') || window->KeyPress(VK_UP)))
        {
            velY = -PAWN_JUMP_FORCE;
            onGround = false;
            if (gAudio) gAudio->Play(SND_PLAYER_JUMP);
        }
    }
    else if (gControlMode == CTRL_XBOX && gamepadOn && gamepad)
    {
        // Usa DpadDown para descer das plataformas atravessáveis
        if (gamepad->XboxButton(DpadDown))
        {
            droppingThrough = true;
            if (onGround) velY = PAWN_FALL_FORCE * gameTime;
        }

        // Usa ButtonA para pular no controle
        if (onGround && gamepad->XboxButton(ButtonA))
        {
            velY = -PAWN_JUMP_FORCE;
            onGround = false;
            if (gAudio) gAudio->Play(SND_PLAYER_JUMP);
        }
    }

    // -------------------------------------------------------------------------
    // ATAQUES
    // -------------------------------------------------------------------------
    if (swordCooldownTimer <= 0.0f)
    {
        bool attackPressed = false;
        if (window->KeyPress('J')) attackPressed = true;
        if (gControlMode == CTRL_XBOX && gamepadOn && gamepad && gamepad->XboxButton(ButtonX)) attackPressed = true;

        if (attackPressed) TrySwordAttack();
    }

    if (gHasRangedAttack && shootCooldownTimer <= 0.0f)
    {
        bool shootPressed = false;
        if (window->KeyPress('K')) shootPressed = true;
        if (gControlMode == CTRL_XBOX && gamepadOn && gamepad && gamepad->XboxButton(ButtonY)) shootPressed = true;

        if (shootPressed) TryRangedAttack();
    }
}

// ---------------------------------------------------------------------------------

void Pawn::TrySwordAttack()
{
    if (swordCooldownTimer > 0.0f) return;

    swordCooldownTimer = PAWN_SWORD_COOLDOWN;
    swordTimer = PAWN_SWORD_DURATION;
    state = PAWN_ATTACK;

    if (gAudio) gAudio->Play(SND_SWORD_SWING);

    // Cria hitbox temporária na frente do peão
    float hitX = facingRight ? x + halfW + 24.0f : x - halfW - 24.0f;
    swordHitbox = new SwordHitbox(hitX, y, PAWN_SWORD_DAMAGE, PAWN_SWORD_DURATION);
    if (gScene) gScene->Add(swordHitbox, MOVING);
}

// ---------------------------------------------------------------------------------

void Pawn::TryRangedAttack()
{
    if (!gHasRangedAttack)       return;
    if (shootCooldownTimer > 0.0f) return;

    shootCooldownTimer = PAWN_SHOOT_COOLDOWN;

    float dirX = facingRight ? 1.0f : -1.0f;
    PawnProjectile* proj = new PawnProjectile(x, y, dirX);
    if (gScene) gScene->Add(proj, MOVING);
}

// ---------------------------------------------------------------------------------

void Pawn::ApplyPhysics()
{
    // Gravidade
    if (!onGround)
        velY += PAWN_GRAVITY * gameTime;

    Translate(velX * gameTime, velY * gameTime);

    // Limite inferior da tela (fallback de segurança)
    if (y + halfH >= window->Height() - 10.0f)
    {
        MoveTo(x, window->Height() - 10.0f - halfH);
        velY = 0.0f;
        onGround = true;
    }

    // Limites horizontais da arena
    if (x - halfW < 0.0f)
        MoveTo(halfW, y);
    else if (x + halfW > window->Width())
        MoveTo(window->Width() - halfW, y);

    // Reseta o frame para assumir no ar, Platform confirma via OnCollision
    onGround = false;
}

// ---------------------------------------------------------------------------------

void Pawn::UpdateAnimation()
{
    if (isDead) { state = PAWN_DEAD;   return; }
    if (hurtTimer > 0.f) { state = PAWN_HURT;   return; }
    if (swordTimer > 0.f) { state = PAWN_ATTACK; return; }

    if (!onGround)
        state = velY < 0.0f ? PAWN_JUMP : PAWN_FALL;
    else if (abs(velX) > 10.0f)
        state = PAWN_WALK;
    else
        state = PAWN_IDLE;
}

// ---------------------------------------------------------------------------------

void Pawn::Update()
{
    // Atualiza timers
    if (iFrameTimer > 0.0f)        iFrameTimer -= gameTime;
    if (swordTimer > 0.0f)         swordTimer -= gameTime;
    if (swordCooldownTimer > 0.0f) swordCooldownTimer -= gameTime;
    if (shootCooldownTimer > 0.0f) shootCooldownTimer -= gameTime;
    if (hurtTimer > 0.0f)          hurtTimer -= gameTime;

    if (isDead) return;

    HandleInput();
    ApplyPhysics();
    UpdateAnimation();
}

// ---------------------------------------------------------------------------------

void Pawn::Draw()
{
    // Pisca durante invencibilidade
    bool visible = true;
    if (iFrameTimer > 0.0f)
    {
        int frame = (int)(iFrameTimer * 10.0f);
        visible = (frame % 2 == 0);
    }
    if (!visible) return;

    if (anim)
    {
        // Correção de assinatura nativa da Engine Inertia: Draw(x, y, z, scale, rotation)
        float scaleX = facingRight ? 1.0f : -1.0f;
        anim->Draw(x, y, z, scaleX, 0.0f);
    }
    else
    {
        // ---- PLACEHOLDER GEOMÉTRICO ----
        Engine::renderer->BeginPixels();

        DWORD color = 0xFF8B4513;   // marrom
        if (state == PAWN_HURT)  color = 0xFFFF4444;   // vermelho ao tomar dano
        if (state == PAWN_DEAD)  color = 0xFF444444;   // cinza ao morrer
        if (state == PAWN_ATTACK)color = 0xFFFFD700;   // dourado ao atacar

        Rect body(-halfW, -halfH, halfW, halfH);
        body.MoveTo(x, y);
        Engine::renderer->Draw(&body, color);

        // Indicador de direção: quadrado branco pequeno na borda do olhar
        DWORD arrowColor = 0xFFFFFFFF;
        float ax = facingRight ? x + halfW - 4.0f : x - halfW + 4.0f;
        Rect arrow(ax - 4.0f, y - 4.0f, ax + 4.0f, y + 4.0f);
        Engine::renderer->Draw(&arrow, arrowColor);

        Engine::renderer->EndPixels();
    }
}

// ---------------------------------------------------------------------------------

void Pawn::OnCollision(Object* obj)
{
    // Colisão com drop de coração
    if (obj->Type() == OBJ_HEART_DROP)
    {
        HeartDrop* h = (HeartDrop*)obj;
        if (!h->collected)
        {
            h->collected = true;
            Heal(1);
            if (gScene) gScene->Delete(h, MOVING);
        }
    }
}