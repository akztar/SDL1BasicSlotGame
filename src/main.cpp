// On linux compile with:
// g++ -std=c++17 main.cpp -o prog -lSDL2 -lSDL2_image -ldl

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <vector>
#include <memory>


#include "../include/DynamicText.hpp"
#include "../include/SDLApp.hpp"
#include "../include/TexturedRectangle.hpp"
#include "../include/AnimatedSprite.hpp"
#include "../include/GameEntity.hpp"
#include "../Drum.h"


// One possibility of creating as a global our app
SDLApp* app;

// Create two objects to render
// Eventually, we will want some sort of factory
// to manage object creation in our App...
//GameEntity* object1;
//GameEntity* object2;
DynamicText* FPSText;
DrumStak* DS;
SDL_Rect* topRect;

extern std::vector<std::string> Tiles;
extern SDL_Renderer* Renderer;

static int fPScounterX = 50;
static int fPScounterY = 20;
static int screenX = 1280;
static int screenY = 960;
static int tileX = 100;
static int tileY = 100;

void HandleEvents(){
    SDL_Event event;

    int SecWaiting = 1000;
    // (1) Handle Input
    // Start our event loop
    while(SDL_PollEvent(&event)){
        // Handle each specific event
        if(event.type == SDL_QUIT){
            app->StopAppLoop();
        }
        // Detect collision from our two shapes if mouse
        // button is pressed
        if (event.button.button == SDL_BUTTON_LEFT && DS != nullptr)
            if (SecWaiting == 1000)
            {
                DS->LeverPull();
                SecWaiting = 0;
                std::cout << "L pulled" << std::endl;
                //if(object2->GetTexturedRectangle().IsColliding(object1->GetTexturedRectangle())){
                //            //     std::cout << "Is colliding" << std::endl;
                //            //}else{
                //            //    std::cout << "Not colliding" << std::endl;
                //}
            }
            else
                SecWaiting++;
    }
}


void HandleRendering(){
    // Set draw positions and size
    //object1->GetTexturedRectangle().SetPosition(screenX - app->GetMouseX() > tileX ? app->GetMouseX() : screenX - tileX,
    //                                            screenY - app->GetMouseY() > tileY ? app->GetMouseY() : screenY - tileY);
    //object1->GetTexturedRectangle().SetDimensions(tileX, tileY);

    static int FPS=0;
    /*
    static int posX =0;
    static int posY =0;
    static bool up = true;
    static bool right = true;

    if(up){
        posY--;
    }else{
        posY++;
    }

    if(right){
        posX++;
    }else{
        posX--;
    }
    
    if(posX > app->GetWindowWidth()){
        right = false;
    }else if(posX < 0){
        right = true;
    }

    if(posY < 0){
        up = false;
    }else if(posY > app->GetWindowHeight()){
        up = true;
    }
          */
    FPS = app->GetFPS();

    //object2->GetTexturedRectangle().SetPosition(posX,posY);
    //object2->GetTexturedRectangle().SetDimensions(tileX, tileY);
    if(DS != nullptr)
        DS->Step();
    FPSText->DrawText(Renderer, std::to_string(FPS), screenX - fPScounterX, screenY- fPScounterY, fPScounterX, fPScounterY);
    SDL_RenderFillRect(Renderer, topRect);
    // Render our objects
    //object1->Render();
    //object2->Render();
}


int main(int argc, char* argv[]){
    // Setup the SDLApp
    const char* title = "New SDL2 Abstraction";
    app = new SDLApp(title, 20,20, screenX, screenY);
    app->SetMaxFrameRate(16);
    Renderer = app->GetRenderer();
    srand(SDL_GetTicks());
    topRect = new SDL_Rect();
    topRect->x = 0;
    topRect->y = 0;
    topRect->h = 200;
    topRect->w = 1280;
    // Create any objects in our scene
    //object1 = new GameEntity(app->GetRenderer(),"./images/test.bmp");
    //object2 = new GameEntity(app->GetRenderer(),"./images/test.bmp");
    FPSText = new DynamicText("./assets/fonts/8bitOperatorPlus8-Regular.ttf", 32);
    Tiles.resize(15);
    Tiles[0] = "./images/tile0.bmp";
    Tiles[1] = "./images/tile1.bmp";
    Tiles[2] = "./images/tile2.bmp";
    Tiles[3] = "./images/tile3.bmp";
    Tiles[4] = "./images/tile4.bmp";
    Tiles[5] = "./images/tile5.bmp";
    Tiles[6] = "./images/tile6.bmp";
    Tiles[7] = "./images/tile7.bmp";
    Tiles[8] = "./images/tile8.bmp";
    Tiles[9] = "./images/tile9.bmp";
    Tiles[10] = "./images/tile10.bmp";
    Tiles[11] = "./images/tile11.bmp";
    Tiles[12] = "./images/tile12.bmp";
    Tiles[13] = "./images/tile13.bmp";
    Tiles[14] =  "./images/tile14.bmp";
    DS = new DrumStak(120, 200, 40, 40, 160, 160);
    // Set callback functions
    app->SetEventCallback(HandleEvents);
    app->SetRenderCallback(HandleRendering);
    // Run our application until terminated
    app->RunLoop();

    // Clean up our application
    delete app;
    //delete object1;
    //delete object2;

    return 0;
}
