#include <3ds.h>
#include <stdio.h>
#include "ui.h"
char anilisttoken[1085];

void GetToken(){
    //UiTokenNotFound();
    FILE* f = fopen("sdmc:/3ds/Kankitsulist/token.txt", "r");
    if (!f) ScreenSelection = UiTokenNotFoundMenu;
    fgets(anilisttoken, sizeof(anilisttoken), f);
    fclose(f);
}