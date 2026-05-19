#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonparser.h"
#include "httprequester.h"
#include <citro2d.h>
#include "ui.h"

int main() {
 romfsInit();
	initNetwork();

UiInit();

while (aptMainLoop()) {
//  hidScanInput();	

//if (hidKeysDown() & KEY_START) break;

UiInputHandler();

  UiRenderTop();
		
UiRenderBottom();
  }
    // Cleanup
    exitNetwork();
    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;

}


