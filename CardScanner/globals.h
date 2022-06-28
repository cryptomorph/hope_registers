#pragma once

#include "framework.h"
#include "winscard.h"
#include "ct_api.h"

// Application state
typedef enum {
    NOT_CONFIGURED = 0,
    CONFIGURED,
    READER_CONNECTED,
    READER_POLLING,
    READER_NOT_POLLING,
    READER_DISCONNECTED,
    READER_TERMINATED
} SCARD_READER_STATE;

// Globals for the rooms list control (defined in globals.c)
extern WCHAR	*_rooms_list[];
extern int		_selected_room;
extern int		_rooms_list_size;

// Globals for the times list (also defiens in globals.c)
extern WCHAR    *_times_list[];
extern int      _selected_time;
extern int      _times_list_size;

// Current Application state (configured, reading etc - also set in globals.c)
extern SCARD_READER_STATE _app_state;

// The HANDLEs that are convenient to have hanging arond
extern HINSTANCE hInst;
extern HWND hMainWindow;
extern HWND hEditWindow;

// SCARD specific handles
extern SCARDCONTEXT hScardContext;      // Card context
extern SCARDHANDLE hCard;               // Card context handle
extern DWORD dwActProtocol;             // Protocol ID

// New custom messages
#define WM_BAD_POLL     WM_USER+1
#define WM_NEW_SID      WM_USER+2
#define WM_STATE_CHANGE WM_USER+3

// Utility methods (writes or clears the main EditText area)
void clearEditText();
void appendEditText(WCHAR* newText);

