#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonparser.h"
#include "httprequester.h"



int main() {
	char *reqretrun;
	char **items = NULL;
	int count = 0;


	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	getlist(&reqretrun);
	char *out = NULL;
	//printf("%s\n", reqretrun);

	//parasejson(reqretrun, &out, "data.MediaListCollection.lists");
//printf("%s\n", out);

printf("Watching LIST\n");
paraselistjson(reqretrun, &items, &count, "data.MediaListCollection.lists.entries");
printf("Total = %d\n", count);
//printf("%s\n", items[0]);
for (int i = 0;i < count;i++){
parasejson(items[i], &out, "media.title.romaji");
printf("%s\n", out);

}




/*	for (int i = 0; i < count; ++i) {
    printf("[%d] %s\n", i, items[i]);
    free(items[i]);
}*/
	//printf("%s\n", out);
	
	//printf("%s\n", reqretrun);
	printf("Press START to exit.\n");
	while (aptMainLoop()) {
	hidScanInput();
	if (hidKeysDown() & KEY_START) break;
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	gfxExit();
	return 0;
}


