#pragma once
#include <map>
#include "IRectifyUtil_h.h"

typedef std::map<int, wstring> ThemesMapBase;

class RectifyMainPage : public Element
{
public:

	RectifyMainPage();
	virtual ~RectifyMainPage() override;

	static DirectUI::IClassInfo* Class;
	static HRESULT Create(Element* pParent, DWORD* pdwDeferCookie, Element** ppElement);

	//Element
	virtual IClassInfo* GetClassInfoW() override;

	// Element overrides
	virtual void OnEvent(Event* iev) override;
	virtual void OnDestroy() override;

	// Important methods
	virtual void OnInit();
	virtual void SetSite(IUnknown* site) { this->site = site;  }

	static inline DirectUI::IClassInfo* GetClassInfoPtr() { return Class; }

private:
	bool HasAdmin = false;
	IRectifyUtil* RectifyUtil = NULL;
	vector<ULONG> themes;
	ThemesMapBase ThemesMap;
	IUnknown* site = NULL;
	bool initializing = true;

	void UpdateThemeGraphic();
	void ShowRestartExplorer();
	void InitNavLinks();
	void UpdateThemetoolStatus();
	HWND GetMainHwnd();
};