#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;
enum GameState{PLAY, EXIT};

//declaracin de variables
void GameLoop();
int processInput();
void RefreshScreen();
void RefreshVidas();
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *plataforma;
SDL_Texture *fondo;
SDL_Texture *marco;
SDL_Texture *vida;
SDL_Rect dest_rect;
SDL_Rect dest_plat;
SDL_Rect dest_marco;
SDL_Rect dest_vida;
bool vidas[3]{true,true,true};

// Manage error messages
void check_error_sdl(bool check, const char* message);
void check_error_sdl_img(bool check, const char* message);

// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);


int main(int argc, char** argv) {
    // Initialize SDL
    check_error_sdl(SDL_Init(SDL_INIT_VIDEO) != 0, "Unable to initialize SDL");

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

    // carga de imagenes
    fondo = load_texture("fondo.png", renderer);
    plataforma = load_texture("plataforma.png", renderer);
    marco = load_texture("marco.png", renderer);
    vida = load_texture("vida.png", renderer);
    // definiendo tamaño y ubicacion de las texturas

    dest_rect.x = 0; dest_rect.y = 0;
    dest_rect.w = 800; dest_rect.h = 600;

    dest_plat.x = 340; dest_plat.y =490 ;
    dest_plat.w = 120; dest_plat.h = 21;

    dest_marco.x = 0; dest_marco.y =0 ;
    dest_marco.w = 800; dest_marco.h = 600;

    dest_vida.x = 690; dest_vida.y =550;
    dest_vida.w = 25; dest_vida.h = 26;

    //mostrar fondo inicial
    SDL_SetRenderDrawColor(renderer, 29, 29, 29, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderPresent(renderer);

    //se inicia el ciclo del juego
    GameLoop();

    // Clear the allocated resources
    SDL_DestroyTexture(plataforma);
    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(vida);

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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

//ciclo de refrezcamiento de la pantalla
void RefreshScreen(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fondo, NULL, &dest_rect);
    SDL_RenderCopy(renderer, marco, NULL, &dest_marco);
    SDL_RenderCopy(renderer, plataforma, NULL, &dest_plat);
    RefreshVidas();
    SDL_RenderPresent(renderer);

}

//ciclo principal
void GameLoop(){
    GameState _gamestate = GameState::PLAY;
    int cambio_estado = 1;
    while (_gamestate != GameState::EXIT){
        // Update the window surface (show the renderer)

        cambio_estado = processInput();
        if (cambio_estado == 0){
            _gamestate = GameState::EXIT;
        }

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
                case SDLK_x:
                    state=0;
                    break;

            }
        }

    }
    return state;
}
