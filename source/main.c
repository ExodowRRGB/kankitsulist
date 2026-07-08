#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "httprequester.h"
#include <citro2d.h>
#include "ui.h"
#include "storage.h"

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
  UiCleanup();
  return 0;

}


