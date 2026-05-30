/**********************************************************************************
// Pawn.cpp - Jogador Pe√£o
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

static const float PAWN_W = 48.0f;
static const float PAWN_H = 64.0f;

// ---------------------------------------------------------------------------------

Pawn::Pawn(float posX, float posY)
{
    gamepad = new Controller();
    gamepadOn = gamepad->Initialize();

    MoveTo(posX, posY, Layer::MIDDLE);
    type = OBJ_PAWN;
    halfW = PAWN_W * 0.5f;
    halfH = PAWN_H * 0.5f;

    velX = velY = 0.0f;
    onGround = droppingThrough = false;
    facingRight = true;
    isDead = false;

    hearts = gPawnHearts;
    iFrameTimer = 0.0f;
    swordTimer = 0.0f;
    swordCooldownTimer = 0.0f;
    shootCooldownTimer = 0.0f;
    hurtTimer = 0.0f;
    swordHitbox = nullptr;

    state = PAWN_IDLE;
    tileSet = nullptr;
    anim = nullptr;

    BBox(new Rect(-halfW, -halfH, halfW, halfH));
}

// ---------------------------------------------------------------------------------

Pawn::~Pawn()
{
    delete gamepad;
    delete anim;
    delete tileSet;
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

    if (hearts <= 0) { hearts = 0; isDead = true; state = PAWN_DEAD; }
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

    if (gamepadOn && gamepad) gamepad->UpdateState();

    bool moveLeft = false, moveRight = false;
    bool jumpPress = false, dropDown = false;
    bool attackMelee = false, attackRanged = false;

    if (gControlMode == CTRL_KEYBOARD_MOUSE || gControlMode == CTRL_KEYBOARD_ONLY)
    {
        moveLeft = window->KeyDown('A') || window->KeyDown(VK_LEFT);
        moveRight = window->KeyDown('D') || window->KeyDown(VK_RIGHT);
        jumpPress = window->KeyPress(VK_SPACE) || window->KeyPress('W') || window->KeyPress(VK_UP);
        dropDown = window->KeyDown('S') || window->KeyDown(VK_DOWN);
        attackMelee = window->KeyPress('J');
        attackRanged = window->KeyPress('K');
    }
    else if (gControlMode == CTRL_XBOX && gamepadOn && gamepad)
    {
        moveLeft = gamepad->XboxAnalog(LeftThumb) < -8000 || gamepad->XboxButton(DpadLeft);
        moveRight = gamepad->XboxAnalog(LeftThumb) > 8000 || gamepad->XboxButton(DpadRight);
        jumpPress = gamepad->XboxButton(ButtonA);
        dropDown = gamepad->XboxButton(DpadDown);
        attackMelee = gamepad->XboxButton(ButtonX);
        attackRanged = gamepad->XboxButton(ButtonY);
    }

    if (moveLeft) { velX = -PAWN_MOVE_SPEED; facingRight = false; }
    if (moveRight) { velX = PAWN_MOVE_SPEED; facingRight = true; }

    // Pulo
    droppingThrough = false;
    if (jumpPress && onGround)
    {
        velY = -PAWN_JUMP_FORCE;
        onGround = false;
        if (gAudio) gAudio->Play(SND_PLAYER_JUMP);
    }

    // Cair pela plataforma
    if (dropDown)
    {
        droppingThrough = true;
        if (!onGround) velY += PAWN_FALL_FORCE * gameTime;
    }

    if (attackMelee && swordCooldownTimer <= 0.0f) TrySwordAttack();
    if (attackRanged && shootCooldownTimer <= 0.0f) TryRangedAttack();
}

// ---------------------------------------------------------------------------------

void Pawn::TrySwordAttack()
{
    swordCooldownTimer = PAWN_SWORD_COOLDOWN;
    swordTimer = PAWN_SWORD_DURATION;
    state = PAWN_ATTACK;
    if (gAudio) gAudio->Play(SND_SWORD_SWING);

    float hitX = facingRight ? x + halfW + 24.0f : x - halfW - 24.0f;
    swordHitbox = new SwordHitbox(hitX, y, PAWN_SWORD_DAMAGE, PAWN_SWORD_DURATION);
    if (gScene) gScene->Add(swordHitbox, MOVING);
}

// ---------------------------------------------------------------------------------

void Pawn::TryRangedAttack()
{
    if (!gHasRangedAttack) return;
    shootCooldownTimer = PAWN_SHOOT_COOLDOWN;
    float dirX = facingRight ? 1.0f : -1.0f;
    if (gScene) gScene->Add(new PawnProjectile(x, y, dirX), MOVING);
}

// ---------------------------------------------------------------------------------

void Pawn::ApplyPhysics()
{
    if (!onGround) velY += PAWN_GRAVITY * gameTime;

    Translate(velX * gameTime, velY * gameTime);

    // Fallback ch√£o (borda inferior)
    if (y + halfH >= window->Height() - 10.0f)
    {
        MoveTo(x, window->Height() - 10.0f - halfH);
        velY = 0.0f; onGround = true;
    }

    // Limites laterais
    if (x - halfW < 0.0f)              MoveTo(halfW, y);
    if (x + halfW > window->Width())   MoveTo(window->Width() - halfW, y);

    onGround = false;  // Platform confirma via OnCollision
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
    if (iFrameTimer > 0.0f) iFrameTimer -= gameTime;
    if (swordTimer > 0.0f) swordTimer -= gameTime;
    if (swordCooldownTimer > 0.0f) swordCooldownTimer -= gameTime;
    if (shootCooldownTimer > 0.0f) shootCooldownTimer -= gameTime;
    if (hurtTimer > 0.0f) hurtTimer -= gameTime;

    if (isDead) return;

    HandleInput();
    ApplyPhysics();
    UpdateAnimation();
}

// ---------------------------------------------------------------------------------

void Pawn::Draw()
{
    if (anim)
    {
        bool visible = !(iFrameTimer > 0.0f && (int)(iFrameTimer * 10.0f) % 2 == 1);
        if (visible) anim->Draw(x, y, z, facingRight ? 1.0f : -1.0f, 0.0f);
        return;
    }

    // ---- PLACEHOLDER (enquanto n„o h· sprite) ----
    bool blinkVisible = !(iFrameTimer > 0.0f && (int)(iFrameTimer * 10.0f) % 2 == 1);
    if (!blinkVisible) return;

    DWORD color = 0xFF8B4513;               // marrom
    if (state == PAWN_HURT)   color = 0xFFFF4444;
    if (state == PAWN_DEAD)   color = 0xFF444444;
    if (state == PAWN_ATTACK) color = 0xFFFFD700;

    Rect body(-halfW, -halfH, halfW, halfH);
    body.MoveTo(x, y);
    Engine::renderer->Draw(&body, color);

    // Tri‚ngulo de direÁ„o
    float ax = facingRight ? x + halfW - 6.0f : x - halfW + 6.0f;
    Rect arrow(ax - 5.0f, y - 5.0f, ax + 5.0f, y + 5.0f);
    Engine::renderer->Draw(&arrow, 0xFFFFFFFF);
}

// ---------------------------------------------------------------------------------

void Pawn::OnCollision(Object* obj)
{
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