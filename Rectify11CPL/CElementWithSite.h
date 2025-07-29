#pragma once

#include "CElementWithIUnknown.h"

extern const IID IID_IFrameManager;

class CElementWithSite
	: public CElementWithIUnknown
	, public IObjectWithSite
{
public:
	STDMETHODIMP QueryInterface(REFIID, void **) override;
	STDMETHODIMP_(ULONG) AddRef() override;
	STDMETHODIMP_(ULONG) Release() override;

	STDMETHODIMP SetSite(IUnknown *punkSite) override;
	STDMETHODIMP GetSite(REFIID riid, void **ppvSite) override;

	static HRESULT Create(DirectUI::Element *pParent, DWORD *pdwDeferCookie, DirectUI::Element **ppElement);

	static DirectUI::IClassInfo *Class;
	DirectUI::IClassInfo *GetClassInfo() override { return Class; }
	static HRESULT Register();

	HRESULT GetPersistStream(int, unsigned long, IStream **);

protected:
	~CElementWithSite();

	IUnknown *_punkSite;
};
