#include <Windows.h>
#include <shellapi.h>

int WINAPI wWinMain(
  HINSTANCE   hInstance,
  HINSTANCE   hPrevInstance,
  PWSTR       lpCmdLine,
  int         nCmdShow
)
{
  ShellExecuteA(NULL, NULL, "Game.exe", "-fl", "Bin", 1);

  return 0;
}
