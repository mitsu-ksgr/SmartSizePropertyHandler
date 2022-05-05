/*
 * SmartSizePropertyHandler.h
 */
#ifndef __SmartSizePropertyHandler_H__
#define __SmartSizePropertyHandler_H__

#include <windows.h>


//
// Defines
//
#define SMARTSIZE_PROP_DESCRIPTION	L"SmartSize Property Handler."
#define SMARTSIZE_PROP_GUID			"{E5E96240-C9EE-11EC-9D64-0242AC120002}"
#define SMARTSIZE_PROP_GUID_W		L"{E5E96240-C9EE-11EC-9D64-0242AC120002}"
#define SMARTSIZE_PROP_FORMAT_ID	"E5E96240-C9EE-11EC-9D64-0242AC120002"
#define SMARTSIZE_PROP_PROP_ID		100
#define SMARTSIZE_PROP_KEY			{0xe5e96240, 0xc9ee, 0x11ec, 0x9d, 0x64, 0x02, 0x42, 0xac, 0x12, 0x00, 0x02}

// Register the PropertyHandler for the following extension.
// However, if the extension is registreed in
//	SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\SystemPropertyHandlers
// , you need to change it directly. ‚¾‚é.
#define SMARTSIZE_PROP_TARGET		L".mp4"

// Attaches a GUID to the CSmartSizePropertyHandler class.
class __declspec(uuid(SMARTSIZE_PROP_FORMAT_ID)) SmartSizePropertyHandler;


//
// Functions
//
HRESULT SmartSizePropertyHandler_CreateInstance(REFIID riid, void ** ppv);


#endif /* __SmartSizePropertyHandler_H__ */
