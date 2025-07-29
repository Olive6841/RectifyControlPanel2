#pragma once

class CElementWithIUnknown
	: public DirectUI::Element
	, public IUnknown
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv) override PURE;
	STDMETHODIMP_(ULONG) AddRef() override PURE;
	STDMETHODIMP_(ULONG) Release() override PURE;

	static HRESULT Create(DirectUI::Element *pParent, DWORD *pdwDeferCookie, DirectUI::Element **ppElement);

	static DirectUI::IClassInfo *Class;
	DirectUI::IClassInfo *GetClassInfo() override;
	static HRESULT Register();

	static IUnknown *GetUnknownFromElement(Element *pe);
};
