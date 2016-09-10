#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>

//Para que funcione compilar en la computadora de Jafet hay que  agregar #include <cstdlib>, #include <stdlib.h>

using namespace std;


//declaracin de variables

void GameLoop();
int processInput();
void RefreshScreen();
void RefreshVidas();
void Pprincipal();
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color colour;

SDL_Texture *plataforma;
SDL_Texture *fondo;
SDL_Texture *marco;
SDL_Texture *vida;
SDL_Texture *bienvenida;
SDL_Texture *brojo;
SDL_Texture *bturquesa;
SDL_Texture *bfucsia;
SDL_Texture *bverde;
SDL_Texture *bcafe;
SDL_Texture *texture_curso_msg;
SDL_Texture *texture_sem_msg;
SDL_Texture *texture_name_msg;
SDL_Texture *texture_player;
SDL_Texture *texture_erase_player;

SDL_Rect dest_rect;
SDL_Rect dest_plat;
SDL_Rect dest_marco;
SDL_Rect dest_vida;
SDL_Rect dest_bienvenida;
SDL_Rect dest_bloque;
SDL_Rect curso_msg_rect;
SDL_Rect sem_msg_rect;
SDL_Rect name_msg_rect;
SDL_Rect player_rect;
SDL_Rect erase_player_rect;

SDL_Surface *bienv_mensaje;
SDL_Surface *curso_msg;
SDL_Surface *sem_msg;
SDL_Surface *name_msg;
SDL_Surface *player;
SDL_Surface *erase_player;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gBeep = NULL;

SDL_Event evt_nomb;
TTF_Font* stocky;
TTF_Font* sans;
int tam_letra = 75;
int tam_player = 0;
int tam_erase_player = 13;
bool vidas[3]{true,true,true};
bool bloques[65];
bool ciclo = true;
char nombre[80];


// Manage error messages
void check_error_sdl(bool check, const char* message);
void check_error_sdl_img(bool check, const char* message);
void check_error_sdl_audio(bool check, const char* message);

// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);


int main(int argc, char** argv) {
    // Initialize SDL
    check_error_sdl(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, "Unable to initialize SDL");

    // Create and initialize a 800x600 window
    window = SDL_CreateWindow("Test SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,800,600, SDL_WINDOW_OPENGL);
    check_error_sdl(window == nullptr, "Unable to create window");

    // Create and initialize a hardware accelerated renderer that will be refreshed in sync with your monitor (at approx. 60 Hz)

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_error_sdl(renderer == nullptr, "Unable to create a renderer");


    // Initialize SDL_img
    int flags=IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    check_error_sdl_img((initted & flags) != flags, "Unable to initialize SDL_image");

    //Inicializa ttf
    TTF_Init();

    //Initialize SDL_mixer
	int audio_init = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	check_error_sdl_audio(audio_init, "Unable to initialize SDL_audio");

    // se inicializa el arreglo de enable de los bloques
    for(int i=0;i<=64;i++){
    bloques[i]=true;
    }

    // carga de imagenes
    fondo = load_texture("fondo2.png", renderer);
    plataforma = load_texture("plataforma.png", renderer);
    marco = load_texture("marco.png", renderer);
    vida = load_texture("vida.png", renderer);
    bienvenida = load_texture("bienvenida.png", renderer);
    brojo = load_texture("brojo.png", renderer);
    bturquesa = load_texture("bturquesa.png", renderer);
    bfucsia = load_texture("bfucsia.png", renderer);
    bverde = load_texture("bverde.png", renderer);
    bcafe = load_texture("bcafe.png", renderer);

    //Load music
    gMusic = Mix_LoadMUS("bmbr2battle.wav"); //Puede ser un archivo en formato WAVE, MOD, MIDI, OGG, MP3, FLAC
    check_error_sdl_audio(gMusic, "Unable to initialize SDL_audio");

    //Load sound effects
    gBeep = Mix_LoadWAV( "ball_beep_5.wav" );
    check_error_sdl_audio(gMusic, "Unable to initialize SDL_audio");

    colour = {0,200,0};                     //Color del texto
    stocky = TTF_OpenFont("From_Cartoon_Blocks.ttf", 100);   //Carga una fuente para texto
    sans = TTF_OpenFont("FreeSansBold.ttf", tam_letra);
    curso_msg = TTF_RenderText_Solid(sans, "EL-4313-Lab. Estructura de Microprocesadores", colour);
    sem_msg = TTF_RenderText_Solid(sans, "2S-2016", colour);
    name_msg = TTF_RenderText_Solid(sans, "Ingrese el nombre del jugador: ", colour);


    texture_curso_msg = SDL_CreateTextureFromSurface(renderer,curso_msg);
    texture_sem_msg = SDL_CreateTextureFromSurface(renderer,sem_msg);
    texture_name_msg = SDL_CreateTextureFromSurface(renderer,name_msg);


    // definiendo tamaño y ubicacion de las texturas

    dest_rect.x = 0; dest_rect.y = 0;
    dest_rect.w = 800; dest_rect.h = 600;
    dest_plat.x = 340; dest_plat.y =490 ;
    dest_plat.w = 120; dest_plat.h = 21;
    dest_marco.x = 0; dest_marco.y =0 ;
    dest_marco.w = 800; dest_marco.h = 600;
    dest_vida.x = 690; dest_vida.y =550;
    dest_vida.w = 25; dest_vida.h = 26;
    dest_bienvenida.x = 40; dest_bienvenida.y =100;
    dest_bienvenida.w = 715; dest_bienvenida.h = 73;
    dest_bloque.x = 16; dest_bloque.y =12;
    dest_bloque.w = 59; dest_bloque.h = 30;
    curso_msg_rect.x = 150;   curso_msg_rect.y = 250;           //Destino del mensaje del curso
    curso_msg_rect.w = 510;   curso_msg_rect.h = 45;
    sem_msg_rect.x = 360;   sem_msg_rect.y = 300;               //Destino del semestre
    sem_msg_rect.w = 90;   sem_msg_rect.h = 45;
    name_msg_rect.x = 130;   name_msg_rect.y = 390;             //Destino del mensaje para el nombre
    name_msg_rect.w = 312;   name_msg_rect.h = 35;

    //mostrar fondo inicial
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer,bienvenida, NULL, &dest_bienvenida);
    SDL_RenderCopy(renderer, texture_curso_msg,NULL,&curso_msg_rect);
    SDL_RenderCopy(renderer, texture_sem_msg,NULL,&sem_msg_rect);
    SDL_RenderCopy(renderer, texture_name_msg,NULL,&name_msg_rect);
    SDL_RenderPresent(renderer);                //Coloca el texto en pantalla

    //se inicia el proceso de captura del nombre
    Pprincipal();

    //se inicia el ciclo del juego
    GameLoop();

    // Clear the allocated resources
    SDL_DestroyTexture(plataforma);
    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(vida);
    SDL_DestroyTexture(bienvenida);

    //Free the sound effects
    Mix_FreeChunk(gBeep);
    gBeep = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

	//Quit SDL subsystems
	Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
//juego

// In case of error, print the error code and close the application
void check_error_sdl(bool check, const char* message) {
    if (check) {
        std::cout << message << " " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(-1);
    }
}

// In case of error, print the error code and close the application
void check_error_sdl_img(bool check, const char* message) {
    if (check) {
        std::cout << message << " " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        std::exit(-1);
    }
}

// In case of error, print the error code and close the application
void check_error_sdl_audio(bool check, const char* message){
    if (check) {
        std::cout << message << " " << Mix_GetError() << std::endl;
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
        std::exit(-1);
    }
}

// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(fname);
    check_error_sdl_img(image == nullptr, "Unable to load image");

    SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
    check_error_sdl_img(img_texture == nullptr, "Unable to create a texture from the image");
    SDL_FreeSurface(image);
    return img_texture;
}

//ciclo de refrecamiento de vidas
void RefreshVidas(){
if (vidas[0]==true){
    dest_vida.x=690;
    SDL_RenderCopy(renderer, vida, NULL, &dest_vida);
}
if (vidas[1]==true){
    dest_vida.x=720;
    SDL_RenderCopy(renderer, vida, NULL, &dest_vida);
}
if (vidas[2]==true){
    dest_vida.x=750;
    SDL_RenderCopy(renderer, vida, NULL, &dest_vida);
    }
}

void RefreshBloques(){
    int contador=0;
    int despx=1;
    int despy=1;
    int rotBloques=1;
    dest_bloque.x=16;
    dest_bloque.y=12;
    while(despy<=5){
        while(despx<=13){
            switch(rotBloques){
                case 1:
                    if (bloques[contador]==true){
                        SDL_RenderCopy(renderer, brojo, NULL, &dest_bloque);
                    }
                    break;
                case 2:
                    if (bloques[contador]==true){
                        SDL_RenderCopy(renderer, bturquesa, NULL, &dest_bloque);
                    }
                    break;
                case 3:
                    if (bloques[contador]==true){
                        SDL_RenderCopy(renderer, bfucsia, NULL, &dest_bloque);
                    }
                    break;
                case 4:
                    if (bloques[contador]==true){
                        SDL_RenderCopy(renderer, bverde, NULL, &dest_bloque);
                    }
                    break;
                case 5:
                    if (bloques[contador]==true){
                        SDL_RenderCopy(renderer, bcafe, NULL, &dest_bloque);
                    }
                    break;
            }
            rotBloques=rotBloques+1;
            if (rotBloques==6){rotBloques=1;}
            despx=despx+1;
            if (despx==14){rotBloques=rotBloques+1;}
            if (rotBloques==6){rotBloques=1;}
            dest_bloque.x=dest_bloque.x+59;
            contador=contador+1;
        }
        dest_bloque.x=16;
        dest_bloque.y=dest_bloque.y+30;
        despy=despy+1;
        despx=1;
    }
}

//ciclo de refrezcamiento de la pantalla
void RefreshScreen(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer, marco, NULL, &dest_marco);
    SDL_RenderCopy(renderer, plataforma, NULL, &dest_plat);
    RefreshVidas();
    RefreshBloques();
    SDL_RenderPresent(renderer);

}

//proceso de la pantalla principal
void Pprincipal(){
    while (ciclo){
        while (SDL_PollEvent(&evt_nomb)){
            switch (evt_nomb.type){
                case SDL_TEXTINPUT:
                    strcat(nombre,evt_nomb.text.text);  //concatena al texto del nombre
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
                    SDL_RenderCopy(renderer,bienvenida, NULL, &dest_bienvenida);
                    //SDL_RenderCopy(renderer, texture_bienv_mens,NULL,&bienv_mensaje_rect);  //Pone el texto en el Render
                    SDL_RenderCopy(renderer, texture_curso_msg,NULL,&curso_msg_rect);
                    SDL_RenderCopy(renderer, texture_sem_msg,NULL,&sem_msg_rect);
                    SDL_RenderCopy(renderer, texture_name_msg,NULL,&name_msg_rect);
                    tam_player = tam_player + 13;
                    player = TTF_RenderText_Solid(sans,nombre,colour);
                    texture_player = SDL_CreateTextureFromSurface(renderer,player);
                    player_rect.x = 450;   player_rect.y = 390;
                    player_rect.w = tam_player;   player_rect.h = 35;
                    SDL_RenderCopy(renderer, texture_player,NULL,&player_rect);
                    SDL_RenderPresent(renderer);
                    break;
                case SDL_KEYDOWN:
                    switch(evt_nomb.key.keysym.sym){
                    case SDLK_RETURN:
                        SDL_DestroyTexture(texture_curso_msg);
                        SDL_DestroyTexture(texture_sem_msg);
                        SDL_DestroyTexture(texture_name_msg);
                        SDL_DestroyTexture(texture_player);
                        ciclo = false;
                        break;
                    }
            }
        }
    }

}

//ciclo principal
void GameLoop(){
    RefreshScreen();
    Mix_PlayMusic( gMusic, -1 );//Reproduce la música indefinidamente
    //Mix_PlayChannel( -1, gBeep, 0 );;//Para reproducir el efecto de sonido del beep
    int cambio_estado = 1;
    while (cambio_estado != 0){
        cambio_estado = processInput();
    }
}

int processInput(){

    int state = 1;
    SDL_Event evento;
    SDL_StartTextInput();
    while (SDL_PollEvent(&evento)){ //SDL_PollEvent() Pregunta al procesador cuales eventos han pasado
        switch (evento.type){
        case SDL_QUIT:
            state = 0;
            break;
        case SDL_KEYDOWN:
            switch(evento.key.keysym.sym){
                case SDLK_c:
                    if (dest_plat.x<670){
                        dest_plat.x += 10;
                        RefreshScreen();
                    }
                    break;
                case SDLK_z:
                    if (dest_plat.x>10){
                        dest_plat.x -= 10;
                        RefreshScreen();
                    }
                    break;
                case SDLK_q:
                    state=0;
                    break;

            }
        }

    }
    return state;
}
