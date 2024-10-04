/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Simulator Example");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    float Slider001Value = 0.0f;
    float Slider002Value = 0.0f;
    float Slider003Value = 0.0f;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            GuiSlider((Rectangle){ 0, 24, 120, 16 }, NULL, NULL, &Slider001Value, 0, 100);
            GuiSlider((Rectangle){ 0, 72, 120, 16 }, NULL, NULL, &Slider002Value, 0, 100);
            GuiSlider((Rectangle){ 0, 120, 120, 16 }, NULL, NULL, &Slider003Value, 0, 100);
            GuiLabel((Rectangle){ 8, 0, 24, 24 }, "X");
            char buffer[32] = {0};
            sprintf(buffer, "%lf", Slider001Value);
            GuiLabel((Rectangle){ 24, 0, 96, 24 }, buffer);
            GuiLabel((Rectangle){ 8, 48, 24, 24 }, "Y");
            sprintf(buffer, "%lf", Slider002Value);
            GuiLabel((Rectangle){ 24, 48, 96, 24 }, buffer);
            GuiLabel((Rectangle){ 8, 96, 24, 24 }, "Z");
            sprintf(buffer, "%lf", Slider003Value);
            GuiLabel((Rectangle){ 24, 96, 96, 24 }, buffer);
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

