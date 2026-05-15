#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonparser.h"
#include "httprequester.h"
#include <citro2d.h>


int main() {
	char *reqretrun;
	char **items = NULL;
	int count = 0;


	//gfxInitDefault();
	//consoleInit(GFX_TOP, NULL);
	getlist(&reqretrun);
	char *out = NULL;
	//printf("%s\n", reqretrun);

	//parasejson(reqretrun, &out, "data.MediaListCollection.lists");
//printf("%s\n", out);

printf("Watching LIST\n");
paraselistjson(reqretrun, &items, &count, "data.MediaListCollection.lists.entries");
printf("Total = %d\n", count);
//printf("%s\n", items[0]);
/*for (int i = 0;i < count;i++){
parasejson(items[i], &out, "media.title.romaji");
printf("%s   ", out);
parasejson(items[i], &out, "progress");
printf("%s/", out);
parasejson(items[i], &out, "media.episodes");
printf("%s\n", out);


}*/




/*	for (int i = 0; i < count; ++i) {
    printf("[%d] %s\n", i, items[i]);
    free(items[i]);
}*/
	//printf("%s\n", out);
	
	//printf("%s\n", reqretrun);
/*	printf("Press START to exit.\n");
	while (aptMainLoop()) {
	hidScanInput();
	if (hidKeysDown() & KEY_START) break;
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	gfxExit();
	return 0;*/

float x = 0,y = 0;
gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
 C3D_RenderTarget* top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	C2D_Font font = C2D_FontLoadSystem(CFG_REGION_USA);
C2D_TextBuf textBuf = C2D_TextBufNew(4096);
u32 red = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
  while (aptMainLoop()) {
        hidScanInput();
		

        if (hidKeysDown() & KEY_START) break;
  u32 held = hidKeysHeld();
    if (held & KEY_UP) y += 10;
    if (held & KEY_DOWN)  y -= 10;
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Draw on top screen
        C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
        C2D_SceneBegin(top);
        // --- your draw calls here ---
		C2D_TextBufClear(textBuf);
		C2D_Text txt;
		for (int i = 0;i < count;i++){
parasejson(items[i], &out, "media.title.romaji");
C2D_TextFontParse(&txt, font, textBuf, out);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 0, 10 + i * 10 + y, 0, 0.6f,0.6f , red);

parasejson(items[i], &out, "progress");
C2D_TextFontParse(&txt, font, textBuf, out);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 250, 10 + i * 10 + y, 0, 0.6f,0.6f , red);
//printf("%s/", out);
parasejson(items[i], &out, "media.episodes");
C2D_TextFontParse(&txt, font, textBuf, out);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 280, 10 + i * 10 + y, 0, 0.6f,0.6f , red);
//printf("%s\n", out);


}
		
//C2D_DrawRectSolid(10, 10, 0, 20, 10, red);


        // Draw on bottom screen
        C2D_TargetClear(bottom, C2D_Color32(0x1A, 0x1A, 0x2E, 0xFF));
        C2D_SceneBegin(bottom);
        // --- your draw calls here ---
		
C2D_DrawRectSolid(20, 40, 0, 50, 10, red);
        C3D_FrameEnd(0);
    }

    // Cleanup
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;

}


