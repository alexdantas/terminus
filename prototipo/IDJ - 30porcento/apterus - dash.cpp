
int Apterus::update(int dt){

    input = input->getInstance();
    acceleration = jump = 0;

    if(renderAction != dashLeft && renderAction != dashRight){
        if(input->isKeyPressed(SDLK_UP)){
            jump = -8;
        }
        if(input->isKeyPressed(SDLK_RIGHT)){
            acceleration = 5;
        }
        if(input->isKeyPressed(SDLK_LEFT)){
            acceleration = -5;
        }
        if(input->isKeyPressed(SDLK_DOWN)){
            jump = 5;
        }
    }
    else{
        if(renderAction == dashLeft)
            acceleration = -10;
        else
            acceleration = 10;
    }

    speed += acceleration;

    box.x += speed;
    box.y += jump;

    speed = 0;

    return 1;
}