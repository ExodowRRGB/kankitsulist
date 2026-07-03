#include <citro2d.h>
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "jsonparser.h"
#include "httprequester.h"
#include "paraser.h"
float y = 0;
 C3D_RenderTarget* top;
 C3D_RenderTarget* bottom;
 C2D_Font font;
 C2D_TextBuf textBuf;
 C2D_Text txt;
char *out = NULL;
char *reqretrun = NULL;
char **items = NULL;
char **filtereditems = NULL;
int count = 0;
u32 white;
u32 held;
u32 red;
u32 blue;
        u32 kDown;
int ShouldExitflag = 0;
int MenuSelection = 0;






void RetriveList(){
  getlist(&reqretrun, MenuSelection);	
  paraselistjson(reqretrun, &items, &count, "data.MediaListCollection.lists.entries");
  filtereditems = malloc(count * sizeof(char*));
  for (int i = 0 ;i < count ;i++){
    parasejson(items[i], &out, "media.title.romaji");
    filtereditems[i] = strdup(out);
  }
}


void UiInit(){
  RetriveList();
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  font = C2D_FontLoadSystem(CFG_REGION_USA);
  textBuf = C2D_TextBufNew(4096);


  top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

//色           
  white = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
  red = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
  blue = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
}




void UiInputHandler(){
 hidScanInput();
held = hidKeysHeld();
kDown = hidKeysDown();
    if (kDown & KEY_START) ShouldExitflag =1;
    if (kDown & KEY_DOWN && y < count*20 - 200) y += 20;
    if (kDown & KEY_UP  && y > 0)  y -= 20;
    if (held & KEY_L) {
      MenuSelection = 1;
      RetriveList();
    }
     if (held & KEY_R) {
      MenuSelection = 0;
      RetriveList();

    }
  }


char tmpbuffer[100][100];
void UiRenderTop(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

  // Draw on top screen
  C2D_TargetClear(top, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
  C2D_SceneBegin(top);
  // --- your draw calls here ---
	C2D_TextBufClear(textBuf);
		

if (MenuSelection == 1){
  C2D_TextFontParse(&txt, font, textBuf, "Watching:");
} else  {
  C2D_TextFontParse(&txt, font, textBuf, "Completed:笑");
}
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 0,  0  , 0, 1.0f,1.0f , red);


int k = 0;
for(int i = 20; i < 40;i++){
int linestokaku = textchoper(filtereditems[i], 25, tmpbuffer);
C2D_DrawRectangle(10, 30 + k * 20 -y , 0, 330,linestokaku  * 20,
                   blue, blue,
                   red, red);
for(int j = 0;j < linestokaku;j++){
C2D_TextFontParse(&txt, font, textBuf, tmpbuffer[j]);
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 10,  30 + (k* 20) - y , 0, 0.8f,0.8f , white);
k++;
}
k ++;
}


/*int k = 0;
for (int i = 0 ;i < 10 ;i++){
C2D_DrawRectangle(10, 30 + i * 60, 0, 300, 30,
                   blue, blue,
                   red, red);
for(int j = 0;j < textchoper(filtereditems[i], 27, tmpbuffer);j++){
C2D_TextFontParse(&txt, font, textBuf, tmpbuffer[j]);
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 10,  30 + (k* 20) - y , 0, 0.8f,0.8f , white);
k++;
}
k++;
}*/
	/*for ( i = y/20 ;i < y/20 + 10 ;i++){
    C2D_TextFontParse(&txt, font, textBuf, filtereditems[i]);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 0,  i * 20 - y + 30  , 0, 0.8f,0.8f , white);
  }*/

}


void UiRenderBottom(){
  C2D_TargetClear(bottom, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
  C2D_SceneBegin(bottom);
//此処から

//此処まで
  C3D_FrameEnd(0);
    
}




void UiCleanup(){
   
}

