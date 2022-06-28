#include "defs.h"
#include "globals.h"
#include "ct_interface.h" 

/*
 *  _ct_get_context
 * 
 *  Establishes a win32 resource manager (HANDLE) for a card reader
 *
 *  ... any card reader - this is a windows context at this point.
 * 
 */
LONG _ct_get_context(SCARDCONTEXT *context) {

    // To establish the resource manager context and assign it to “hContext”
    LONG retCode = SCardEstablishContext(SCARD_SCOPE_USER,
                                        NULL,
                                        NULL,
                                           context);

    return retCode;
}

/*
 *  _ct_get_reader_list
 *
 *  Establishes that a card reader is attached to the PC
 * 
 *  In principal could terminate the application with an error if the
 *  attached card reader is not a ACR1252U (is it worth it?).
 * 
 */
LONG _ct_get_reader_list(SCARDCONTEXT hContext, WCHAR **reader_list) {

    DWORD buf_size = 0;

    // Dummy call to get the required buffer size
    LONG retCode = SCardListReaders(hContext, NULL, NULL, &buf_size);
    if (retCode != SCARD_S_SUCCESS) {
        goto EXIT_ERROR;
    }

    // List the available reader which can be used in the system
    *reader_list = malloc(buf_size);
    if (NULL != *reader_list) {
        retCode = SCardListReaders(hContext, NULL, *reader_list, &buf_size);
    }

EXIT_ERROR:
    return retCode;

}


/*
 *  _ct_scard_connect
 *
 *  Opens a connection to a NFC card
 * 
 *  I think it's absolutely safe to call this multiple times on the same card
 *  Additionally, I think it's safe to remove the card without explicily calling the 
 *  SCardDisconnect() function.
 * 
 */
LONG _ct_scard_connect(SCARDCONTEXT hContext, SCARDHANDLE *hCard, DWORD *dwActProtocol) {

    LONG retCode = SCardConnect(hContext,
        L"ACS ACR1252 1S CL Reader PICC 0",
        SCARD_SHARE_SHARED,
        SCARD_PROTOCOL_T1,
        hCard,
        dwActProtocol);

    return retCode;
}


/*
 *  _ct_get_scard_uid
 *
 *  Reads the Miwave card ID from the card.
 *
 *  This is a unique 4 byte ID that idenfifies
 *  the card and (lookup?) the staff/student details.
 *
 */
LONG _ct_get_scard_uid(SCARDHANDLE hCard, BYTE *rcv_buffer, DWORD *rcv_len) {

    BYTE snd_buffer[128];
    DWORD snd_len = 5;

    snd_buffer[0] = 0xFF;
    snd_buffer[1] = 0xCA;
    snd_buffer[2] = 0x00;
    snd_buffer[3] = 0x00;
    snd_buffer[4] = 0x00;

    LONG retCode = SCardTransmit(hCard, NULL, snd_buffer, snd_len, NULL, rcv_buffer, rcv_len);

    return retCode;
}


/*
 *  _ct_authenticate_sid_block
 *
 *  Authenticates at the SID block (block 14) which is in Sector 5 of the Mifare Classic memory.
 *  This must be done (for each card) before an attempt to read data is done.
 * 
 *  Note: The transmit function always returns SCARD_S_SUCCESS, we know the function worked if the
 *  rcv_buffer contains the single byte 0x90 (success) else the authenticate failed
 * 
 */
LONG _ct_authenticate_sid_block(SCARDHANDLE hCard, BYTE* rcv_buffer, DWORD* rcv_len) {
    LONG err = SCARD_S_SUCCESS;

    BYTE snd_buffer[128];
    DWORD snd_len = 10;

    snd_buffer[0] = 0xFF;
    snd_buffer[1] = 0x86;
    snd_buffer[2] = 0x00;
    snd_buffer[3] = 0x00;
    snd_buffer[4] = 0x05;
    snd_buffer[5] = 0x01;
    snd_buffer[6] = 0x00;
    snd_buffer[7] = 0x14;
    snd_buffer[8] = 0x60;
    snd_buffer[9] = 0x00;

    LONG retCode = SCardTransmit(hCard, NULL, snd_buffer, snd_len, NULL, rcv_buffer, rcv_len);

    return err;
}

//To authenticate the Block 04h with a{ TYPE A, key number 00h }. PC / SC V2.07
//APDU = { FF 86 00 00 05 01 00 04 60 00h

/*
 *  _ct_read_sid_block
 * 
 *  Reads from the card file system at Sector 5 (specifically block 0x14)
 * 
 *  This block holds a 10 byte array of the form: 0x00 0x00 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX 0xXX
 * 
 *  The last 8 bytes are a student ID. In staff cards these bytes are all 0x00
 *  These bytes are unencrypted.
 * 
 *  Note: transmit function always returns SCARD_S_SUCCESS. We know the function did work if
 *  rcv_len is 16 (0x10) and the buffer contains a sensible SID. The cail state is a single byte
 *  in the rcv_buffer 0x63 with a rcv_len of 2.
 * 
 */
LONG _ct_read_sid_block(SCARDHANDLE hCard, BYTE *rcv_buffer, DWORD *rcv_len) {
    LONG err = SCARD_S_SUCCESS;

    BYTE snd_buffer[128];
    DWORD snd_len = 5;

    // APDU to retrieve 10 bytes from block 0x14
    snd_buffer[0] = 0xFF;
    snd_buffer[1] = 0xB0;
    snd_buffer[2] = 0x00;
    snd_buffer[3] = 0x14;
    snd_buffer[4] = 0x10;

    LONG retCode = SCardTransmit(hCard, NULL, snd_buffer, snd_len, NULL, rcv_buffer, rcv_len);

    return err;
}


LONG _ct_scard_disconnect(SCARDHANDLE hCard) {
    LONG retCode = SCardDisconnect(hCard, SCARD_RESET_CARD);
    return retCode;
}

void _ct_poll() {

    LONG err = SCARD_S_SUCCESS;

    Sleep(1000);

    err = _ct_scard_connect(hScardContext, &hCard, &dwActProtocol);
    if (err != SCARD_S_SUCCESS) {
        SendMessage(hMainWindow, WM_BAD_POLL, (WPARAM)NULL, (LPARAM)NULL);
        return;
    }
    // We have a card in range - try and read data
    BYTE rcv_buffer[128];
    memset(rcv_buffer, 0x00, 128);
    DWORD rcv_len = 128;

    err = _ct_get_scard_uid(hCard, rcv_buffer, &rcv_len);
    if (err != SCARD_S_SUCCESS) {
        SendMessage(hMainWindow, WM_BAD_POLL, (WPARAM)NULL, (LPARAM)NULL);
        return;
    }
    memset(rcv_buffer, 0x00, 128);
    rcv_len = 128;
        
    err = _ct_authenticate_sid_block(hCard, rcv_buffer, &rcv_len);
    if (err != SCARD_S_SUCCESS) {
        SendMessage(hMainWindow, WM_BAD_POLL, (WPARAM)NULL, (LPARAM)NULL);
        return;
    }
    memset(rcv_buffer, 0x00, 128);
    rcv_len = 128;
        
    err = _ct_read_sid_block(hCard, rcv_buffer, &rcv_len);

    if (err == SCARD_S_SUCCESS && rcv_len > 1) {
        // We read (something like) a student ID ... send it to the main window loop
        BYTE* sid = malloc(16);
        if (NULL != sid) {
            memcpy(sid, rcv_buffer, 16);
            SendMessage(hMainWindow, WM_NEW_SID, (WPARAM)NULL, (LPARAM)sid);
        }
    }

    Sleep(1000);
    
}



