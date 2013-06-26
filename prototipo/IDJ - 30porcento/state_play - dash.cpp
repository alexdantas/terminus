
int State_play::update(int dt){
    Ponto followApterus;

    input->update();
    processEvents();

    if(input->isKeyDown(SDLK_p)){
        paused = (paused ? false : true);
        return GAME_NO_CHANGE;
    }

    if(!paused){

        checkCollision(dt);

        apterus->update(dt);
        apterus->gravity(dt, air);
        if(state == dashLeft || state == dashRight){
            if(apterusAniDashLeft->inLoop()){
                apterus->updateAnimation(state, dt);
            }
            else{
                apterusAniDashLeft->setLoop(1);
                state = apterusState();
            }
        }
        else{
            state = apterusState();
            apterus->updateAnimation(state, dt);
        }



        /*if(explodiu){
            timer->start();
            if(timer->getTime() > 2){
                if(ufo)
                    return GAME_LOSE;
                else
                    return GAME_WIN;
            }
        }*/

        //followApterus = apterus->getPos();
        //followApterus.x -= 400;
        //followApterus.y -= 300;
        //camera->setCamera(followApterus.x, followApterus.y);
        return GAME_NO_CHANGE;
    }
    else{
        if(input->isKeyDown(SDLK_v))
            return GAME_START;
    }

}