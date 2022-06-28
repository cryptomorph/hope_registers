#include "framework.h"
#include "globals.h"

WCHAR *_rooms_list[] = {L"--- NONE --- ",
						L"FML 200",
						L"FML 201",
						L"FML 209",
						L"FML 211",
						L"FML 212",
						L"FML 215",
						L"FML 217",
						L"FML 300",
						L"FML 301",
						L"FML 309",
						L"FML 311A",     
						L"FML 400",
						L"FML 401",
						L"FML 408",
						L"FML 409",
						L"FML 411",
						L"FML 415",
						L"FML 420" };

WCHAR *_times_list[] = {L"--- NONE ---",
						L"09.00 am",
						L"10.00 am",
						L"11.00 am",
						L"12.00 pm",
						L"13.00 pm",
						L"14.00 pm",
						L"15.00 pm",
						L"16.00 pm",
						L"17.00 pm",
						L"18.00 pm" };

int _selected_room = 0;
int _rooms_list_size = 19;

int _selected_time = 0;
int _times_list_size = 11;

// Application state, use this to control menu states
SCARD_READER_STATE _app_state = NOT_CONFIGURED;

// Initialise all the HANDLES we need around the application
HINSTANCE hInst = 0;
HWND hMainWindow = 0;
HWND hEditWindow = 0;

// Initialise all the SCARD handles also
SCARDCONTEXT hScardContext = 0;     // Card context
SCARDHANDLE  hCard = 0;				// Card context handle
DWORD dwActProtocol = 0;

void clearEditText() {
	SetWindowText(hEditWindow, L"");
}

void appendEditText(WCHAR* newText) {
	int index = GetWindowTextLength(hEditWindow);
	//SetFocus(hEditWindow);											// set focus
	SendMessage(hEditWindow, EM_SETSEL, (WPARAM)index, (LPARAM)index);  // set selection - end of text
	SendMessage(hEditWindow, EM_REPLACESEL, 0, (LPARAM) newText);		// append!
}



