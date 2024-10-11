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
    bool enterBtnPressed = false;
    bool periodEditMode = false;
    int periodValue = getPeriod();
    float weightingValue = (float)getEwmaCoeff();
    float oldWeightingValue = weightingValue;
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
        if(enterBtnPressed)
        {
            sprintf(buffer, "p %d\n", periodValue);
            char * temp = buffer;
            while(*temp) charReceivedCallback(*temp++);

            sprintf(buffer, "m %d\n", maxAccelValue);
            temp = buffer;
            while(*temp) charReceivedCallback(*temp++);
        }
        if(oldWeightingValue != weightingValue)
        {
            sprintf(buffer, "w %lf\n", (double)weightingValue);
            char * temp = buffer;
            while(*temp) charReceivedCallback(*temp++);
            oldWeightingValue = weightingValue;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (GuiValueBox((Rectangle){ 96, 8, 80, 24 }, "Task period (ms)", &periodValue, 1, 10000, periodEditMode)) periodEditMode = !periodEditMode;
            GuiCheckBox((Rectangle){ 32, 40, 24, 24 }, "Randomize inputs?", &randomizeChecked);
            GuiLabel((Rectangle){ 80, 56, 72, 24 }, "Volatility");
            GuiSlider((Rectangle){ 32, 80, 120, 16 }, NULL, NULL, &volatilitySliderValue, 0, 1);            

            GuiLabel((Rectangle){ 40, 104, 24, 24 }, "X");
            sprintf(buffer, "%f", xValue);
            GuiLabel((Rectangle){ 56, 104, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 32, 128, 120, 16 }, NULL, NULL, &xValue, -10, 10);

            GuiLabel((Rectangle){ 40, 152, 24, 24 }, "Y");
            sprintf(buffer, "%f", yValue);
            GuiLabel((Rectangle){ 56, 152, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 32, 176, 120, 16 }, NULL, NULL, &yValue, -10, 10);

            GuiLabel((Rectangle){ 40, 200, 24, 24 }, "Z");
            sprintf(buffer, "%f", zValue);
            GuiLabel((Rectangle){ 56, 200, 96, 24 }, buffer);
            GuiSlider((Rectangle){ 32, 224, 120, 16 }, NULL, NULL, &zValue, -10, 10);

            if (GuiValueBox((Rectangle){ 248, 8, 72, 24 }, "Max accel", &maxAccelValue, -100, 100, maxAccelEditMode)) maxAccelEditMode = !maxAccelEditMode;
            enterBtnPressed = GuiButton((Rectangle){ 328, 8, 48, 24 }, "Enter"); 

            GuiLabel((Rectangle){ 224, 40, 120, 24 }, "EWMA weighting");
            GuiSlider((Rectangle){ 224, 64, 120, 16 }, NULL, NULL, &weightingValue, 0, 1);

            GuiLabel((Rectangle){ 200, 88, 112, 24 }, "Direction for motor:");
            char * directionStrings[4] = { [X] = "X", [Y] = "Y", [Z] = "Z", [TOTAL] = "TOTAL"};
            GuiLabel((Rectangle){ 312, 88, 56, 24 }, directionStrings[getDirectionOfInterest()]);
            if((doubleTapBtnPressed = GuiButton((Rectangle){ 224, 112, 120, 24 }, "Double Tap"))) accelDoubleTapCallback();;

            GuiLabel((Rectangle){ 200, 144, 88, 24 }, "Motor speed");
            sprintf(buffer, "%f", motorSpeedSliderValue);
            GuiLabel((Rectangle){ 272, 144, 104, 24 }, buffer);
            GuiSliderBar((Rectangle){ 224, 168, 120, 16 }, NULL, NULL, &motorSpeedSliderValue, -1, 1);

            GuiLabel((Rectangle){ 200, 192, 48, 24 }, "LED");
            sprintf(buffer, "%f", ledBrightnessSliderValue);
            GuiLabel((Rectangle){ 272, 192, 104, 24 }, buffer);
            GuiSliderBar((Rectangle){ 224, 216, 120, 16 }, NULL, NULL, &ledBrightnessSliderValue, 0, 1);
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