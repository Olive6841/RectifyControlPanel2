#pragma once

class CControlPanelNavLink // should be 88 bytes (0x58) in size
{
public:
	CControlPanelNavLink(CPNAV_LIST list);
	~CControlPanelNavLink();

	static HRESULT Create(CPNAV_LIST list, CControlPanelNavLink **ppLink);

	CPNAV_LIST GetList() { return _list; }

	// HRESULT SetName(struct HINSTANCE__ * ,unsigned int );
	void SetName(LPCWSTR pszName);
	LPWSTR GetName() { return _pszName; }

	// HRESULT SetNameAcc(struct HINSTANCE__ * ,unsigned int );
	// HRESULT SetNameAcc(wchar_t * );
	LPWSTR GetNameAcc() { return _pszNameAcc; }

	// HRESULT SetIcon(HINSTANCE hInstance, int nIconId);
	// HRESULT SetIcon(HICON);
	HICON GetIcon() { return _hIcon; }

	// void SetSQMStream(unsigned long ,unsigned long ,struct _SQM_STREAM_ENTRY * );
	// HRESULT SetCommandNotify(int );
	// HRESULT SetCommandShellEx(wchar_t * ,wchar_t * );
	// HRESULT SetCommandControlPanel(wchar_t *, wchar_t *, bool);

	CControlPanelNavLinkCommand *GetCommand() { return &_cmd; }

	// void SetDisabled(bool);
	bool GetDisabled() { return _fDisabled; }


	CPNAV_LIST _list;
	WCHAR *_pszName;
	WCHAR *_pszNameAcc;
	HICON _hIcon;
	CControlPanelNavLinkCommand _cmd;
	bool _fDisabled;
};
