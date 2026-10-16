#include <citro2d.h>
#include <3ds.h>
#include <stdio.h>
#include "ui.h"




void ShowLoadingScreen(){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

  // Draw on top screen
  C2D_TargetClear(top, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
  C2D_SceneBegin(top);
    C2D_TextBufClear(textBuf);
    C2D_TextFontParse(&txt, font, textBuf, "Loading Lol");
    C2D_TextOptimize(&txt);
    C2D_DrawText(&txt, C2D_WithColor, 0,  0  , 0, 1.0f,1.0f , red);
     C3D_FrameEnd(0);
}

