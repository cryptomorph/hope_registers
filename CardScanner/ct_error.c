#include "ct_error.h"

WCHAR* resolve_warning_string(long err_code) {
	WCHAR* ret;
	switch (err_code)
	{
		// SCARD Warnings
		case SCARD_W_UNSUPPORTED_CARD:
			ret = L"The reader cannot communicate with the smart card, due to ATR configuration conflicts.";
			break;
		case SCARD_W_UNRESPONSIVE_CARD:
			ret = L"The smart card is not responding to a reset.";
			break;
		case SCARD_W_UNPOWERED_CARD:
			ret = L"Power has been removed from the smart card, so that further communication is not possible.";
			break;
		case SCARD_W_RESET_CARD:
			ret = L"The smart card has been reset, so any shared state information is invalid.";
			break;
		case SCARD_W_REMOVED_CARD:
			ret = L"The smart card has been removed, so that further communication is not possible.";
			break;
		case SCARD_W_SECURITY_VIOLATION:
			ret = L"Access was denied because of a security violation.";
			break;
		case SCARD_W_WRONG_CHV:
			ret = L"The card cannot be accessed because the wrong PIN was presented.";
			break;
		case SCARD_W_CHV_BLOCKED:
			ret = L"The card cannot be accessed because the maximum number of PIN entry attempts has been reached.";
			break;
		case SCARD_W_EOF:
			ret = L"The end of the smart card file has been reached.";
			break;
		case SCARD_W_CANCELLED_BY_USER:
			ret = L"The action was cancelled by the user.";
			break;
		case SCARD_W_CARD_NOT_AUTHENTICATED:
			ret = L"No PIN was presented to the smart card.";
			break;
		case SCARD_W_CACHE_ITEM_NOT_FOUND:
			ret = L"The requested item could not be found in the cache.";
			break;
		case SCARD_W_CACHE_ITEM_STALE:
			ret = L"The requested cache item is too old and was deleted from the cache.";
			break;
		case SCARD_W_CACHE_ITEM_TOO_BIG:
			ret = L"The new cache item exceeds the maximum per-item size defined for the cache.";
			break;
		default:
			ret = NULL;
	}
	return ret;
}

WCHAR *resolve_error_string(long err_code) {

	WCHAR* ret = NULL;
	switch (err_code)
	{
		// SCARD Errors
		case SCARD_F_INTERNAL_ERROR:
			ret = L"An internal consistency check failed.";
			break;
		case SCARD_E_CANCELLED:
			ret = L"The action was cancelled by an SCardCancel request.";
			break;
		case SCARD_E_INVALID_HANDLE:
			ret = L"The supplied handle was invalid.";
			break;
		case SCARD_E_INVALID_PARAMETER:
			ret = L"One or more of the supplied parameters could not be properly interpreted.";
			break;
		case SCARD_E_INVALID_TARGET:
			ret = L"Registry startup information is missing or invalid.";
			break;
		case SCARD_E_NO_MEMORY:
			ret = L"Not enough memory available to complete this command.";
			break;
		case SCARD_F_WAITED_TOO_LONG:
			ret = L"An internal consistency timer has expired.";
			break;
		case SCARD_E_INSUFFICIENT_BUFFER:
			ret = L"The data buffer to receive returned data is too small for the returned data.";
			break;
		case SCARD_E_UNKNOWN_READER:
			ret = L"The specified reader name is not recognized.";
			break;
		case SCARD_E_TIMEOUT:
			ret = L"The user-specified timeout value has expired.";
			break;
		case SCARD_E_SHARING_VIOLATION:
			ret = L"The smart card cannot be accessed because of other connections outstanding.";
			break;
		case SCARD_E_NO_SMARTCARD:
			ret = L"The operation requires a smart card, but no smart card is currently in the device.";
			break;
		case SCARD_E_UNKNOWN_CARD:
			ret = L"The specified smart card name is not recognized.";
			break;
		case SCARD_E_CANT_DISPOSE:
			ret = L"The system could not dispose of the media in the requested manner.";
			break;
		case SCARD_E_PROTO_MISMATCH:
			ret = L"The requested protocols are incompatible with the protocol currently in use with the smart card.";
			break;
		case SCARD_E_NOT_READY:
			ret = L"The reader or smart card is not ready to accept commands.";
			break;
		case SCARD_E_INVALID_VALUE:
			ret = L"One or more of the supplied parameters values could not be properly interpreted.";
			break;
		case SCARD_E_SYSTEM_CANCELLED:
			ret = L"The action was cancelled by the system, presumably to log off or shut down.";
			break;
		case SCARD_F_COMM_ERROR:
			ret = L"An internal communications error has been detected.";
			break;
		case SCARD_F_UNKNOWN_ERROR:
			ret = L"An internal error has been detected, but the source is unknown.";
			break;
		case SCARD_E_INVALID_ATR:
			ret = L"An ATR obtained from the registry is not a valid ATR string.";
			break;
		case SCARD_E_NOT_TRANSACTED:
			ret = L"An attempt was made to end a non-existent transaction.";
			break;
		case SCARD_E_READER_UNAVAILABLE:
			ret = L"The specified reader is not currently available for use.";
			break;
		case SCARD_P_SHUTDOWN:
			ret = L"The operation has been aborted to allow the server application to exit.";
			break;
		case SCARD_E_PCI_TOO_SMALL:
			ret = L"The PCI Receive buffer was too small.";
			break;
		case SCARD_E_READER_UNSUPPORTED:
			ret = L"The reader driver does not meet minimal requirements for support.";
			break;
		case SCARD_E_DUPLICATE_READER:
			ret = L"The reader driver did not produce a unique reader name.";
			break;
		case SCARD_E_CARD_UNSUPPORTED:
			ret = L"The smart card does not meet minimal requirements for support.";
			break;
		case SCARD_E_NO_SERVICE:
			ret = L" The Smart Card Resource Manager is not running.";
			break;
		case SCARD_E_SERVICE_STOPPED:
			ret = L"The Smart Card Resource Manager has shut down.";
			break;
		case SCARD_E_UNEXPECTED:
			ret = L"An unexpected card error has occurred.";
			break;
		case SCARD_E_ICC_INSTALLATION:
			ret = L"No Primary Provider can be found for the smart card.";
			break;
		case SCARD_E_ICC_CREATEORDER:
			ret = L"The requested order of object creation is not supported.";
			break;
		case SCARD_E_UNSUPPORTED_FEATURE:
			ret = L"This smart card does not support the requested feature.";
			break;
		case SCARD_E_DIR_NOT_FOUND:
			ret = L"The identified directory does not exist in the smart card.";
			break;
		case SCARD_E_FILE_NOT_FOUND:
			ret = L"The identified file does not exist in the smart card.";
			break;
		case SCARD_E_NO_DIR:
			ret = L"The supplied path does not represent a smart card directory.";
			break;
		case SCARD_E_NO_FILE:
			ret = L"The supplied path does not represent a smart card file.";
			break;
		case SCARD_E_NO_ACCESS:
			ret = L"Access is denied to this file.";
			break;
		case SCARD_E_WRITE_TOO_MANY:
			ret = L"The smart card does not have enough memory to store the information.";
			break;
		case SCARD_E_BAD_SEEK:
			ret = L"There was an error trying to set the smart card file object pointer.";
			break;
		case SCARD_E_INVALID_CHV:
			ret = L"The supplied PIN is incorrect.";
			break;
		case SCARD_E_UNKNOWN_RES_MNG:
			ret = L"An unrecognized error code was returned from a layered component.";
			break;
		case SCARD_E_NO_SUCH_CERTIFICATE:
			ret = L"The requested certificate does not exist.";
			break;
		case SCARD_E_CERTIFICATE_UNAVAILABLE:
			ret = L"The requested certificate could not be obtained.";
			break;
		case SCARD_E_NO_READERS_AVAILABLE:
			ret = L"Cannot find a smart card reader.";
			break;
		case SCARD_E_COMM_DATA_LOST:
			ret = L"A communications error with the smart card has been detected. Retry the operation.";
			break;
		case SCARD_E_NO_KEY_CONTAINER:
			ret = L"The requested key container does not exist on the smart card.";
			break;
		case SCARD_E_SERVER_TOO_BUSY:
			ret = L"The Smart Card Resource Manager is too busy to complete this operation.";
			break;
		case SCARD_E_PIN_CACHE_EXPIRED:
			ret = L"The smart card PIN cache has expired.";
			break;
		case SCARD_E_NO_PIN_CACHE:
			ret = L"The smart card PIN cannot be cached.";
			break;
		case SCARD_E_READ_ONLY_CARD:
			ret = L"The smart card is read only and cannot be written to.";
			break;
		default:
			ret = NULL;
	}
	return ret;
}

void show_scard_error(LONG err_code) {
	WCHAR *err_string = NULL;
	if (NULL != (err_string = resolve_error_string((long) err_code))) {
		MessageBox(0, resolve_error_string(err_code), L"Error", MB_ICONEXCLAMATION);
	} else if (NULL != (err_string = resolve_warning_string((long) err_code))) {
		MessageBox(0, resolve_error_string(err_code), L"Warning", MB_ICONEXCLAMATION);
	} else {
		MessageBox(0, L"Unknown error", L"Error", MB_ICONEXCLAMATION);
	}
}



