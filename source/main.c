#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "httprequester.h"
#include <citro2d.h>
#include "ui.h"

int main() {
 romfsInit();
	initNetwork();

UiInit();

while (aptMainLoop()) {


if (ShouldExitflag == 1) break;

UiInputHandler();

  UiRenderTop();
		
UiRenderBottom();
  }
    exitNetwork();
    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;

}


