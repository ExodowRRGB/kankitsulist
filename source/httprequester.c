#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <curl/curl.h>

#define SOC_BUFFERSIZE 0x100000
static u32 *socBuffer;

typedef struct {
    char  *data;
    size_t size;
} ResponseBuffer;

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realSize = size * nmemb;
    ResponseBuffer *buf = (ResponseBuffer *)userp;
    char *ptr = realloc(buf->data, buf->size + realSize + 1);
    if (!ptr) return 0;
    buf->data = ptr;
    memcpy(&(buf->data[buf->size]), contents, realSize);
    buf->size += realSize;
    buf->data[buf->size] = '\0';
    return realSize;
}

void initNetwork() {
    acInit();
    u32 status = 0;
    while (status == 0) {
        ACU_GetWifiStatus(&status);
        svcSleepThread(100000000LL);
    }
    socBuffer = (u32*)memalign(0x1000, SOC_BUFFERSIZE);
    socInit(socBuffer, SOC_BUFFERSIZE);
}

void exitNetwork() {
    socExit();
    free(socBuffer);
    acExit();
}

void getlist(char **response) {
    

    //printf("Network ready!\n");

    CURL *curl = curl_easy_init();
    if (!curl) {
       // printf("curl init failed\n");
        return;
    }

    ResponseBuffer buf = { malloc(1), 0 };
    buf.data[0] = '\0';

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer API_KEY_PLACEHOLDER");
   const char *body = "{\"query\":\"query($userId:Int){MediaListCollection(userId:$userId,type:ANIME,status:COMPLETED){lists{entries{media{title{romaji}episodes}progress}}}}\",\"variables\":{\"userId\":6309041}}";
 curl_easy_setopt(curl, CURLOPT_URL, "https://graphql.anilist.co/");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(body));
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "romfs:/cacert.pem");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
  CURLcode res = curl_easy_perform(curl);
   curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

if (res != CURLE_OK) {
    printf("curl error: %s\n", curl_easy_strerror(res));
    free(buf.data);
    *response = NULL;}else {
    printf("Status OK, got %zu bytes\n", buf.size);
    *response = buf.data;

}

    
    
}




