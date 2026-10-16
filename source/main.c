#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "httprequester.h"
#include <citro2d.h>
#include "ui.h"
#include "storage.h"

int main() {
    freopen("sdmc:/kankitsu_log.txt", "w", stdout);
    freopen("sdmc:/kankitsu_log.txt", "w", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("Started ig hahahahah\n");


  romfsInit();
	initNetwork();
  GetToken();
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


