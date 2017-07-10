#pragma  once
#include <windows.h>
#include <tchar.h>
#include "pluginapi.h"
#include "MsgDef.h"


void InitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void ShowPageItem(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void FindControlEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void OnBindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetControlFocusEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetButtonData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void GetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void GetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void GetDiskSpaceFreeSizeEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void CreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void GetInstallFileName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void MakeSureCreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void ShowLicense(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetPercentValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void SetProgressValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

void SelectFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

void InitMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void ShowMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void OnBindBoxControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void CloseMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

void ExitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

void TBCIACreatTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
void TBCIAKillTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);