#include "../include/GameManager.h"

GameManager::GameManager(){
    srand(time(NULL));

    bg = new Sprite("img/clouds.jpg");
    planetSprite = new Sprite("img/PlanetaVermelho.png");
    cavaloSprite= new Sprite("img/cavalinho2.png");

    //cavaloObj = new Cavalo(cavaloSprite,20,0,(600 - cavaloSprite->getWidth()));
    cavaloObj = new Cavalo(cavaloSprite,20,0,0);
}

GameManager::~GameManager(){
    delete(bg);
    delete(planetSprite);
    delete(cavaloSprite);
}

void GameManager::addPlaneta(){
//Adiciona um planet no final do vector
    planetArray.push_back(new Planeta(planetSprite, rand()%20, rand()%800, rand()%600));
}

void GameManager::checkPlaneta(){
    int tamanho = planetArray.size();
    Planeta *planet;

    for(int i = 0; i < tamanho; i++){
        planet = planetArray[i];
        if(planet->getHitpoints() <= 0){
           planetArray.erase(planetArray.begin() + i);
        }
    }
}

void GameManager::processEvents(){
    // procura um evento
    SDL_Event event;
    if( SDL_PollEvent( &event ) ){
        // um evento foi achado
        if( event.type == SDL_QUIT )
            SDL_Quit();
        if( event.type == SDL_MOUSEBUTTONDOWN ){
            // obtém o estado do mouse
            int mouseX, mouseY;
            SDL_GetMouseState( &mouseX, &mouseY );

            // percorre o array de monstros de forma invertida
            // até achar o monstro que o usuário clicou.
            // percorremos de forma invertida porque os últimos
            // monstros são desenhados no topo, e estes serão
            // atingidos primeiro
            for(std::vector<Planeta*>::iterator it = planetArray.end() - 1;it != planetArray.begin() - 1; --it){
                Planeta *planet = *it;

                if( mouseX >= planet->getX() - camera.x && mouseX < planet->getX() + planet->getSprite()->getClip().w - camera.x && mouseY >= planet->getY() - camera.y &&mouseY < planet->getY() + planet->getSprite()->getClip().h - camera.y ){
                    // acerta o planeta, dando dano aleatório entre 10 e 19

                    planet->levaDano(rand() % 10 + 10);

                    // acertamos apenas um monstro
                    // portanto saímos do break
                     break;
                }
            }
        }
        if( event.type == SDL_KEYDOWN ){
            // uma tecla foi pressionada
            if( event.key.keysym.sym == SDLK_ESCAPE ){
                // se a tecla ESC foi pressionada, sair do programa
                SDL_Event quit;
                quit.type = SDL_QUIT;
                SDL_PushEvent( &quit );
            }
            else{
                // caso qualquer outra tecla seja pressionada,
                // chamar a rotina de adicionar monstro
                addPlaneta();
            }
        }
    }
        // fim do loop de eventos
}

void GameManager::movimento(){
    keystate = SDL_GetKeyState(NULL);

    if (keystate[SDLK_LEFT] ) {
      cavaloObj->andeX(-2);
    }
    if (keystate[SDLK_RIGHT] ) {
      cavaloObj->andeX(+2);
    }
    if (keystate[SDLK_UP] ) {
      cavaloObj->andeY(-2);
    }
    if (keystate[SDLK_DOWN] ) {
      cavaloObj->andeY(+2);
    }
/*
    if(cavaloObj->getX() < 0){
        cavaloObj->setX(0);
    }
    else if(cavaloObj->getX() > SCREEN_WIDTH - cavaloSprite->getWidth()){
        cavaloObj->setX((SCREEN_WIDTH - cavaloSprite->getWidth()));
    }

    if(cavaloObj->getY() < 0){
        cavaloObj->setY(0);
    }
    else if(cavaloObj->getY() > SCREEN_HEIGHT - cavaloSprite->getHeight()){
        cavaloObj->setY(SCREEN_HEIGHT - cavaloSprite->getHeight());
    }
    */
}

void GameManager::update()
{
    camera.x = cavaloObj->getX() - 400;
    camera.y = cavaloObj->getY() - 300;
}

void GameManager::render(){
    int tamanho = planetArray.size();

    bg->render(0,0);
    for(int i = 0; i < tamanho; i++){
        planetArray[i]->render(camera.x, camera.y);
    }
    cavaloObj->render(camera);
}

void GameManager::run(){

    while(!SDL_QuitRequested()) {
        /* Captura de Input */
        processEvents();
        movimento();
        /* Atualização dos objetos */
        checkPlaneta();
        update();
        /* Todos os comandos de renderização */
        render();
        /* Atualizar a tela */
        SDLBase::atualizarTela();
    }
}
