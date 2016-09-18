#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#define SCREEN_W 	960
#define SCREEN_H 	544

#define BLACK 	RGBA8(0, 0, 0, 255)
#define LIME 	RGBA8(0, 255, 20, 255)
#define RED 	RGBA8(255, 0, 0, 255)
#define BLUE 	RGBA8(0, 0, 255, 255)
#define WHITE 	RGBA8(255, 255, 255, 255)

PSP2_MODULE_INFO(0, 0, "VitaSample");

int boundaryXLeft = 0;
int boundaryXRight = SCREEN_W;
int boundaryYUp = 0;
int boundaryYDown = SCREEN_H;

int firstMovingX = 420;
int firstMovingY = SCREEN_H / 2;

int secondMovingX = 520;
int secondMovingY = SCREEN_H / 2;

int speed = 3;

void HandleInput(int * circle_draw_true, SceCtrlData * pad);
void HandleDrawing(vita2d_pgf * pgf);

int main()
{
	vita2d_pgf * pgf;
	
	vita2d_init();
	vita2d_set_clear_color(WHITE);
	
	pgf = vita2d_load_default_pgf();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	
	SceCtrlData pad;
	
	memset(&pad, 0, sizeof(pad));
	
	int circle_draw_true = 0;
	
	while(1)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();
		sceCtrlPeekBufferPositive(0, &pad, 1);	
		
		if(circle_draw_true)
		{
			HandleInput(&circle_draw_true, &pad);
			HandleDrawing(pgf);
		}
		else
		{
			vita2d_pgf_draw_text(pgf, 10, 50, BLACK, 1.0f, "Press cross to draw and move a circle!");
			vita2d_pgf_draw_text(pgf, 10, 80, BLACK, 1.0f, "Press start to quit");
			
			if(pad.buttons & SCE_CTRL_CROSS)
			{
				circle_draw_true = 1;
			}
			
			if(pad.buttons & SCE_CTRL_START)
			{
				break;
			}
		}
		
		vita2d_end_drawing();
        vita2d_swap_buffers();// New
	}
	
	vita2d_fini();
   	vita2d_free_pgf(pgf);
    sceKernelExitProcess(0);
    
    return 0;
}

void HandleInput(int * circle_draw_true, SceCtrlData * pad)
{
	//sceCtrlPeekBufferPositive(0, pad, 1); 
					
	if ((pad->buttons & SCE_CTRL_UP) && (firstMovingY > boundaryYUp))
	{
 		firstMovingY -= speed;
	}
	else if((pad->buttons & SCE_CTRL_DOWN) && (firstMovingY < boundaryYDown) )
	{
		firstMovingY += speed;
	}	
	else if((pad->buttons & SCE_CTRL_RIGHT) && (firstMovingX < boundaryXRight))
	{
		firstMovingX += speed;
	}
	else if((pad->buttons & SCE_CTRL_LEFT) && (firstMovingX > boundaryXLeft))
	{
		firstMovingX -= speed; 
	}
		 
	if(pad->buttons & SCE_CTRL_START)
	{
		*circle_draw_true = 0;
	}
	 
	if(pad->buttons & SCE_CTRL_RTRIGGER)
	{
		++speed;
	}
				 
	if(pad->buttons & SCE_CTRL_LTRIGGER && (speed > 0))
	{
		--speed;
	}
				 
	if ((pad->buttons & SCE_CTRL_TRIANGLE) && (secondMovingY > boundaryYUp))
	{
   		secondMovingY -= speed;
  	}
   	else if ((pad->buttons & SCE_CTRL_CROSS) && (secondMovingY < boundaryYDown)){
  		secondMovingY += speed;
  	}
  	else if ((pad->buttons & SCE_CTRL_CIRCLE) && (secondMovingX < boundaryXRight)){
		secondMovingX += speed;
  	}
  	else if ((pad->buttons & SCE_CTRL_SQUARE) && (secondMovingX > boundaryXLeft)){
  		secondMovingX -= speed;
   	}
   	
}

void HandleDrawing(vita2d_pgf * pgf)
{
   	int diffX = firstMovingX - secondMovingX;
  	if(diffX < 0)
   	{
   		diffX = secondMovingX - firstMovingX; // Positive cnversion
   	}
            	
   	int diffY = firstMovingY - secondMovingY;
   	if(diffY < 0)
   	{
   		diffY = secondMovingY - firstMovingY;
   	}
   	if(diffX < 100 && diffY < 100)
   	{
   		vita2d_pgf_draw_text(pgf, 500, 100, RED, 1.0f, "Collision");
  	}
  	            	
	vita2d_pgf_draw_textf(pgf, SCREEN_W - 200, 100, LIME, 1.0f, "firstMovingX: %d", firstMovingX);
    vita2d_pgf_draw_textf(pgf, SCREEN_W - 200, 130, LIME, 1.0f, "firstMovingY: %d", firstMovingY);
    vita2d_pgf_draw_textf(pgf, SCREEN_W - 200, 160, LIME, 1.0f, "secondMovingX: %d", secondMovingX);
    vita2d_pgf_draw_textf(pgf, SCREEN_W - 200, 190, LIME, 1.0f, "secondMovingY: %d", secondMovingY);
    vita2d_pgf_draw_text(pgf, SCREEN_W - 300, SCREEN_H - 20, LIME, 1.0f, "Press start to exit");
         	
    vita2d_draw_fill_circle(firstMovingX, firstMovingY, 50, BLUE);
    vita2d_draw_fill_circle(secondMovingX, secondMovingY, 50, LIME);
}







