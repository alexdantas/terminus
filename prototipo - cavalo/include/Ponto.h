#pragma once

#include "SDLbase.h"
#include "Sprite.h"

class Ponto{
    private:
        float x, y;
    public:
        Ponto();
        void setCamera(Sprite *cavalo, int largura, int altura);
};

