#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <SDL_mixer.h>
#include <cstdlib>

using namespace std;

//declaración de variables

void GameLoop();
int processInput();
int Salida();
int Pausa();
int checkColisiones();
void RefreshScreen();
void RefreshFrame();
void RefreshVidas();
void RefreshBola();
void Pprincipal();
void RefreshBloques();
void ReiniciarValores();
void PantallaFinal();
int NumeroRandom(int mayot, int menor);
bool PresionarX();
bool PresionarENTER();
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color colour;

SDL_Texture *plataforma;
SDL_Texture *fondo;
SDL_Texture *marco;
SDL_Texture *vida;
SDL_Texture *bienvenida;
SDL_Texture *bola;
SDL_Texture *brojo;
SDL_Texture *bturquesa;
SDL_Texture *bfucsia;
SDL_Texture *bverde;
SDL_Texture *bcafe;
SDL_Texture *marcop;
SDL_Texture *despedida;
SDL_Texture *texture_curso_msg;
SDL_Texture *texture_sem_msg;
SDL_Texture *texture_name_msg;
SDL_Texture *texture_player;
SDL_Texture *texture_name;
SDL_Texture *texture_pressX;
SDL_Texture *texture_leaveGame;
SDL_Texture *texture_pausa1;
SDL_Texture *texture_pausa2;
SDL_Texture *texture_integrante1;
SDL_Texture *texture_integrante2;
SDL_Texture *texture_integrante3;
SDL_Texture *texture_integrante4;
SDL_Texture *texture_carne1;
SDL_Texture *texture_carne2;
SDL_Texture *texture_carne3;
SDL_Texture *texture_carne4;
SDL_Texture *texture_tit_integr;
SDL_Texture *texture_pressENTER;
SDL_Texture *texture_tit_rend;
SDL_Texture *texture_tit_proc;
SDL_Texture *texture_endGame;
SDL_Texture *texture_congrats;
SDL_Texture *texture_novida;
SDL_Texture *texture_luck;
SDL_Texture *texture_intento;
SDL_Texture *texture_Xconti;
SDL_Texture *texture_justEnter;

SDL_Rect dest_rect;
SDL_Rect dest_plat;
SDL_Rect dest_marco;
SDL_Rect dest_vida;
SDL_Rect dest_bienvenida;
SDL_Rect dest_bola;
SDL_Rect dest_bloque;
SDL_Rect dest_marcop;
SDL_Rect dest_despedida;
SDL_Rect curso_msg_rect;
SDL_Rect sem_msg_rect;
SDL_Rect name_msg_rect;
SDL_Rect player_rect;
SDL_Rect name_rect;
SDL_Rect pressX_rect;
SDL_Rect leaveGame_rect;
SDL_Rect pausa1_rect;
SDL_Rect pausa2_rect;
SDL_Rect integrante1_rect;
SDL_Rect integrante2_rect;
SDL_Rect integrante3_rect;
SDL_Rect integrante4_rect;
SDL_Rect carne1_rect;
SDL_Rect carne2_rect;
SDL_Rect carne3_rect;
SDL_Rect carne4_rect;
SDL_Rect tit_integr_rect;
SDL_Rect pressENTER_rect;
SDL_Rect tit_proc_rect;
SDL_Rect tit_rend_rect;
SDL_Rect endGame_rect;
SDL_Rect congrats_rect;
SDL_Rect novida_rect;
SDL_Rect luck_rect;
SDL_Rect intento_rect;
SDL_Rect Xconti_rect;
SDL_Rect justEnter_rect;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gBeep = NULL;

SDL_Event evt_nomb;
TTF_Font* sans;
int tam_letra = 50;
int tam_player = 0;
int tam_erase_player = 13;
bool vidas[3];
bool bloques[65];
bool ciclo = true;
char nombre[80];
int cicloMov;
int xMov;
int yMov;
int xCantMov;
int yCantMov = 5;
int bloquesy =163;
int contB=64;
int cont_vidas;
bool flag_vidas;
int state;
int chequeo;
bool comienzo;
bool start;
int total_vidas;

// Manage error messages
void check_error_sdl(bool check, const char* message);
void check_error_sdl_img(bool check, const char* message);
void check_error_sdl_audio(bool check, const char* message);

// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);
SDL_Texture* cargar_texto(TTF_Font *fuente, const char *texto, SDL_Color color, SDL_Renderer *renderer);

int main(int argc, char** argv) {
    // Initialize SDL
    check_error_sdl(SDL_Init(SDL_INIT_EVERYTHING) != 0, "Unable to initialize SDL");

    // Create and initialize a 800x600 window
    window = SDL_CreateWindow("Micronoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,800,600, SDL_WINDOW_OPENGL);
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
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0){
            printf("SDL_Mixer error");
     }
    gMusic = Mix_LoadMUS("bmbr2battle.wav"); //Puede ser un archivo en formato WAVE, MOD, MIDI, OGG, MP3, FLAC
    if (gMusic == NULL){
        printf("No cargo el archivo Musica");
    }
    //Load sound effects
    gBeep = Mix_LoadWAV( "ball_beep_5.wav" );
    if (gBeep == NULL){
        printf("No cargo el archivo Sonido");
    }

    // carga de imagenes
    fondo = load_texture("fondo.png", renderer);
    plataforma = load_texture("plataforma.png", renderer);
    marco = load_texture("marco.png", renderer);
    vida = load_texture("vida.png", renderer);
    bienvenida = load_texture("bienvenida.png", renderer);
    bola = load_texture("bola.png",renderer);
    brojo = load_texture("brojo.png", renderer);
    bturquesa = load_texture("bturquesa.png", renderer);
    bfucsia = load_texture("bfucsia.png", renderer);
    bverde = load_texture("bverde.png", renderer);
    bcafe = load_texture("bcafe.png", renderer);
    marcop = load_texture("marcop.png",renderer);
    despedida = load_texture("despedida.png", renderer);

    colour = {000,200,000};                                 //Color del texto
    sans = TTF_OpenFont("ComicSans.ttf", tam_letra);        //Abre la fuente
    texture_curso_msg = cargar_texto(sans, "EL-4313-Lab. Estructura de Microprocesadores", colour,renderer);
    texture_sem_msg = cargar_texto(sans, "2S-2016", colour,renderer);
    texture_name_msg = cargar_texto(sans, "Ingrese el nombre del jugador: ", colour,renderer);
    texture_name = cargar_texto(sans, "Jugador: ", colour,renderer);
    texture_pressX = cargar_texto(sans, "Presione X para iniciar", colour,renderer);
    texture_leaveGame = cargar_texto(sans, "¿Seguro que desea abandonar el juego? (S/N)", colour,renderer);
    texture_pausa1 = cargar_texto(sans, "Juego pausado", colour,renderer);
    texture_pausa2 = cargar_texto(sans, "Presione P para continuar", colour,renderer);
    texture_integrante1 = cargar_texto(sans, "Melissa Fonseca Rodríguez", colour,renderer);
    texture_integrante2 = cargar_texto(sans, "Johan Arrieta Solórzano", colour,renderer);
    texture_integrante3 = cargar_texto(sans, "Jafet Chaves Barrantes", colour,renderer);
    texture_integrante4 = cargar_texto(sans, "Dayhana Sánchez Jiménez", colour,renderer);
    texture_carne1 = cargar_texto(sans, "2013027576", colour,renderer);
    texture_carne2 = cargar_texto(sans, "2013021822", colour,renderer);
    texture_carne3 = cargar_texto(sans, "2013037524", colour,renderer);
    texture_carne4 = cargar_texto(sans, "201281852", colour,renderer);
    texture_tit_integr = cargar_texto(sans, "Integrantes: ", colour,renderer);
    texture_pressENTER = cargar_texto(sans, "Presione ENTER para terminar", colour,renderer);
    texture_tit_rend = cargar_texto(sans, "Rendimiento:", colour,renderer);
    texture_tit_proc = cargar_texto(sans, "Procesador:", colour,renderer);
    texture_endGame = cargar_texto(sans, "Juego terminado", colour,renderer);
    texture_congrats = cargar_texto(sans, "Felicidades", colour,renderer);
    texture_novida = cargar_texto(sans, "Pierde una vida", colour,renderer);
    texture_luck = cargar_texto(sans, "Mejor suerte la próxima vez", colour,renderer);
    texture_intento = cargar_texto(sans, "Intento fallido", colour,renderer);
    texture_Xconti = cargar_texto(sans, "Presione X para continuar", colour,renderer);
    texture_justEnter = cargar_texto(sans, "Presione ENTER", colour,renderer);

    // definiendo tamaño y ubicacion de las texturas
    dest_rect.x = 0;            dest_rect.y = 0;
    dest_rect.w = 800;          dest_rect.h = 600;
    dest_plat.x = 340;          dest_plat.y =503;
    dest_plat.w = 120;          dest_plat.h = 21;
    dest_marco.x = 0;           dest_marco.y =0 ;
    dest_marco.w = 800;         dest_marco.h = 600;
    dest_vida.x = 690;          dest_vida.y =550;
    dest_vida.w = 25;           dest_vida.h = 26;
    dest_bienvenida.x = 40;     dest_bienvenida.y = 100;
    dest_bienvenida.w = 715;    dest_bienvenida.h = 73;
    dest_bola.w = 15;           dest_bola.h = 15;
    dest_bloque.x = 16;         dest_bloque.y =12;
    dest_bloque.w = 59;         dest_bloque.h = 30;
    dest_marcop.x = 0;          dest_marcop.y =0;
    dest_marcop.w = 800;        dest_marcop.h = 600;
    dest_despedida.x = 45;      dest_despedida.y =80;
    dest_despedida.w = 714;     dest_despedida.h = 194;
    curso_msg_rect.x = 100;     curso_msg_rect.y = 250;           //Destino del mensaje del curso
    curso_msg_rect.w = 610;     curso_msg_rect.h = 45;
    sem_msg_rect.x = 360;       sem_msg_rect.y = 300;               //Destino del semestre
    sem_msg_rect.w = 90;        sem_msg_rect.h = 45;
    name_msg_rect.x = 130;      name_msg_rect.y = 390;             //Destino del mensaje para ingresar el nombre
    name_msg_rect.w = 325;      name_msg_rect.h = 35;
    name_rect.x = 30;           name_rect.y = 545;             //Destino del mensaje para el jugador
    name_rect.w = 95;           name_rect.h = 35;
    pressX_rect.x = 240;        pressX_rect.y = 283;             //Destino del mensaje para presione X
    pressX_rect.w = 295;        pressX_rect.h = 35;
    leaveGame_rect.x = 123;     leaveGame_rect.y = 283;             //Destino del mensaje afirmación
    leaveGame_rect.w = 553;     leaveGame_rect.h = 35;
    pausa1_rect.x = 316;        pausa1_rect.y = 262;             //Destino del mensaje pausa
    pausa1_rect.w = 167;        pausa1_rect.h = 35;
    pausa2_rect.x = 239;        pausa2_rect.y = 303;             //Destino del mensaje pausa
    pausa2_rect.w = 322;        pausa2_rect.h = 35;
    integrante1_rect.x = 15;    integrante1_rect.y = 476;             //Destino del mensaje integrante
    integrante1_rect.w = 248;   integrante1_rect.h = 26;
    integrante2_rect.x = 15;    integrante2_rect.y = 504;             //Destino del mensaje integrante
    integrante2_rect.w = 228;   integrante2_rect.h = 26;
    integrante3_rect.x = 15;    integrante3_rect.y = 532;             //Destino del mensaje integrante
    integrante3_rect.w = 220;   integrante3_rect.h = 25;
    integrante4_rect.x = 15;    integrante4_rect.y = 560;             //Destino del mensaje integrante
    integrante4_rect.w = 230;   integrante4_rect.h = 25;
    carne1_rect.x = 280;        carne1_rect.y = 485;             //Destino del mensaje carne
    carne1_rect.w = 117;        carne1_rect.h = 23;
    carne2_rect.x = 280;        carne2_rect.y = 510;             //Destino del mensaje carne
    carne2_rect.w = 117;        carne2_rect.h = 23;
    carne3_rect.x = 280;        carne3_rect.y = 535;             //Destino del mensaje carne
    carne3_rect.w = 117;        carne3_rect.h = 23;
    carne4_rect.x = 280;        carne4_rect.y = 560;             //Destino del mensaje carne
    carne4_rect.w = 105;        carne4_rect.h = 23;
    tit_integr_rect.x = 15;     tit_integr_rect.y = 445;             //Destino del mensaje titulo integrantes
    tit_integr_rect.w = 156;    tit_integr_rect.h = 28;
    pressENTER_rect.x = 240;    pressENTER_rect.y = 315;             //Destino del mensaje para presione ENTER
    pressENTER_rect.w = 360;    pressENTER_rect.h = 35;
    tit_proc_rect.x = 500;      tit_proc_rect.y = 535;             //Destino del mensaje para procesador
    tit_proc_rect.w = 117;      tit_proc_rect.h = 23;
    tit_rend_rect.x = 500;      tit_rend_rect.y = 485;             //Destino del mensaje para rendimiento
    tit_rend_rect.w = 129;      tit_rend_rect.h = 23;
    endGame_rect.x = 291;       endGame_rect.y = 283;
    endGame_rect.w = 192;       endGame_rect.h = 35;
    congrats_rect.x = 317;      congrats_rect.y = 245;
    congrats_rect.w = 141;      congrats_rect.h = 35;
    novida_rect.x = 291;        novida_rect.y = 283;
    novida_rect.w = 192;        novida_rect.h = 35;
    luck_rect.x = 214;          luck_rect.y = 283;
    luck_rect.w = 346;          luck_rect.h = 35;
    intento_rect.x = 291;       intento_rect.y = 245;
    intento_rect.w = 192;       intento_rect.h = 35;
    Xconti_rect.x = 227;        Xconti_rect.y = 321;
    Xconti_rect.w = 321;        Xconti_rect.h = 35;
    justEnter_rect.x = 297;     justEnter_rect.y = 321;
    justEnter_rect.w = 180;     justEnter_rect.h = 35;

    //mostrar fondo inicial
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer,bienvenida, NULL, &dest_bienvenida);
    SDL_RenderCopy(renderer, texture_curso_msg,NULL,&curso_msg_rect);
    SDL_RenderCopy(renderer, texture_sem_msg,NULL,&sem_msg_rect);
    SDL_RenderCopy(renderer, texture_name_msg,NULL,&name_msg_rect);
    SDL_RenderCopy(renderer, marcop, NULL, &dest_marcop);
    SDL_RenderPresent(renderer);                //Coloca el texto en pantalla

    //se inicia el proceso de captura del nombre
    Pprincipal();
    player_rect.x = 122;   player_rect.y = 545;     //Recoloca el nombre del jugador
    //Inicializa variables
    for(int i=0;i<=2;i++){vidas[i]=true;}
    ReiniciarValores();
    RefreshScreen();
    GameLoop();                                    //se inicia el ciclo del juego
    PantallaFinal();
    // Clear the allocated resources
    SDL_DestroyTexture(plataforma);
    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(vida);
    SDL_DestroyTexture(bienvenida);
    SDL_DestroyTexture(brojo);
    SDL_DestroyTexture(bfucsia);
    SDL_DestroyTexture(bverde);
    SDL_DestroyTexture(bcafe);
    SDL_DestroyTexture(bturquesa);
    SDL_DestroyTexture(marcop);
    SDL_DestroyTexture(despedida);
    SDL_DestroyTexture(texture_pressX);

    //Free the sound effects
    Mix_FreeChunk(gBeep);
    gBeep = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

    IMG_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void ReiniciarValores(){
    srand(time(NULL));
    cicloMov = NumeroRandom(2,0);
    for(int i=0;i<=64;i++){bloques[i]=true;}        //Inicializa los bloques
    xMov = 391; yMov = 488;                         //Inicializa la posición de la bola en 'x' y 'y'
    dest_bola.x=391; dest_bola.y=488;
    dest_plat.x = 340; dest_plat.y =503;            //Inicializa la posición de la plataforma
}

int NumeroRandom(int mayor, int menor){
    return rand() %mayor + menor;
}

// In case of error, print the error code and close the application
void check_error_sdl(bool check, const char* message) {
    if (check) {
        std::cout << message << " " << SDL_GetError() << std::endl;
        SDL_Quit();
        //std::exit(-1);
    }
}

// In case of error, print the error code and close the application
void check_error_sdl_img(bool check, const char* message) {
    if (check) {
        std::cout << message << " " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        //std::exit(-1);
    }
}

// Cargar las imágenes y el texto
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(fname);
    check_error_sdl_img(image == nullptr, "Unable to load image");

    SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
    check_error_sdl_img(img_texture == nullptr, "Unable to create a texture from the image");
    SDL_FreeSurface(image);
    return img_texture;
}
SDL_Texture* cargar_texto(TTF_Font *fuente, const char *texto, SDL_Color color, SDL_Renderer *renderer){
    SDL_Surface *mensaje = TTF_RenderText_Solid(fuente, texto, color);
    SDL_Texture *msg_texture = SDL_CreateTextureFromSurface(renderer,mensaje);
    SDL_FreeSurface(mensaje);
    return msg_texture;
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

//ciclo de refrezcamiento de la pantalla
void RefreshScreen(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer, plataforma, NULL, &dest_plat);
    SDL_RenderCopy(renderer, texture_name,NULL,&name_rect);
    SDL_RenderCopy(renderer, texture_player,NULL,&player_rect);
    RefreshBloques();
    RefreshVidas();
    RefreshBola();
    SDL_RenderCopy(renderer, marco, NULL, &dest_marco);
    SDL_RenderPresent(renderer);

}

void RefreshBola(){                 //Posiciona la bola y realiza los ciclos de movimiento
    dest_bola.x = xMov; dest_bola.y = yMov;
    chequeo = checkColisiones();
    if (chequeo == 3){                          //Implica que la bola llegó al piso
        ReiniciarValores();                            //Inicializa variables
    }
    else if (cicloMov == 0){
        if (chequeo == 1 || chequeo ==4){
            xMov = xMov +xCantMov;
            yMov = yMov +yCantMov;
            cicloMov = 3;
        }
        else if (chequeo == 2 || xMov>774){
            xMov = xMov -xCantMov;
            yMov = yMov -yCantMov;
            cicloMov = 1;
        }
        else {
            xMov = xMov +xCantMov;
            if (xMov>775){xMov=775;}
            yMov = yMov -yCantMov;
        }
    }
    else if (cicloMov == 1){
        if (chequeo == 1 || chequeo ==4){
            xMov = xMov -xCantMov;
            yMov = yMov +yCantMov;
            cicloMov = 2;
        }
        else if (chequeo == 2 || xMov<=10){
            xMov = xMov +xCantMov;
            yMov = yMov -yCantMov;
            cicloMov = 0;
        }
        else {
            xMov = xMov -xCantMov;
            if (xMov<10){xMov=10;}
            yMov = yMov -yCantMov;
        }
    }
    else if (cicloMov == 2){
        if (chequeo == 1 || chequeo == 4){
            xMov = xMov -xCantMov;
            yMov = yMov -yCantMov;
            cicloMov =1;
        }
        else if (chequeo == 2 ||xMov<=10){
            xMov = xMov +xCantMov;
            yMov = yMov +yCantMov;
            cicloMov = 3;
        }
        else if (yMov>=533){
            xMov = xMov -xCantMov;
            yMov = yMov -yCantMov;
            cicloMov = 1;
        }
        else {
            xMov = xMov -xCantMov;
            if (xMov<10){xMov=10;}
            yMov = yMov +yCantMov;
        }
    }
    else if (cicloMov == 3){
        if (chequeo == 1 || chequeo == 4){
            xMov = xMov + xCantMov;
            yMov = yMov - yCantMov;
            cicloMov = 0;
        }
        else if (chequeo == 2 ||xMov>=775){
            xMov = xMov -xCantMov;
            yMov = yMov +yCantMov;
            cicloMov = 2;
        }
        else if (yMov>=533){
            xMov = xMov +xCantMov;
            yMov = yMov -yCantMov;
            cicloMov = 0;
        }
        else {
            xMov = xMov +xCantMov;
            if (xMov>775){xMov=775;}
            yMov = yMov +yCantMov;
        }
    }

    SDL_RenderCopy(renderer, bola,NULL,&dest_bola);
}

//pdest_bola.y == bloquesyroceso de la pantalla principal
void Pprincipal(){
    while (ciclo){
        while (SDL_PollEvent(&evt_nomb)){
            switch (evt_nomb.type){
                case SDL_TEXTINPUT:
                    strcat(nombre,evt_nomb.text.text);  //concatena al texto del nombre
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
                    SDL_RenderCopy(renderer,bienvenida, NULL, &dest_bienvenida);
                    SDL_RenderCopy(renderer, texture_curso_msg,NULL,&curso_msg_rect);
                    SDL_RenderCopy(renderer, texture_sem_msg,NULL,&sem_msg_rect);
                    SDL_RenderCopy(renderer, texture_name_msg,NULL,&name_msg_rect);
                    SDL_RenderCopy(renderer, marcop, NULL, &dest_marcop);
                    tam_player = tam_player + 11;
                    texture_player = cargar_texto(sans,nombre,colour,renderer);
                    player_rect.x = 455;   player_rect.y = 390;
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
                        ciclo = false;
                        break;
                    /*case SDLK_BACKSPACE:
                        nombre[tam_player/11] =NULL;
                        tam_player = tam_player -11;
                        player = TTF_RenderText_Solid(sans,nombre,colour);
                        texture_player = SDL_CreateTextureFromSurface(renderer,player);
                        player_rect.x = 450;   player_rect.y = 390;
                        player_rect.w = tam_player;   player_rect.h = 35;
                        SDL_RenderCopy(renderer, texture_player,NULL,&player_rect);
                        SDL_RenderPresent(renderer);*/
                    }
            }
        }
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

//ciclo de detección de colisiones
int checkColisiones(){
    int resultado=0;
    int centro_bola = xMov +8;
    int bloquesx = 724;
    bloquesy=163;
    int numbloquex=0;
    int numbloquey=0;
    if (dest_bola.y<=163){
        //colision vertical
        for(int i=0;i<=12;i++){
            if (centro_bola>=bloquesx){
            numbloquex=12-i;
            i=13;
            }
            bloquesx=bloquesx-59;
        }
        for(int j=0;j<=4;j++){
            if ((cicloMov==0)||(cicloMov==1)){
                if(yMov==bloquesy){
                   numbloquey=5-j;
                   resultado=1;
                }
                bloquesy=bloquesy-30;
            }
            if ((cicloMov==2)||(cicloMov==3)){
                if((yMov+12)==(bloquesy-30)){
                   numbloquey=5-j;
                   resultado=1;
                }
                bloquesy=bloquesy-30;
            }
        }
        //colision horizontal
        if (numbloquey==0){
            if ((dest_bola.y<163)&&(dest_bola.y>133)){
                numbloquey=5;
                resultado=2;
            }
            else if ((dest_bola.y<133)&&(dest_bola.y>103)){
                numbloquey=4;
                resultado=2;
            }
            else if ((dest_bola.y<103)&&(dest_bola.y>73)){
                numbloquey=3;
                resultado=2;
            }
            else if ((dest_bola.y<73)&&(dest_bola.y>43)){
                numbloquey=2;
                resultado=2;
            }
            else if ((dest_bola.y<43)&&(dest_bola.y>13)){
                numbloquey=1;
                resultado=2;
            }

            if (cicloMov==2){
                for(int i=0;i<12;i++){
                    if (xMov==bloquesx){
                        numbloquex=11-i;
                        i=13;
                        resultado=2;
                    }
                    bloquesx=bloquesx-59;
                }
            }
            else if (cicloMov==3){
                for(int i=0;i<12;i++){
                    if ((xMov+15)==bloquesx){
                        numbloquex=12-i;
                        i=13;
                        resultado=2;
                    }
                    bloquesx=bloquesx-59;
                }
            }
        }
        //comprobación del bloque implicado
        switch(numbloquey){
            case 0:
                resultado=0;
                break;
            case 1:
                if (bloques[numbloquex]){bloques[numbloquex]=false;}
                else{resultado=0;}
                break;
            case 2:
                if (bloques[(numbloquex+13)]){bloques[(numbloquex+13)]=false;}
                else{resultado=0;}
                break;
            case 3:
                if (bloques[numbloquex+26]){bloques[numbloquex+26]=false;}
                else{resultado=0;}
                break;
            case 4:
                if (bloques[numbloquex+39]){bloques[numbloquex+39]=false;}
                else{resultado=0;}
                break;
            case 5:
                if (bloques[(numbloquex+52)]){bloques[(numbloquex+52)]=false;}
                else{resultado=0;}
                break;
        }
    }
    else if (dest_bola.y>531){              //Choque con el piso
        vidas[cont_vidas] = false;
        cont_vidas++;
        resultado = 3;
        if(cont_vidas==3){
            flag_vidas=true;
        }
    }
    if ((dest_bola.y+15)==dest_plat.y && (dest_bola.x+8)>= dest_plat.x && (dest_bola.x+7)<=(dest_plat.x + dest_plat.w) && (cicloMov == 2 || cicloMov == 3)){ //Choque con la plataforma
        resultado = 4;
    }
    if (dest_bola.y == 13){    //Choque con el techo
        resultado = 4;
    }
    if (resultado == 1 || resultado == 2){total_vidas++;Mix_PlayChannel( -1, gBeep, 0 );}
    return resultado;
}

//ciclo principal
void GameLoop(){
    cont_vidas = 0;

    flag_vidas = false;                         //Se pone en true cuando se acaban las vidas
    bool pantalla_final= true;
    int cambio_estado = 1;
    bool mainloop = true;
    chequeo = 0;
    comienzo = false;
    start = true;
    while (flag_vidas == false && mainloop && total_vidas !=65){
            //cout << total_vidas <<endl;
        if (Mix_PlayingMusic()==0){
            Mix_PlayMusic( gMusic, -1 );                //Reproduce la música indefinidamente
        }
        else if (Mix_PausedMusic() == 1){
            Mix_ResumeMusic();
        }


        if (chequeo == 3){                  //
            comienzo = false;
        }
        while (comienzo == false){
            comienzo = PresionarX();
            total_vidas = 0;
            xCantMov = NumeroRandom(7,1);
            RefreshFrame();
            if (start){SDL_RenderCopy(renderer, texture_pressX, NULL, &pressX_rect);}
            else {
                SDL_RenderCopy(renderer, texture_Xconti, NULL, &Xconti_rect);
                SDL_RenderCopy(renderer, texture_intento, NULL, &intento_rect);
                SDL_RenderCopy(renderer, texture_novida, NULL, &novida_rect);
            }
            if (comienzo){start = false;}
            SDL_RenderPresent(renderer);                //Coloca el texto en pantalla
        }
        RefreshScreen();
        bool loop = true;
        cambio_estado = processInput();
        if (cambio_estado == 0){
            RefreshFrame();
            SDL_RenderCopy(renderer, texture_leaveGame, NULL, &leaveGame_rect);
            SDL_RenderPresent(renderer);
            while (loop){
                    cambio_estado = Salida();
                    if (cambio_estado==0){loop = false; mainloop= false;}
                    else if (cambio_estado==1) {loop = false;}
            }
        }
        else if (cambio_estado == 2){
            RefreshFrame();
            SDL_RenderCopy(renderer, texture_pausa1, NULL, &pausa1_rect);
            SDL_RenderCopy(renderer, texture_pausa2, NULL, &pausa2_rect);
            SDL_RenderPresent(renderer);
            while (loop){
                    cambio_estado = Pausa();
                    if (cambio_estado==0){loop = false;}
            }

        }
    }
    if (Mix_PlayingMusic()!=0){
        Mix_PauseMusic();
    }
    if (flag_vidas){
        while (pantalla_final){            //Hasta que no presione ENTER no pasa a la pantalla final
            pantalla_final = PresionarENTER();
            RefreshFrame();
            endGame_rect.y = 245;
            SDL_RenderCopy(renderer, texture_justEnter, NULL, &justEnter_rect);
            SDL_RenderCopy(renderer, texture_luck, NULL, &luck_rect);
            SDL_RenderCopy(renderer, texture_endGame, NULL, &endGame_rect);
            SDL_RenderPresent(renderer);                //Coloca el texto en pantalla
        }
    }
    if (total_vidas == 65){
        while (pantalla_final){
            pantalla_final = PresionarENTER();
            RefreshFrame();
            endGame_rect.y = 283;
            SDL_RenderCopy(renderer, texture_justEnter, NULL, &justEnter_rect);
            SDL_RenderCopy(renderer, texture_congrats, NULL, &congrats_rect);
            SDL_RenderCopy(renderer, texture_endGame, NULL, &endGame_rect);
            SDL_RenderPresent(renderer);                //Coloca el texto en pantalla
        }
    }
}

void RefreshFrame(){
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer, bola, NULL, &dest_bola);
    SDL_RenderCopy(renderer, marco, NULL, &dest_marco);
    SDL_RenderCopy(renderer, plataforma, NULL, &dest_plat);
    SDL_RenderCopy(renderer, texture_name,NULL,&name_rect);
    SDL_RenderCopy(renderer, texture_player,NULL,&player_rect);
    RefreshBloques();
    RefreshVidas();
}

bool PresionarX(){
    SDL_Event teclaX;
    bool salida=false;
    while (SDL_PollEvent(&teclaX)){
        switch (teclaX.type){
            case SDL_KEYDOWN:
            switch (teclaX.key.keysym.sym){
                case SDLK_x:
                salida = true;
                break;
            }
        }
    }
    return salida;
}
bool PresionarENTER(){
    SDL_Event teclaENTER;
    bool salida=true;
    while (SDL_PollEvent(&teclaENTER)){
        switch (teclaENTER.type){
            case SDL_KEYDOWN:
            switch (teclaENTER.key.keysym.sym){
                case SDLK_RETURN:
                salida = false;
                break;
            }
        }
    }
    return salida;
}

int Pausa(){
    state = 1;
    SDL_Event pause;
    while (SDL_PollEvent(&pause)){
        switch (pause.type){
        case SDL_KEYDOWN:
            switch (pause.key.keysym.sym){
                case SDLK_p:
                    state = 0;
                    break;
            }
        }
    }
    return state;
}

int Salida(){
    state = 2;
    SDL_Event out;
    while (SDL_PollEvent(&out)){
        switch (out.type){
        case SDL_KEYDOWN:
            switch (out.key.keysym.sym){
                case SDLK_s:
                    state = 0;
                    break;
                case SDLK_n:
                    state = 1;
                    break;
            }
        }
    }
    return state;
}

int processInput(){
    state = 1;
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
                    state = 0;
                    break;
                case SDLK_p:
                    state = 2;
                    break;
            }
        }
    }
    return state;
}

void PantallaFinal(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer, marcop, NULL, &dest_marcop);
    SDL_RenderCopy(renderer, despedida, NULL, &dest_despedida);
    SDL_RenderCopy(renderer, texture_integrante1, NULL, &integrante1_rect);
    SDL_RenderCopy(renderer, texture_integrante2, NULL, &integrante2_rect);
    SDL_RenderCopy(renderer, texture_integrante3, NULL, &integrante3_rect);
    SDL_RenderCopy(renderer, texture_integrante4, NULL, &integrante4_rect);
    SDL_RenderCopy(renderer, texture_carne1, NULL, &carne1_rect);
    SDL_RenderCopy(renderer, texture_carne2, NULL, &carne2_rect);
    SDL_RenderCopy(renderer, texture_carne3, NULL, &carne3_rect);
    SDL_RenderCopy(renderer, texture_carne4, NULL, &carne4_rect);
    SDL_RenderCopy(renderer, texture_tit_integr, NULL, &tit_integr_rect);
    SDL_RenderCopy(renderer, texture_pressENTER, NULL, &pressENTER_rect);
    SDL_RenderCopy(renderer, texture_tit_proc, NULL, &tit_proc_rect);
    SDL_RenderCopy(renderer, texture_tit_rend, NULL, &tit_rend_rect);
    SDL_RenderPresent(renderer);
    ciclo = true;
    while (ciclo){
        ciclo = PresionarENTER();
    }
    SDL_DestroyTexture(texture_integrante1);
    SDL_DestroyTexture(texture_integrante2);
    SDL_DestroyTexture(texture_integrante3);
    SDL_DestroyTexture(texture_integrante4);
    SDL_DestroyTexture(texture_carne1);
    SDL_DestroyTexture(texture_carne2);
    SDL_DestroyTexture(texture_carne3);
    SDL_DestroyTexture(texture_carne4);
    SDL_DestroyTexture(texture_tit_integr);
    SDL_DestroyTexture(texture_pressENTER);
    SDL_DestroyTexture(texture_tit_proc);
    SDL_DestroyTexture(texture_tit_rend);
}


