#include <windows.h> // sisteminiai
#include <tchar.h>
#include "constants.h" // projekto


////----
//Glogal variables
HWND g_hToolbar = NULL; // toolbar
// int x = 0;
// int y = 0;

////-----------------
// animacijai kamuoliukas
//

const int ID_TIMER = 1;
const int BALL_MOVE_DELTA = 2;

typedef struct _BALLINFO
{
	int width;
	int height;
	int x;
	int y;

	int dx;
	int dy;

}BALLINFO;

BALLINFO g_ballInfo = {5,5,30,30,0,0 }; // create structure with default values

HBITMAP g_hbmBall = NULL;
HBITMAP g_hbmMask = NULL;

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

void DrawBall(HDC hdc, RECT* prc)
{
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmMask);

	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, g_hbmBall);
	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCPAINT);

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void UpdateBall(RECT* prc)
{
	g_ballInfo.x += g_ballInfo.dx;
	g_ballInfo.y += g_ballInfo.dy;

	if(g_ballInfo.x < 0)
	{
		g_ballInfo.x = 0;
		g_ballInfo.dx = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.x + g_ballInfo.width > prc->right)
	{
		g_ballInfo.x = prc->right - g_ballInfo.width;
		g_ballInfo.dx = -BALL_MOVE_DELTA;
	}

	if(g_ballInfo.y < 0)
	{
		g_ballInfo.y = 0;
		g_ballInfo.dy = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.y + g_ballInfo.height > prc->bottom)
	{
		g_ballInfo.y = prc->bottom - g_ballInfo.height;
		g_ballInfo.dy = -BALL_MOVE_DELTA;
	}
}


BOOL LoadTextFileToEdit(LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
    	{
		if(&g_ballInfo != NULL)
		{
			LPSTR pszText;
			DWORD dwBufferSize = sizeof(BALLINFO) + 1;
					DWORD dwWritten;
					if(ReadFile(hFile,&g_ballInfo, sizeof(BALLINFO), &dwWritten, NULL))
						bSuccess = TRUE;
		}
		CloseHandle(hFile);
        }
    return bSuccess;
}

BOOL SaveTextFileFromEdit( LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		if(&g_ballInfo != NULL)
		{
			LPSTR pszText;
			DWORD dwBufferSize = sizeof(BALLINFO) + 1;
					DWORD dwWritten;
					if(WriteFile(hFile, (LPCVOID)&g_ballInfo, sizeof(BALLINFO), &dwWritten, NULL))
						bSuccess = TRUE;
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
		LoadTextFileToEdit( szFileName);
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
		SaveTextFileFromEdit(szFileName);
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

			SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
			SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
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
                        g_ballInfo.x += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
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
                    g_ballInfo.x -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_TL	:
                    {
                        g_ballInfo.x -= 1;
                        g_ballInfo.y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_T	:
                    {
                        g_ballInfo.y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_TR	:
                    {
                        g_ballInfo.x += 1;
                        g_ballInfo.y += 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_B	:
                    {
                        g_ballInfo.y -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_BL	:
                    {
                        g_ballInfo.x -= 1;
                        g_ballInfo.y -= 1;
                    SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                    SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
                    }
                break;
                case IDC_B_BR	:
                    {
                        g_ballInfo.x += 1;
                        g_ballInfo.y -= 1;
                        SetDlgItemInt(hwnd, IDC_EDIT_X, g_ballInfo.x, FALSE);
                        SetDlgItemInt(hwnd, IDC_EDIT_Y, g_ballInfo.y, FALSE);
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
		{

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

            //---------
            // animation

            UINT ret;
			BITMAP bm;

			g_hbmBall = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));
			if(g_hbmBall == NULL)
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);

			g_hbmMask = CreateBitmapMask(g_hbmBall, RGB(0, 0, 0));
			if(g_hbmMask == NULL)
				MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);

			GetObject(g_hbmBall, sizeof(bm), &bm);

			//ZeroMemory(&g_ballInfo, sizeof(g_ballInfo));
			g_ballInfo.width = bm.bmWidth;
			g_ballInfo.height = bm.bmHeight;

			g_ballInfo.dx = BALL_MOVE_DELTA;
			g_ballInfo.dy = BALL_MOVE_DELTA;

			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);

            }
            break;



		case WM_PAINT:
		{
			RECT rcClient;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rcClient);
			DrawBall(hdc, &rcClient);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_TIMER:
		{
			RECT rcClient;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rcClient);

			UpdateBall(&rcClient);
			DrawBall(hdc, &rcClient);

			ReleaseDC(hwnd, hdc);
		}


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


