#pragma once

#include "CControlPanelPage.h"

#include <map>
#include "IRectifyUtil_h.h"

typedef std::map<int, wstring> ThemesMapBase;

class RectifyMainPage : public CControlPanelPage
{
public:
	RectifyMainPage();
	virtual ~RectifyMainPage() override;

	static HRESULT Create(Element *pParent, DWORD *pdwDeferCookie, Element **ppElement);

	// Element overrides
	virtual void OnEvent(Event *iev) override;
	virtual void OnDestroy() override;

	static DirectUI::IClassInfo* Class;
	virtual IClassInfo *GetClassInfo() override;

	static HRESULT Register();

	IFACEMETHOD(QueryInterface)(REFIID riid, void ** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override { return CElementWithSite::AddRef(); }
	IFACEMETHOD_(ULONG, Release)() override { return CElementWithSite::Release(); }

	// Important methods
	virtual HRESULT LayoutInitialized() override;
	virtual HRESULT OnInnerElementDestroyed() override;

private:
	IUnknown *_punkSite;
	bool HasAdmin;
	IRectifyUtil* RectifyUtil = NULL;
	bool initializing;

	vector<ULONG> themes;
	ThemesMapBase ThemesMap;

	void UpdateThemeGraphic();
	void ShowRestartExplorer();
	void InitNavLinks();
	void UpdateThemetoolStatus();
	HWND GetMainHwnd();
};