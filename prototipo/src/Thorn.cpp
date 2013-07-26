 #include "Thorn.hpp"

 Thorn::Thorn(std::string file, int x, int y, int w, int h):
     GameObject(x,y,w,h)
 {
    sprite = new Sprite(file);
    this->desiredPosition = new Rectangle();
 }

void Thorn::update(float dt)
{
    this->position->x -= (this->box->x - this->desiredPosition->x);
    this->position->y -= (this->box->y - this->desiredPosition->y);

    // Refreshing next bounding box
    this->box->copy(this->desiredPosition);
}

void Thorn::render(float cameraX, float cameraY)
{
    this->sprite->render(this->position->x - cameraX,
                         this->position->y - cameraY);

    if(Config::showBoundingBoxes){
        Rectangle tmp(this->box->x - cameraX,
                          this->box->y - cameraY,
                          this->box->w,
                          this->box->h);

        Graphics::drawRectangle(tmp);
    }
}

void Thorn::setPositionCollision(int y)
{
    this->box->setY(y);
}

void Thorn::setPositionSprite(int y)
{
    this->position->y = y;
}

void Thorn::setPosition(float x, float y)
{
    this->desiredPosition->copy(this->box);

    this->desiredPosition->setY(y);
    this->desiredPosition->setX(x);
}

void Thorn::addPosition(float x, float y)
{
    this->desiredPosition->copy(this->box);

    this->desiredPosition->addY(y);
    this->desiredPosition->addX(x);
}
