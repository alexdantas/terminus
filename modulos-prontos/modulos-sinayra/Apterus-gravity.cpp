//Deixei como foram feitos os trabalhos pra não causar conflito
//Todo modo, deve ser mais fácil de arrumar
//Os números são arbitrários
int Apterus::update(int dt){

    input = input->getInstance();
    acceleration = jump = 0;

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

    speed += acceleration;

    box.x += speed;
    box.y += jump;

    speed = 0;

    return 1;
}


///Calcula a gravidade exercida no Apterus
// http://www.dreamincode.net/forums/topic/227175-c-sdl-sprite-jump/
//Se estiver no ar, então calcule a gravidade
//aceleração = somente a aceleração da Gravidade = 9.8
//velocidade = v(t) + acc
//Ou seja, é a velocidade naquele tempo t + a aceleração constante
//posição = s(t) + v(novo t)
void Apterus::gravity(int dt, bool air){
    float acc = 0;
    if(air){
        acc += GRAVITY_ACC * dt/3;
        yVel += acc * dt/3;
        box.y += yVel * dt/3;
        inAir = true;
    }
    else{
        yVel = 0;
        inAir = false;
    }
}