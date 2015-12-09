#include <windows.h> // sisteminiai
#include "constants.h" // projekto



////-------
//// DIALOGA apdorojantis switchas
//

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






//----------------
/* pagrindinis _while'as_*/
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
        break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
            /*? atskiras casas maniu punktui??? ?*/
        case WM_COMMAND:
            {
            switch(LOWORD(wParam))
                {

                case IDM_FILE_EXIT:
                    {
                        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */

                    }
                break;
                case IDM_FILE_OPEN:
                    {
                    char szFileName[MAX_PATH];
                    HINSTANCE hInstance = GetModuleHandle(NULL);
                    GetModuleFileName(hInstance, szFileName, MAX_PATH);
                    MessageBox(hwnd, szFileName, "This program is:", MB_OK |
                        MB_ICONINFORMATION);
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
                /*
                case IDOK:
					EndDialog(hwnd, IDOK);
				break;
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
				break;
				*/
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
    if (!TranslateAccelerator(hwnd, hAccel, &messages))
        {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
 }

 return messages.wParam; // zinute kuria apdorosime auksciau esancioje funkcijoje
}


