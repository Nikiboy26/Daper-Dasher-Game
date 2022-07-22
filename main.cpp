#include "raylib.h"
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running Time 
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;
        // update animation frame
         data.rec.x = data.frame * data.rec.width;
                data.frame++;
                if (data.frame >maxFrame)
                {
                    data.frame = 0;
                }
    }
    return data;
}

int main(){
    double windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

   // initialise window
   InitWindow(windowDimensions[0], windowDimensions[1], "Dasher Game");

   // acceleration due to gravity (pixels/s)/s
   const int gravity{1'200};

   // nebula varibales 
   Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

   const int sizeOfNebula{10};

    AnimData nebulae[sizeOfNebula]{};

    for (int i = 0; i < sizeOfNebula; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8.f;
        nebulae[i].rec.height = nebula.height/8.f;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8.f;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 400;
    }

    float finishLine{ nebulae[sizeOfNebula - 1].pos.x};

   // neb X velocity (pixels per second)
   int nebVel{-320};

   // scarfy variables
   Texture2D scarfy = LoadTexture("textures/scarfy.png");
   AnimData scarfyData;
   scarfyData.rec.width = scarfy.width/6;
   scarfyData.rec.height = scarfy.height;
   scarfyData.rec.x = 0;
   scarfyData.rec.y = 0;
   scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
   scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
   scarfyData.frame = 0;
   scarfyData.updateTime = 1.0/12.0;
   scarfyData.runningTime = 0.0;

   // is the rectangle in the air
   bool isInAir{};
   // jump velocity (pixels/sec)
   const int jumpVel{-600};
 
   int velocity{0};

   // load background layers
   Texture2D background = LoadTexture("textures/far-buildings.png");
   float bgX{0};
   Texture2D midground = LoadTexture("textures/back-buildings.png");
   float mgX{0};
   Texture2D foreground = LoadTexture("textures/foreground.png");
   float fgX{0};

    bool collision{};

   SetTargetFPS(60);
   while(!WindowShouldClose())
   {
        // delta time (time since last frame)
        const float dT{ GetFrameTime() };

       // start drawing
       BeginDrawing();
       ClearBackground(WHITE);

       // scroll background
       bgX -= 20 *dT;
       if (bgX <= -background.width*2)
       {
        bgX = 0.0;
       }
       // scroll midground
       mgX -= 40 *dT;
       if (mgX <= -midground.width*2)
       {
        mgX = 0.0;
       }
       // scroll foreground
       fgX -= 80 *dT;
       if (fgX <= -foreground.width*2)
       {
        fgX = 0.0;
       }

       // draw the background
       Vector2 bg1Pos{bgX, 0.0};
       DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
       Vector2 bg2pos{bgX + (background.width*2), 0.0};
       DrawTextureEx(background, bg2pos, 0.0, 2.0, WHITE);

       // draw midground 
       Vector2 mg1pos{mgX, 0.0};
       DrawTextureEx(midground, mg1pos, 0.0, 2.0, WHITE);
       Vector2 mg2pos{mgX + (midground.width*2), 0.0};
       DrawTextureEx(midground, mg2pos, 0.0, 2.0, WHITE);

       // draw foreground
       Vector2 fg1pos{fgX, 0.0};
       DrawTextureEx(foreground, fg1pos, 0.0, 2.0, WHITE);
       Vector2 fg2pos{fgX + foreground.width*2, 0.0};
       DrawTextureEx(foreground, fg2pos, 0.0, 2.0, WHITE);

       // perform ground check
       if (isOnGround(scarfyData, windowDimensions[1]))
       {
           // scarfy on the ground
           velocity = 0;
           isInAir = false;
       }
else
       {
           // scarfy is in the air
           velocity += gravity * dT;
           isInAir = true;
       }
 
       // jump check
       if (IsKeyPressed(KEY_SPACE) && !isInAir)
       {
           velocity += jumpVel;
       }

       for (int i = 0; i < sizeOfNebula; i++)
       {
        // update the position of each nebula
        nebulae[i].pos.x += nebVel * dT;
       }

       // update scarfy position
       scarfyData.pos.y += velocity * dT;

       // update finish line position 
       finishLine += nebVel * dT;

       // update animation scarfy
       if(!isInAir)
       {
            scarfyData = updateAnimData(scarfyData, dT, 5);
       }
       
       for (AnimData nebula : nebulae)
       {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec)){
                collision = true;
            }
       }

       if (collision)
       {
            // lose the game 
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);

       }
       else if (scarfyData.pos.x >= finishLine)
       {
        DrawText("You win!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);
       }
       else
       {
            // update animation for nebulae
        for (int i = 0; i < sizeOfNebula; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        // draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        // draw nebula 
        for (int i = 0; i < sizeOfNebula; i++)
        {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
       }

       // stop drawing
       EndDrawing();
   }
   UnloadTexture(scarfy);
   UnloadTexture(nebula);
   UnloadTexture(background);
   UnloadTexture(midground);
   UnloadTexture(foreground);
   CloseWindow();
}