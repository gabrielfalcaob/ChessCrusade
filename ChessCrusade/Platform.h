/**********************************************************************************
// Platform.h - Plataforma do jogo
//
// Descrição: Plataforma sólida. O jogador pousa em cima.
//            Com S/baixo pressionado, o jogador atravessa a plataforma.
**********************************************************************************/

#ifndef _PEAOREBELDE_PLATFORM_H_
#define _PEAOREBELDE_PLATFORM_H_

#include "Object.h"
#include "Sprite.h"
#include "PeaoRebelde.h"

// ---------------------------------------------------------------------------------

class Platform : public Object
{
private:
    Sprite* sprite;
    float   halfW, halfH;

public:
    // posX/posY = centro da plataforma
    // w/h = largura e altura em pixels
    // spritePath = nullptr usa retângulo placeholder (cinza)
    Platform(float posX, float posY, float w, float h,
             const char* spritePath = nullptr);
    ~Platform();

    float Top()    const { return y - halfH; }
    float Bottom() const { return y + halfH; }
    float Left()   const { return x - halfW; }
    float Right()  const { return x + halfW; }

    void OnCollision(Object* obj) override;
    void Update()  override;
    void Draw()    override;
};

// ---------------------------------------------------------------------------------
#endif
