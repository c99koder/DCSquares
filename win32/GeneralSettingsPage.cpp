// GeneralSettingsPage.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "GeneralSettingsPage.h"
#include ".\generalsettingspage.h"


// CGeneralSettingsPage dialog

IMPLEMENT_DYNAMIC(CGeneralSettingsPage, CPropertyPage)
CGeneralSettingsPage::CGeneralSettingsPage()
	: CPropertyPage(CGeneralSettingsPage::IDD)
	, musicCheck(FALSE)
	, soundCheck(FALSE)
	, themeTxt(_T(""))
{
}

CGeneralSettingsPage::~CGeneralSettingsPage()
{
}

void CGeneralSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THEMELIST, themeList);
	DDX_Check(pDX, IDC_BGM, musicCheck);
	DDX_Check(pDX, IDC_SFX, soundCheck);
	DDX_CBString(pDX, IDC_THEMELIST, themeTxt);
}


BEGIN_MESSAGE_MAP(CGeneralSettingsPage, CPropertyPage)
END_MESSAGE_MAP()


// CGeneralSettingsPage message handlers

BOOL CGeneralSettingsPage::OnInitDialog(void)
{
    BOOL            fFinished;
    HANDLE          hList;
    WIN32_FIND_DATA FileData;

	CPropertyPage::OnInitDialog();
    // Get the first file
    hList = FindFirstFile("themes\\*", &FileData);
    if (hList == INVALID_HANDLE_VALUE)
    { 
        printf("No files found\n\n");
    }
    else
    {
        // Traverse through the directory structure
        fFinished = FALSE;
        while (!fFinished)
        {
            // Check the object is a directory or not
            if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((strcmp(FileData.cFileName, ".") != 0) && (strcmp(FileData.cFileName, "..") != 0))
                {
					themeList.AddString(FileData.cFileName);
                }
            }

            if (!FindNextFile(hList, &FileData))
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                {
                    fFinished = TRUE;
                }
            }
        }
    }
    FindClose(hList);
	themeList.SetCurSel(themeList.FindString(0,theApp.theme));
	return 0;
}
