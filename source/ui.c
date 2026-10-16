#include <citro2d.h>
#include <3ds.h>
#include <string.h>
#include "ui.h"
#include "httprequester.h"



C3D_RenderTarget* top;
C3D_RenderTarget* bottom;
C2D_Font font;
C2D_TextBuf textBuf;
C2D_Text txt;
u32 red, blue, white;
u32 kDown, held;

int ShouldExitflag = 0;
int ScreenSelection = MainMenu;


void MainMenuBottom();
void MainMenuTop();
void UiTokenNotFoundTop();





void UiInit(){
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  font = C2D_FontLoadSystem(CFG_REGION_USA);
  textBuf = C2D_TextBufNew(4096);
  top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);         
  white = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
  red = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
  blue = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
  RetriveList();
}




void UiInputHandler(){
  if (kDown & KEY_START) ShouldExitflag =1;
 hidScanInput();
held = hidKeysHeld();
kDown = hidKeysDown();
  switch (ScreenSelection) {
    case MainMenu:
      MainMenuInput();
      break;
  }
}



void UiRenderTop(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

  // Draw on top screen
  C2D_TargetClear(top, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
  C2D_SceneBegin(top);
  // --- your draw calls here ---
  switch (ScreenSelection) {
    case MainMenu:
      MainMenuTop();
      break;
    case UiTokenNotFoundMenu:
      UiTokenNotFoundTop();
      break;
  }
  
}


void UiRenderBottom(){
  C2D_TargetClear(bottom, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
  C2D_SceneBegin(bottom);
//此処から
switch (ScreenSelection) {
    case MainMenu:
      MainMenuBottom();
      break;
    case UiTokenNotFoundMenu:
      
      break;
  }
//此処まで
  C3D_FrameEnd(0);
    
}



void UiCleanup(){
       C2D_Fini();
    C3D_Fini();
    gfxExit();
}


void UiTokenNotFoundTop() {
 
C2D_TextBufClear(textBuf);
C2D_TextFontParse(&txt, font, textBuf, "Not Found za token lol:");
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 0,  0  , 0, 1.0f,1.0f , red);

  

}




