#include <windows.h> // sisteminiai
#include <tchar.h>
#include "constants.h" // projekto


////----
//Glogal variables
HWND g_hToolbar = NULL; // toolbar
int x = 0;
int y = 0;

////--
//functions

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;

			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if(pszFileText != NULL)
			{
				DWORD dwRead;

				if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator
					if(SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;

		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if(dwTextLength > 0)
		{
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
			if(pszText != NULL)
			{
				if(GetWindowText(hEdit, pszText, dwBufferSize))
				{
					DWORD dwWritten;

					if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

void DoFileOpen(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if(GetOpenFileName(&ofn))
	{
		//HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		//LoadTextFileToEdit(hEdit, szFileName);
	}
}

void DoFileSave(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
	{
		//HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		//SaveTextFileFromEdit(hEdit, szFileName);
	}
}

////---------
// DIALOGA ABOUT apdorojantis switchas
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:

		return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hwnd, IDOK);
				break;
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
				break;


			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

////------------------
//toolbar message switch
BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
        case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values

			SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
		break;
		case WM_COMMAND:

			switch(LOWORD(wParam))
			{

				case IDC_PRESS:
					MessageBox(hwnd, "Hi!", "This is a message",
						MB_OK | MB_ICONEXCLAMATION);
				break;
				case IDC_OTHER:

					MessageBox(hwnd, "Bye!", "wuuups", MB_OK | MB_ICONEXCLAMATION);
				break;

                ////////////////////////
				// buttons
				//////////////////
                case IDC_B_R 	:
                    {
                        x += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                        // siunciu pagal gluobalu, BET NIEKAS NEVEIKIA
                        SendMessage (g_hToolbar ,  IDC_EVENTAS, (WPARAM)0,(LPARAM)0);
                    }
                break;
                // KODEL NEVEIKIA !??!?!!??!?!!
                case IDC_EVENTAS:
                    {
                       MessageBox(hwnd, "issikvieciau eventa,jeeeeeee", "wuuups", MB_OK | MB_ICONEXCLAMATION);
                    }
                break;
                case IDC_B_L	:
                    {
                    x -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_TL	:
                    {
                        x -= 1;
                        y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_T	:
                    {
                        y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_TR	:
                    {
                        x += 1;
                        y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_B	:
                    {
                        y -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_BL	:
                    {
                        x -= 1;
                        y -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                break;
                case IDC_B_BR	:
                    {
                        x += 1;
                        y -= 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, y, FALSE);
                    }
                //--------------------------
                break;
                case IDC_UPDATE:
                    {

                    switch(HIWORD(wParam))
                        {
                            case IDC_EVENTAS:
                                    MessageBox(hwnd, "Bye!", "Tsssssa message",
                                    MB_OK | MB_ICONEXCLAMATION);
                            break;
                        }
                    }
                break;

			}


		break;
		default:
			return FALSE;
	}

	return TRUE;
}




//----------------
/* pagrindinis switch'as_*/
//-----------------
long __stdcall WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        /* right click'inant issoka langas*/
        case WM_RBUTTONDOWN:
        {
            char szFileName[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);
            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(hwnd, szFileName, "This program is:", MB_YESNO | MB_ICONINFORMATION );
        }

        // programos pabaiga
        break;
        case WM_DESTROY:
            DestroyWindow(g_hToolbar); // destroy toolbar
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;

        //--------
        // Min/MAX size of window
        // !!error!! pjaunasi su toolbaru
        /* case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                lpMMI->ptMinTrackSize.x = 300; // min x
                lpMMI->ptMinTrackSize.y = 300;
                lpMMI->ptMaxTrackSize.x = 300; // max x
                lpMMI->ptMaxTrackSize.y = 300;
            }
        */
        //------------
        // on window creation

        case WM_CREATE:
			g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR),
				hwnd, ToolDlgProc);
			if(g_hToolbar != NULL)
			{
				ShowWindow(g_hToolbar, SW_SHOW);
			}
			else
			{
				MessageBox(hwnd, "CreateDialog returned NULL", "Warning!",
					MB_OK | MB_ICONINFORMATION);
			}
            break;

        /*
		case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values

			SetDlgItemInt(hwnd, IDC_EDIT_X, 5, FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT_Y, 15, FALSE);
		break;
        */

        ////------------
        // PAGRINDIS - apdorojami messagai kaip mygtuku paspaudimai
        /////
        case WM_COMMAND:
            {
            switch(LOWORD(wParam))
                {
                // show/hide toolbar
				case ID_DIALOG_SHOW:
					ShowWindow(g_hToolbar, SW_SHOW);
				break;
				case ID_DIALOG_HIDE:
					ShowWindow(g_hToolbar, SW_HIDE);
				break;
                //--------------

                //file I/O
                case ID_FILE_OPEN :
					DoFileOpen(hwnd);
				break;
				case ID_FILE_SAVEAS :
					DoFileSave(hwnd);
				break;
                //---

                case IDM_FILE_EXIT:
                    {
                        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */

                    }
                break;
                case IDM_FILE_OPEN:
                    {
                        //cia irgi kaskodel neveikia
                    SendMessage (g_hToolbar ,  IDC_EVENTAS, (WPARAM)0,(LPARAM)0);
                    /*
                    char szFileName[MAX_PATH];
                    HINSTANCE hInstance = GetModuleHandle(NULL);
                    GetModuleFileName(hInstance, szFileName, MAX_PATH);
                    MessageBox(hwnd, szFileName, "This program is:", MB_OK |
                        MB_ICONINFORMATION);
                    */
                    }
                break;

                /* dialogas ABOUT */
				case ID_HELP_ABOUT:
				{
					int ret = DialogBox(GetModuleHandle(NULL),
						MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
					if(ret == IDOK){
						MessageBox(hwnd, "Dialog exited with IDOK.", "Notice",
							MB_OK | MB_ICONINFORMATION);
					}
					else if(ret == IDCANCEL){
						MessageBox(hwnd, "Dialog exited with IDCANCEL.", "Notice",
							MB_OK | MB_ICONINFORMATION);
					}
					else if(ret == -1){
						MessageBox(hwnd, "Dialog failed!", "Error",
							MB_OK | MB_ICONINFORMATION);
					}
				}
				break;
                // END dialogas

                }
            }
        default:         /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
 return 0;
}

//--------------------------
// pagrindinio lango inicilizacija
//--------------------------
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpszArgument, int nCmdShow)
{
 char szClassName[ ] = "Zygio super aplikacija";
 MSG messages;
 WNDCLASS wc;
 HWND hwnd;

 //--------
 // acceleratoriai


 HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));

 wc.hInstance     = hInstance,                         wc.lpszClassName  = szClassName;
 wc.lpfnWndProc   = WindowProcedure,                   wc.style          = 0;
 wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION),   wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
 wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU),    wc.cbClsExtra     = 0;
 wc.cbWndExtra    = 0,                                 wc.hbrBackground  = (HBRUSH)COLOR_BACKGROUND;
 RegisterClass(&wc);
 hwnd=CreateWindow(szClassName,"Zygio super aplikacija",WS_OVERLAPPEDWINDOW,50,50,544,375,HWND_DESKTOP,NULL,hInstance,NULL);
 ShowWindow (hwnd, nCmdShow);

 while(GetMessage (&messages, NULL, 0, 0)>0)
 {
    if (!TranslateAccelerator(hwnd, hAccel, &messages) && !IsDialogMessage(g_hToolbar, &messages) )
        {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
 }

 return messages.wParam; // zinute kuria apdorosime auksciau esancioje funkcijoje
}


