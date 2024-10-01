#include <time.h>			// For clock
#include <pthread.h>		// For pthread_t, pthread_create
#include <stdio.h>			// For printf, scanf
#include <string.h>			// For strtok_r
#include <stdlib.h>			// For atof, atoi
#include <stdint.h>			// For uint32_t
#include <ncurses.h>		// For 
#include <stdbool.h>		// For bool, true, false
#include "application.h"

#define NUM_VIEWS 3
#define MAX_NUM_ARGS 10
#define MAX_INPUT_CHARS 64

char view0[] =		"+------------------------+------------------------+\n\r"
					"|Max accel:              |Motor speed:            |\n\r"
					"|X:                      |LED brightness:         |\n\r"
					"|Y:                      +------------------------+\n\r"
					"|Z:                      |                         \n\r"
					"+------------------------+                         \n\r"
					"+-------------------------------------------------+\n\r"
					"|>>                                               |\n\r"
					"+-------------------------------------------------+\n\r";

char view0[] =		"+------------------------+------------------------+\n\r"
					"|Max accel:              |Motor speed:            |\n\r"
					"|X:                      |LED brightness:         |\n\r"
					"|  - Max:                +------------------------+\n\r"
					"|Z:                      |                         \n\r"
					"+------------------------+                         \n\r"
					"+-------------------------------------------------+\n\r"
					"|>>                                               |\n\r"
					"+-------------------------------------------------+\n\r";

char view1[] =		"+------------------------++------------------------+\n\r"
					"|Max accel:              ||Motor speed:            |\n\r"
					"|X:                      ||                        |\n\r"
					"|                        ||LED brightness:         |\n\r"
					"|Y:                      ||                        |\n\r"
					"|                        |+------------------------+\n\r"
					"|Z:                      |                          \n\r"
					"|                        |                          \n\r"
					"+------------------------+                          \n\r"
					"+-------------------------------------------------+ \n\r"
					"|>>                                               | \n\r"
					"+-------------------------------------------------+ \n\r";

char view2[] = 		"+------------------------+-Graph-of-X-Y-Z---------------+\n\r"
					"|Max accel:              |                              |\n\r"
					"|X:                      |                              |\n\r"
					"|                        |                              |\n\r"
					"|Y:                      |                              |\n\r"
					"|                        |                              |\n\r"
					"|Z:                      |                              |\n\r"
					"|                        |                              |\n\r"
					"+------------------------|                              |\n\r"
					"|Motor speed:            |                              |\n\r"
					"|                        |                              |\n\r"
					"|LED brightness:         |                              |\n\r"
					"|                        |                              |\n\r"
					"+------------------------+------------------------------+\n\r"
					"|>>                                                     |\n\r"
					"+-------------------------------------------------------+\n\r";

/*
char view1[] =		"┌────────────────────────┐┌────────────────────────┐\n\r"
					"│Max accel:              ││Motor speed:            │\n\r"
					"│X:                      ││                        │\n\r"
					"│                        ││LED brightness:         │\n\r"
					"│Y:                      ││                        │\n\r"
					"│                        │└────────────────────────┘\n\r"
					"│Z:                      │                          \n\r"
					"│                        │                          \n\r"
					"└────────────────────────┘                          \n\r"
					"┌──────────────────────────────────────────────────┐\n\r"
					"│>>                                                │\n\r"
					"└──────────────────────────────────────────────────┘\n\r";
*/

typedef struct view_t
{
	char * view;
	uint8_t locations[7][2];
} view_t;
									// Max       X        Y        Z       Speed   Brightness Prompt
view_t views[NUM_VIEWS] = {	{ view0, { {1, 17}, {2, 17}, {3, 17}, {4, 17}, {1, 41}, {2, 41},  {7, 4}  } },
							{ view1, { {1, 17}, {2, 17}, {4, 17}, {6, 17}, {1, 42}, {3, 42},  {10, 4} } },
							{ view2, { {1, 17}, {2, 17}, {4, 17}, {6, 17}, {9, 17}, {11, 17}, {14, 4} } } };

WINDOW * mainwin;
pthread_t h_scanfPthread;
void* scanfPthread(void* data);
double curr_x = 1.0, curr_y = 2.0, curr_z = 3.0;
uint8_t curr_view = 0;
bool rand_accels = false;
float volatility = 0.1;

void showDouble(double val, int y, int x)
{
	int old_y, old_x;
	getyx(mainwin, old_y, old_x);
	mvprintw(y, x, "%lf", val);
	move(old_y, old_x);
	refresh();
}

void showView(uint8_t view_num)
{
	clear();
	printw("%s", views[view_num].view);
	showDouble(curr_x, views[view_num].locations[1][0], views[view_num].locations[1][1]);
	showDouble(curr_y, views[view_num].locations[2][0], views[view_num].locations[2][1]);
	showDouble(curr_z, views[view_num].locations[3][0], views[view_num].locations[3][1]);
	move(views[view_num].locations[6][0], views[view_num].locations[6][1]);
	refresh();
}

void initHardware(int argc, char ** argv)
{
	mainwin = initscr();
	showView(curr_view);
	pthread_create(&h_scanfPthread, NULL, scanfPthread, NULL);
}

void tokenize(char * buffer, uint8_t * argc, char ** argv, uint8_t sizeof_argv)
{
	char *rest;
    char *token = strtok_r(buffer, " \n", &rest);
    while (token != NULL && *argc <= sizeof_argv) {
      argv[(*argc)++] = token;
      token = strtok_r(NULL, " \n", &rest);
    }
}

void drawBarGraph(uint8_t y, uint8_t x, uint8_t width, double min, double max, double val)
{
	char * full_bar = "█";
	char * half_bar = "▌";
	char * bars[] = { "█", "▌"};
	double increment = (max - min) / width;
	double bar_val = min;
	while((val - increment) > bar_val)
	{
		move(y, x);
		printf("█");
		refresh();
		x++;
		//mvprintw(x++, y, "%s", (char*)bars[0]);
		bar_val += increment;
	}
	if((bar_val - val) > 0.4*increment)
	{
		move(y, x);
		printf("▌");
		refresh();
		//mvprintw(x++, y, "%s", (char*)bars[1]);
	}
	fflush(stdout);
	refresh();
}

void* scanfPthread(void* data)
{
	uint8_t argc = 0;
	char *argv[MAX_NUM_ARGS] = {0};

	while(1)
	{
		uint8_t idx = 0;
		char buffer[MAX_INPUT_CHARS+1] = {0};

		while((buffer[idx++] = getch()) != '\n' && idx < MAX_INPUT_CHARS);

		switch(buffer[0])
		{
	    	case 'x':
	    		tokenize(buffer, &argc, argv, MAX_NUM_ARGS);
	    		curr_x = atof(argv[1]);
	    		showDouble(curr_x, views[curr_view].locations[1][0], views[curr_view].locations[1][1]);
	    		if(curr_view > 0) drawBarGraph(views[curr_view].locations[1][0]+1, 1, 25, 0, 10, curr_x);
	    		break;
	    	case 'y':
	    		tokenize(buffer, &argc, argv, MAX_NUM_ARGS);
	    		curr_y = atof(argv[1]);
	    		showDouble(curr_x, views[curr_view].locations[2][0], views[curr_view].locations[2][1]);
	    		break;
	    	case 'z':
	    		tokenize(buffer, &argc, argv, MAX_NUM_ARGS);
	    		curr_z = atof(argv[1]);
	    		showDouble(curr_x, views[curr_view].locations[3][0], views[curr_view].locations[3][1]);
	    		break;
	    	case 't':
	    		tokenize(buffer, &argc, argv, MAX_NUM_ARGS);
	    		accelDoubleTapCallback();
	    		break;
	    	case 'v':
	    		tokenize(buffer, &argc, argv, MAX_NUM_ARGS);
	    		curr_view = atoi(argv[1]);
	    		showView(curr_view);
	    		break;
	    	case 'r':
	    		rand_accels = !rand_accels;
	    		break;
			default:								// Message meant for application code
				idx = 0;
				while(buffer[idx]) charReceivedCallback(buffer[idx++]);
				break;
		}

	    // Reset string and clear stdin buffer
	    //char c;
	    //while ((c = getch()) != '\n' && c != EOF);
	    mvprintw(views[curr_view].locations[6][0], views[curr_view].locations[6][1], "                                              ");
	    argc = 0;
	    move(views[curr_view].locations[6][0], views[curr_view].locations[6][1]);
	    refresh();
	}
}

uint32_t getMillis(void)
{
	return (uint32_t)((double)clock() * 1000.0 / CLOCKS_PER_SEC);
}

double getRandFromNegOneToOne(void)
{
	return (((double)rand()/RAND_MAX)*2)-1;
}

void readAccel_gs(double* x, double* y, double* z)
{
	if(rand_accels)
	{
		curr_x = curr_x + getRandFromNegOneToOne()*curr_x*volatility;
		showDouble(curr_x, views[curr_view].locations[1][0], views[curr_view].locations[1][1]);
		curr_y = curr_y + getRandFromNegOneToOne()*curr_y*volatility;
		showDouble(curr_y, views[curr_view].locations[2][0], views[curr_view].locations[2][1]);
		curr_z = curr_z + getRandFromNegOneToOne()*curr_z*volatility;
		showDouble(curr_z, views[curr_view].locations[3][0], views[curr_view].locations[3][1]);
	}
	*x = curr_x;
	*y = curr_y;
	*z = curr_z;
}

void setMotorSpeed(double speed)
{
	showDouble(speed, views[curr_view].locations[4][0], views[curr_view].locations[4][1]);
}

void setLED(double brightness)
{
	showDouble(brightness, views[curr_view].locations[5][0], views[curr_view].locations[5][1]);
}

void display(const char * msg)
{
	//printf("%s", msg);
}