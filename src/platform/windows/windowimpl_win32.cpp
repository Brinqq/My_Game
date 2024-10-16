#include "platform.h"
#include "windows.h"


#define DEFUALT_ASPECT_RATIO_X = 1920
#define DEFUALT_ASPECT_RATIO_Y = 1080

struct WindowHandles{
  HINSTANCE hInstance;
  HWND hWnd;
};

WindowHandles* pHandles;

LRESULT CALLBACK win32MsgCallback(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);

int platformStartUp(){
  const char* ClassName = "Game_Window_Class";
  pHandles = (WindowHandles*)malloc(sizeof(WindowHandles));
  WNDCLASS wc{};
  wc.style = CS_DBLCLKS;
  wc.lpfnWndProc = win32MsgCallback;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = pHandles->hInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszClassName = ClassName;

  if(!RegisterClassA(&wc)){
    return 1;
  }

  HWND result = CreateWindowEx(0, ClassName, "Game", WS_BORDER | WS_VISIBLE, 1920/2, 1080/2, 1920, 1080, 0 , 0, pHandles->hInstance, 0);
  if(result == 0){
    MessageBoxA(NULL, "Failed to create window", "ERROR", MB_ICONEXCLAMATION | MB_OK);
  }
  pHandles->hWnd = result;
  return 0;
};


LRESULT CALLBACK win32MsgCallback(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam){

}
