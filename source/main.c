#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reqretrun;

void getlist(){
	httpcInit(0);
	printf("httpc initialized!\n");
	httpcContext context;
	Result ret = httpcOpenContext(&context, HTTPC_METHOD_POST, "https://graphql.anilist.co/", 1);
	if (R_FAILED(ret)) {
		printf("Failed to open context: 0x%08lX\n", ret);
	} else {
		printf("Context opened!\n");
		
	}
	httpcAddRequestHeaderField(&context, "Content-Type", "application/json");
	httpcAddRequestHeaderField(&context, "Accept", "application/json");
	httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
	const char *body = "{\"query\": \"query ($id: Int) {Media (id: $id, type: ANIME) {id title {romaji english native}}}", "variables": {"id": 5081}\"}";
	httpcAddPostDataRaw(&context, (u32 *)body, strlen(body));
	ret = httpcBeginRequest(&context);
	if (R_FAILED(ret)) {
		printf("Request failed: 0x%08lX\n", ret);
	httpcCloseContext(&context);
	}
	u32 statusCode = 0;
	httpcGetResponseStatusCode(&context, &statusCode);
	printf("Status: %lu\n", statusCode);
	u8 *buffer = malloc(0x100000);
	memset(buffer, 0, 0x100000);
	u32 readSize = 0;
	Result dlret = HTTPC_RESULTCODE_DOWNLOADPENDING;
	while (dlret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING) {
    	dlret = httpcDownloadData(&context, buffer + readSize, 0x100000 - readSize, &readSize);
	}
	reqretrun = (char *)buffer;
	free(buffer);
	httpcCloseContext(&context);
	httpcExit();
}


int main() {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	getlist();
	printf("%s\n", reqretrun);
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