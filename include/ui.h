#ifdef __cplusplus
extern "C" {
#endif
extern int ShouldExitflag;
extern int MenuSelection;
#define MainMenu 0
#define UiTokenNotFoundMenu 1

void UiInit();
void UiRenderTop();
void UiRenderBottom();
void UiCleanup();
void UiInputHandler();


#ifdef __cplusplus
}
#endif

