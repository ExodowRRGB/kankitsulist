#ifdef __cplusplus
extern "C" {
#endif
#include <citro2d.h>
#include <3ds.h>

extern int ShouldExitflag;
extern int ScreenSelection;


extern u32 kDown;
extern u32 held;
extern u32 white;
extern u32 red;
extern u32 blue;

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;
extern C2D_Font font;
extern C2D_TextBuf textBuf;
extern C2D_Text txt;
#define MainMenu 0
#define UiTokenNotFoundMenu 1
#define Loading -1


//ui.c
void UiInit();
void UiRenderTop();
void UiRenderBottom();
void UiCleanup();
void UiInputHandler();

//ui/MainMenu.c
void RetriveList();
void MainMenuTop();
void MainMenuBottom();
void MainMenuInput();

//ui/loading.c
void ShowLoadingScreen();

#ifdef __cplusplus
}
#endif

