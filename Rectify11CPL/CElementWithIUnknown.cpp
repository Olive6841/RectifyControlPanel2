#include "CElementWithIUnknown.h"

#if 1

IClassInfo *CElementWithIUnknown::Class = nullptr;

HRESULT CElementWithIUnknown::Create(DirectUI::Element *pParent, DWORD *pdwDeferCookie, DirectUI::Element **ppElement)
{
	UNREFERENCED_PARAMETER(pParent);
	UNREFERENCED_PARAMETER(pdwDeferCookie);
	UNREFERENCED_PARAMETER(ppElement);

	DUI_ASSERT("Cannot instantiate a CElementWithIUnknown via parser. Must derive.");

	return E_NOTIMPL;
}

IClassInfo *CElementWithIUnknown::GetClassInfo()
{
    return Class;
}

HRESULT CElementWithIUnknown::Register()
{
	return ClassInfo<CElementWithIUnknown, DirectUI::Element>::RegisterGlobal(g_hInst, L"CElementWithIUnknown", 0, NULL);
}

IUnknown *CElementWithIUnknown::GetUnknownFromElement(DirectUI::Element *pe)
{
    IUnknown *punk = nullptr;

    DirectUI::IClassInfo *pci = pe->GetClassInfoW();
    while (pci)
    {
        if (CompareStringOrdinal(pci->GetName(), -1, CElementWithIUnknown::Class->GetName(), -1, FALSE) == CSTR_EQUAL)
        {
            punk = (IUnknown *)(CElementWithIUnknown *)pe;
            break;
        }
        pci = pci->GetBaseClass();
    }

    return punk;
}

#endif
