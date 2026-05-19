#include <citro2d.h>
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "jsonparser.h"
#include "httprequester.h"
float x = 0,y = 0;
 C3D_RenderTarget* top;
 C3D_RenderTarget* bottom;
 C2D_Font font;
 C2D_TextBuf textBuf;
char *out = NULL;
char *reqretrun = NULL;
char **items = NULL;
char **filtereditems = NULL;
int count = 0;
u32 white;
u32 held;
u32 red;
        u32 kDown;


int getstringsize(char *string){
int size;
for(int i = 0; string[i] != '\0';i++) size++;
return size;
}






void Slicer(char **  , char **){


}



void UiInit(){
getlist(&reqretrun);	
paraselistjson(reqretrun, &items, &count, "data.MediaListCollection.lists.entries");
 filtereditems = malloc(count * sizeof(char*));
for (int i = 0 ;i < count ;i++){
parasejson(items[i], &out, "media.title.romaji");
filtereditems[i] = strdup(out);
}


gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
     font = C2D_FontLoadSystem(CFG_REGION_USA);
 textBuf = C2D_TextBufNew(4096);
    C2D_Prepare();
 top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	
 white = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
  red = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);



 

}




void UiInputHandler(){
 hidScanInput();
held = hidKeysHeld();
kDown = hidKeysDown();

    if (kDown & KEY_DOWN && y < count*20 - 200) y += 20;
    if (kDown & KEY_UP  && y > 0)  y -= 20;
    if (held & KEY_RIGHT) x += 10;
    if (held & KEY_LEFT)  x -= 10;

  }



void UiRenderTop(){
     
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Draw on top screen
        C2D_TargetClear(top, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
        C2D_SceneBegin(top);
        // --- your draw calls here ---
		C2D_TextBufClear(textBuf);
		C2D_Text txt;


    C2D_TextFontParse(&txt, font, textBuf, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 0,  0  , 0, 1.0f,1.0f , red);


		for (int i = y/20 ;i < y/20 + 10 ;i++){
    

C2D_TextFontParse(&txt, font, textBuf, filtereditems[i]);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 0,  i * 20 - y + 30  , 0, 0.8f,0.8f , white);

/*parasejson(items[i], &out, "progress");
C2D_TextFontParse(&txt, font, textBuf, out);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 250 - x,  i * 20 + y, 0, 1.0f,1.0f , white);
parasejson(items[i], &out, "media.episodes");
C2D_TextFontParse(&txt, font, textBuf, out);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 280 -x,  i * 20 + y, 0, 1.0f,1.0f , white);*/


}

}


void UiRenderBottom(){
   
    
        // Draw on bottom screen
        C2D_TargetClear(bottom, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
        C2D_SceneBegin(bottom);
        // --- your draw calls here ---
		
C2D_DrawRectSolid(20 + x, 40 - y, 0, 50, 10, white);
        C3D_FrameEnd(0);
    
}




void UiCleanup(){
   
}