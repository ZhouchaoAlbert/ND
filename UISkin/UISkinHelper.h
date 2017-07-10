#ifndef __UISKINHELPER_H__
#define __UISKINHELPER_H__
#include <atlstr.h>
using namespace ATL;
#include <vector>
using namespace std;
UINT64 GetDiskSpaceFreeSize(CString strDiskPath);
void   StringSplit(CString text, CString sp, std::vector<CString>& vecSplit);
CString GetParentDirPath(CString strFilePath);
BOOL ValidDirPath(CString strDirPath);
BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst);
CString GetFileName(CString strPath);
#endif