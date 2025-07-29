#pragma once

#include "CControlPanelNavLink.h"

class CControlPanelNavLinks : public IUnknown
{
public:
	CControlPanelNavLinks();

	IFACEMETHODIMP QueryInterface(REFIID riid, __out void **ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	HRESULT Add(CControlPanelNavLink *pLink);
	HRESULT AddLinkNotify(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, int nLinkId, CControlPanelNavLink **ppLink);
	HRESULT AddLinkShellEx(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, LPCWSTR pszCommand, LPCWSTR pszParams, CControlPanelNavLink **ppLink);
	HRESULT AddLinkControlPanel(CPNAV_LIST list, HINSTANCE hInstance, UINT nNameResId, LPCWSTR pszApplet, LPCWSTR pszAppletPage, CControlPanelNavLink **ppLink);

protected:
	virtual ~CControlPanelNavLinks();

private:
	HDPA _hdpaNavLinks;
	long _cRef;
};
