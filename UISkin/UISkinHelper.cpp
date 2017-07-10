#include "StdAfx.h"
#include "UISkinHelper.h"


UINT64 GetDiskSpaceFreeSize(CString strDiskPath)
{
	UINT64 uSectorsPerCluster = 0;
	UINT64 uBytesPerSector = 0;
	UINT64 uFreeClusters = 0;
	UINT64 uTotalClusters = 0;
	UINT64 uDiskSize = 0;
	if (GetDiskFreeSpace(strDiskPath, (LPDWORD)&uSectorsPerCluster, (LPDWORD)&uBytesPerSector,
		(LPDWORD)&uFreeClusters, (LPDWORD)&uTotalClusters))
	{
		uDiskSize = uFreeClusters * uSectorsPerCluster * uBytesPerSector;
		return uDiskSize;
	}
	return 0;
}


void  StringSplit(CString text, CString sp, std::vector<CString>& vecSplit)
{
	vecSplit.clear();
	INT32 pos = text.Find(sp, 0);
	while (pos >= 0)
	{
		CString t = text.Left(pos);
		vecSplit.push_back(t);
		text = text.Right(text.GetLength() - pos - sp.GetLength());
		pos = text.Find(sp);
	}
	vecSplit.push_back(text);
}

CString GetParentDirPath(CString strFilePath)
{
	int ix = strFilePath.ReverseFind(_T('\\'));
	if (-1 != ix)
	{
		return strFilePath.Mid(0, ix);
	}

	return _T("");
}


BOOL ValidDirPath(CString strDirPath)
{
	CString strParentPath = GetParentDirPath(strDirPath);

	if (strParentPath.IsEmpty())
	{
		return FALSE;
	}

	if (!PathFileExists(strParentPath))
	{
		BOOL bResult = ValidDirPath(strParentPath);
		if (FALSE == bResult)
		{
			return FALSE;
		}
	}

	if (!PathFileExists(strDirPath))
	{
		BOOL bResult = CreateDirectory(strDirPath, NULL);
		if (FALSE == bResult)
		{
			return FALSE;
		}
	}
	return TRUE;
}


BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst)
{
	try
	{
		CW2A pszA(pszSrc);
		if (NULL == pszA)
		{
			return FALSE;
		}
		strDst = pszA;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

CString GetFileName(CString strPath)
{
	int ix = strPath.ReverseFind(_T('\\'));
	if (ix != -1)
	{
		strPath = strPath.Right(strPath.GetLength() - ix - 1);
		ix = strPath.Find(_T('.'));
		if (ix != -1)
		{
			return strPath.Mid(0, ix);
		}
		return strPath.Right(strPath.GetLength() - ix);
	}
	return _T("");
}
