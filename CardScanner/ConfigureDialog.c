#include "ConfigureDialog.h"

#include "framework.h"
#include "Resource.h"
#include "globals.h"

// Message handler for about box.
INT_PTR CALLBACK ConfigureDlgCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    HWND hClassListBox = GetDlgItem(hDlg, IDC_LIST_ROOMS);
    HWND hTimeListBox = GetDlgItem(hDlg, IDC_LIST_TIME);
    HWND hTitleBox = GetDlgItem(hDlg, IDC_EDIT1);

    switch (message)
    {
    case WM_INITDIALOG:

        // Populate the rooms list control
        // TODO do this from a dynamic resource and/or file eventually 
        for (int i = 0; i < _rooms_list_size; i++) {
            SendMessage(hClassListBox, LB_ADDSTRING, 0, (LPARAM) _rooms_list[i]);
        }

        // Populate the times list control
        // TODO do this from a dynamic resource and/or file eventually 
        for (int i = 0; i < _times_list_size; i++) {
            SendMessage(hTimeListBox, LB_ADDSTRING, 0, (LPARAM) _times_list[i]);
        }
        
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // Store the selected room into the global variable
            _selected_room = SendMessage(hClassListBox, LB_GETCURSEL, 0, 0);
            _selected_time = SendMessage(hTimeListBox, LB_GETCURSEL, 0, 0);

            // If no valid room chosen display error message box abd exit dialog
            if ( _selected_room < 1 || _selected_room > 50){
                MessageBox(0, L"You must select a valid room number", L"Warning", MB_ICONEXCLAMATION);
                EndDialog(hDlg, LOWORD(wParam));
                return;
             }
            // If no valid room chosen display error message box abd exit dialog
            if (_selected_time < 1 || _selected_time> 50) {
                MessageBox(0, L"You must select a valid session time", L"Warning", MB_ICONEXCLAMATION);
                EndDialog(hDlg, LOWORD(wParam));
                return;
            }

            WCHAR title[512];
            GetWindowText(hTitleBox, title, 512);

            // Valid configuration exists, write text then sent main window a reset command for the new state
            appendEditText(L"Starting new register session: ");
            appendEditText(title);
            appendEditText(L"\n");
            appendEditText(L"\n");
            appendEditText(L"Room: ");
            appendEditText(_rooms_list[_selected_room]);
            appendEditText(L"\n");
            appendEditText(L"Time: ");
            appendEditText(_times_list[_selected_time]);
            appendEditText(L"\n");
            appendEditText(L"\n");

            _app_state = CONFIGURED;
            SendMessage(hMainWindow, WM_STATE_CHANGE, 0, 0);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            _selected_room = 0;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
