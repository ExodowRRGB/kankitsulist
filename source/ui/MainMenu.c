#include <citro2d.h>
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "jsonparser.h"
#include "httprequester.h"
#include "paraser.h"

int Catgory = 0 ;

char *out = NULL;
char *reqretrun = NULL;
char **items = NULL;
char **filtereditems = NULL;
int count = 0;
float y = 0;
int lol = 0;

char tmpbuffer[100][100];

void RetriveList(){
  ShowLoadingScreen();
  
  getlist(&reqretrun, Catgory);	
  paraselistjson(reqretrun, &items, &count, "data.MediaListCollection.lists.entries");
  filtereditems = malloc(count * sizeof(char*));
  for (int i = 0 ;i < count ;i++){
    parasejson(items[i], &out, "media.title.romaji");
    filtereditems[i] = strdup(out);
  }
  lol = 0;
  printf("count = %d\n", count);

}

void MainMenuTop(){
C2D_TextBufClear(textBuf);
	
		

if (Catgory == 1){
  C2D_TextFontParse(&txt, font, textBuf, "Watching:");
} else  {
  C2D_TextFontParse(&txt, font, textBuf, "Completed:");
}
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 0,  0  , 0, 1.0f,1.0f , red);


int k = 0;
for(int i = 0; i < 50 && i < count ;i++){
int linestokaku = textchoper(filtereditems[i], 26, tmpbuffer);
C2D_DrawRectangle(10, 30 + k * 20 -y , 0, 330,linestokaku  * 20,
                   blue, blue,
                   red, red);
for(int j = 0;j < linestokaku;j++){
C2D_TextFontParse(&txt, font, textBuf, tmpbuffer[j]);
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 10,  30 + (k* 20) - y , 0, 0.8f,0.8f , white);
k++;
}
k ++;
}
C2D_DrawTriangle(0, 35, white, 0, 55, white, 20, 45, white, 0);



/*int k = 0;
for (int i = 0 ;i < 10 ;i++){
C2D_DrawRectangle(10, 30 + i * 60, 0, 300, 30,
                   blue, blue,
                   red, red);
for(int j = 0;j < textchoper(filtereditems[i], 27, tmpbuffer);j++){
C2D_TextFontParse(&txt, font, textBuf, tmpbuffer[j]);
C2D_TextOptimize(&txt);
C2D_DrawText(&txt, C2D_WithColor, 10,  30 + (k* 20) - y , 0, 0.8f,0.8f , white);
k++;
}
k++;
}*/
	/*for ( i = y/20 ;i < y/20 + 10 ;i++){
    C2D_TextFontParse(&txt, font, textBuf, filtereditems[i]);
		C2D_TextOptimize(&txt);
		C2D_DrawText(&txt, C2D_WithColor, 0,  i * 20 - y + 30  , 0, 0.8f,0.8f , white);
  }*/
}



void MainMenuBottom(){

}


void MainMenuInput(){
    if (held & KEY_DOWN) y += 20;
    if (held & KEY_UP  && y > 0)  y -= 20;
    if (held & KEY_L) {
      Catgory = 1;
      RetriveList();
      y = 0;
    }
     if (held & KEY_R) {
      Catgory = 0;
      RetriveList();
      y = 0;
    }
}
