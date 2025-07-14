#pragma once

class CControlPanelNavLink // should be 88 bytes
{
public:
	static HRESULT Create(CPNAV_LIST list, CControlPanelNavLink** result);
	void SetName(LPCWSTR name);

	CPNAV_LIST m_Type;
	WCHAR* m_Name;
	WCHAR* m_args;
	HICON m_Icon;
	CControlPanelNavLinkCommand m_ExecType;
	bool _fDisabled;
	//char unknown_bytes[0x1B];
};
