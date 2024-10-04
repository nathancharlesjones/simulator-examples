#include <time.h>           // For clock
#include <stdio.h>          // For printf, scanf
#include <string.h>         // For strtok_r
#include <stdlib.h>         // For atof, atoi
#include <stdint.h>         // For uint32_t
#include <stdbool.h>        // For bool, true, false
#include "application_MainMajor.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64


float Slider001Value = 0.0f;
float Slider002Value = 0.0f;
float Slider003Value = 0.0f;
float SliderBar017Value = 0.0f;
float SliderBar015Value = 0.0f;
bool Button016Pressed = false;

bool rand_accels = false;
double volatility = 0.2;

int main()
{
    srand(time(NULL));

    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------

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
        runTheApplication();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            GuiSlider((Rectangle){ 0, 64, 120, 16 }, NULL, NULL, &Slider001Value, -10, 10);
            GuiSlider((Rectangle){ 0, 112, 120, 16 }, NULL, NULL, &Slider002Value, -10, 10);
            GuiSlider((Rectangle){ 0, 160, 120, 16 }, NULL, NULL, &Slider003Value, -10, 10);

            GuiLabel((Rectangle){ 8, 40, 24, 24 }, "X");
            char buffer[32] = {0};
            sprintf(buffer, "%f", Slider001Value);
            GuiLabel((Rectangle){ 24, 40, 96, 24 }, buffer);
            GuiLabel((Rectangle){ 8, 88, 24, 24 }, "Y");
            sprintf(buffer, "%f", Slider002Value);
            GuiLabel((Rectangle){ 24, 88, 96, 24 }, buffer);
            GuiLabel((Rectangle){ 8, 136, 24, 24 }, "Z");
            sprintf(buffer, "%f", Slider003Value);
            GuiLabel((Rectangle){ 24, 136, 96, 24 }, buffer);

            if(Button016Pressed = GuiButton((Rectangle){ 0, 8, 120, 24 }, "Double Tap")) accelDoubleTapCallback();

            GuiLabel((Rectangle){ 144, 40, 120, 24 }, "Motor speed");
            sprintf(buffer, "%f", SliderBar017Value);
            GuiLabel((Rectangle){ 144, 64, 120, 24 }, buffer);
            GuiSliderBar((Rectangle){ 144, 88, 120, 16 }, NULL, NULL, &SliderBar017Value, -1, 1);

            GuiLabel((Rectangle){ 144, 112, 120, 24 }, "LED Brightness");
            sprintf(buffer, "%f", SliderBar015Value);
            GuiLabel((Rectangle){ 144, 136, 120, 24 }, buffer);
            GuiSliderBar((Rectangle){ 144, 160, 120, 16 }, NULL, NULL, &SliderBar015Value, -1, 1);
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

uint32_t getMillis(void)
{
    return (uint32_t)((double)clock() * 1000.0 / CLOCKS_PER_SEC);
}

void readAccel_gs(double* x, double* y, double* z)
{
    /*
    if(rand_accels)
    {
        curr_x += volatility*(((double)rand()/RAND_MAX)*2 - 1);
        curr_y += volatility*(((double)rand()/RAND_MAX)*2 - 1);
        curr_z += volatility*(((double)rand()/RAND_MAX)*2 - 1);
    }
    */
    *x = (double)Slider001Value;
    *y = (double)Slider002Value;
    *z = (double)Slider003Value;
}

void setMotorSpeed(double speed)
{
    SliderBar017Value = (float)speed;
}

void setLED(double brightness)
{
    SliderBar015Value = (float)brightness;
}

void display(const char * msg)
{
    printf("%s", msg);
}