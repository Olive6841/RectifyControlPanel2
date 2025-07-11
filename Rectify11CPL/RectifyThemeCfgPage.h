#pragma once

class RectifyThemeCfgPage : public Element
{
public:
	RectifyThemeCfgPage();
	virtual ~RectifyThemeCfgPage() override;

	static DirectUI::IClassInfo* Class;
	static HRESULT Create(Element* pParent, DWORD* pdwDeferCookie, Element** ppElement);

	//Element
	virtual IClassInfo* GetClassInfoW() override;

	// Element overrides
	virtual void OnEvent(Event* iev) override;

	// Important methods
	virtual void OnInit();
	virtual void SetSite(IUnknown* site) { this->site = site; }

	static inline DirectUI::IClassInfo* GetClassInfoPtr() { return Class; }

	void GoBack();
 private:
	IUnknown* site = NULL;
	bool initializing = true;
};

