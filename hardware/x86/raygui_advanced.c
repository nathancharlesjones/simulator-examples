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
#include <time.h>
#include <stdio.h>          // For printf, scanf
#include <stdint.h>         // For uint32_t
#include <stdbool.h>        // For bool, true, false
#include "application_MainMajor.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float xValue = 0.0f;
float yValue = 0.0f;
float zValue = 0.0f;
float motorSpeedSliderValue = 0.0f;
float ledBrightnessSliderValue = 0.0f;
bool randomizeChecked = false;
float volatilitySliderValue = 0.1f;

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
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
    bool doubleTapBtnPressed = false;
    bool maxAccelEditMode = false;
    int maxAccelValue = (int)getMaxAccel();
    bool periodEditMode = false;
    int periodValue = getPeriod();
    float weightingValue = (float)getEwmaCoeff();
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
        char buffer[32] = {0};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (GuiValueBox((Rectangle){ 96, 8, 80, 24 }, "Task period (ms)", &periodValue, 1, 10000, periodEditMode))
            {
                periodEditMode = !periodEditMode;
                if(!periodEditMode)
                {
                    sprintf(buffer, "p %d\n", periodValue);
                    char * temp = buffer;
                    while(*temp) charReceivedCallback(*temp++);
                }
            }
            GuiCheckBox((Rectangle){ 16, 40, 24, 24 }, "Randomize inputs?", &randomizeChecked);
            GuiLabel((Rectangle){ 64, 56, 72, 24 }, "Volatility");
            GuiSlider((Rectangle){ 16, 80, 120, 16 }, NULL, NULL, &volatilitySliderValue, 0, 1);            

            GuiLabel((Rectangle){ 24, 104, 24, 24 }, "X");
            sprintf(buffer, "%f", xValue);
            GuiLabel((Rectangle){ 40, 104, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 16, 128, 120, 16 }, NULL, NULL, &xValue, -10, 10);

            GuiLabel((Rectangle){ 24, 152, 24, 24 }, "Y");
            sprintf(buffer, "%f", yValue);
            GuiLabel((Rectangle){ 40, 152, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 16, 176, 120, 16 }, NULL, NULL, &yValue, -10, 10);

            GuiLabel((Rectangle){ 24, 200, 24, 24 }, "Z");
            sprintf(buffer, "%f", zValue);
            GuiLabel((Rectangle){ 40, 200, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 16, 224, 120, 16 }, NULL, NULL, &zValue, -10, 10);

            if (GuiValueBox((Rectangle){ 248, 8, 72, 24 }, "Max accel", &maxAccelValue, -100, 100, maxAccelEditMode))
            {
                maxAccelEditMode = !maxAccelEditMode;
                if(!maxAccelEditMode)
                {
                    sprintf(buffer, "m %d\n", maxAccelValue);
                    char * temp = buffer;
                    while(*temp) charReceivedCallback(*temp++);
                }
            }

            GuiLabel((Rectangle){ 184, 40, 120, 24 }, "EWMA weighting");
            if(GuiSliderBar((Rectangle){ 184, 64, 120, 16 }, NULL, NULL, &weightingValue, 0, 1))
            {
                sprintf(buffer, "w %lf\n", (double)weightingValue);
                char * temp = buffer;
                while(*temp) charReceivedCallback(*temp++);
            }

            GuiLabel((Rectangle){ 160, 88, 112, 24 }, "Direction for motor:");
            char * directionStrings[4] = { [X] = "X", [Y] = "Y", [Z] = "Z", [TOTAL] = "TOTAL"};
            GuiLabel((Rectangle){ 272, 88, 56, 24 }, directionStrings[getDirectionOfInterest()]);
            if((doubleTapBtnPressed = GuiButton((Rectangle){ 184, 112, 120, 24 }, "Double Tap"))) accelDoubleTapCallback();;

            GuiLabel((Rectangle){ 160, 144, 88, 24 }, "Motor speed");
            sprintf(buffer, "%f", motorSpeedSliderValue);
            GuiLabel((Rectangle){ 232, 144, 104, 24 }, buffer);
            GuiSliderBar((Rectangle){ 184, 168, 120, 16 }, NULL, NULL, &motorSpeedSliderValue, -1, 1);

            GuiLabel((Rectangle){ 160, 192, 48, 24 }, "LED");
            sprintf(buffer, "%f", ledBrightnessSliderValue);
            GuiLabel((Rectangle){ 232, 192, 104, 24 }, buffer);
            GuiSliderBar((Rectangle){ 184, 216, 120, 16 }, NULL, NULL, &ledBrightnessSliderValue, 0, 1);
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

uint32_t getMillis(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (uint32_t)((time.tv_sec*1000) + (time.tv_nsec/1000000));
}

void readAccel_gs(double* x, double* y, double* z)
{
    if(randomizeChecked)
    {
        xValue += volatilitySliderValue*(((double)rand()/RAND_MAX)*2 - 1);
        yValue += volatilitySliderValue*(((double)rand()/RAND_MAX)*2 - 1);
        zValue += volatilitySliderValue*(((double)rand()/RAND_MAX)*2 - 1);
    }
    *x = (double)xValue;
    *y = (double)yValue;
    *z = (double)zValue;
}

void setMotorSpeed(double speed)
{
    motorSpeedSliderValue = (float)speed;
}

void setLED(double brightness)
{
    ledBrightnessSliderValue = (float)brightness;
}

void display(const char * msg)
{
    printf("%s", msg);
}