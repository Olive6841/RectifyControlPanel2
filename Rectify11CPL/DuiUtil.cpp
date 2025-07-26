#include "DuiUtil.h"

HRESULT WINAPI DUI_SetSiteOnUnknown(IUnknown *punk, IUnknown *punkSite)
{
    return IUnknown_SetSite(punk, punkSite);
}

void WINAPI DUI_SendSelectionChangedToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameShellViewClient *pfsvc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfsvc))))
    {
        pfsvc->OnSelectionChanged();
        pfsvc->Release();
    }
}

void WINAPI DUI_SendSelectedItemChangedToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameShellViewClient *pfsvc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfsvc))))
    {
        pfsvc->OnSelectedItemChanged();
        pfsvc->Release();
    }
}

void WINAPI DUI_SendNotificationToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameNotificationClient *pfnc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfnc))))
    {
        pfnc->Notify((LPCWSTR)lparam);
        pfnc->Release();
    }
}

void WINAPI DUI_SendNavigateAwayToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameNotificationClient *pfnc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfnc))))
    {
        pfnc->OnNavigateAway();
        pfnc->Release();
    }
}

void WINAPI DUI_SendInitializationToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameNotificationClient *pfnc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfnc))))
    {
        pfnc->LayoutInitialized();
        pfnc->Release();
    }
}

void WINAPI DUI_SendFolderChangedToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameShellViewClient *pfsvc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfsvc))))
    {
        pfsvc->OnFolderChanged();
        pfsvc->Release();
    }
}

void WINAPI DUI_SendContentsChangedToUnknown(IUnknown *punk, LPARAM lparam)
{
    IFrameShellViewClient *pfsvc;
    if (SUCCEEDED(punk->QueryInterface(IID_PPV_ARGS(&pfsvc))))
    {
        pfsvc->OnContentsChanged();
        pfsvc->Release();
    }
}

void DUI_WalkIUnknownElements(DirectUI::Element *pe, PFNELEMENTCALLBACK pfn, LPARAM lparam)
{
    if (pe)
    {
        DirectUI::Value *pvChildren;
        DirectUI::DynamicArray<DirectUI::Element *> *pel = pe->GetChildren(&pvChildren);
        if (pel)
        {
            for (UINT i = 0; i < pel->GetSize(); ++i)
            {
                DirectUI::Element *peChild = pel->GetItem(i);
                IUnknown *punk = CElementWithIUnknown::GetUnknownFromElement(peChild);
                if (punk)
                {
                    pfn(punk, lparam);
                }

                DUI_WalkIUnknownElements(peChild, pfn, lparam);
            }

            pvChildren->Release();
        }
    }
}