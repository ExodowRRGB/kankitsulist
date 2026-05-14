#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getlist(char* *response){
	httpcInit(0);
	//printf("httpc initialized!\n");
	httpcContext context;
	Result ret = httpcOpenContext(&context, HTTPC_METHOD_POST, "https://graphql.anilist.co/", 1);
	if (R_FAILED(ret)) {
		printf("Failed to open context: 0x%08lX\n", ret);
	} else {
		//printf("Context opened!\n");
		
	}
	httpcAddRequestHeaderField(&context, "Authorization", "Bearer API_KEY_PLACEHOLDER");
	httpcAddRequestHeaderField(&context, "Content-Type", "application/json");
	httpcAddRequestHeaderField(&context, "Accept", "application/json");
	httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
	const char *body = "{\"query\": \"query($userId:Int){MediaListCollection(userId:$userId,type:ANIME,status:CURRENT){lists{entries{media{title{romaji}episodes }progress}}}}\",\"variables\": {\"userId\": 6309041}}";
	httpcAddPostDataRaw(&context, (u32 *)body, strlen(body));
	ret = httpcBeginRequest(&context);
	if (R_FAILED(ret)) {
		printf("Request failed: 0x%08lX\n", ret);
	httpcCloseContext(&context);
	}
	u32 statusCode = 0;
	httpcGetResponseStatusCode(&context, &statusCode);
	//printf("Status: %lu\n", statusCode);
	u8 *buffer = malloc(0x100000);
	memset(buffer, 0, 0x100000);
	u32 readSize = 0;
	Result dlret = HTTPC_RESULTCODE_DOWNLOADPENDING;
	while (dlret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING) {
    	dlret = httpcDownloadData(&context, buffer + readSize, 0x100000 - readSize, &readSize);
	}
	*response = (char *)buffer;
	
	
	httpcCloseContext(&context);
	httpcExit();
}