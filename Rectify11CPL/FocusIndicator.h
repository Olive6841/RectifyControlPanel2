#pragma once

class CFocusIndicator : public DirectUI::Element
{
public:
    static HRESULT Create(DirectUI::Element *pParent, DWORD *pdwDeferCookie, DirectUI::Element **ppElement);
    static DirectUI::IClassInfo *Class;
    DirectUI::IClassInfo *GetClassInfo() override;
    static HRESULT Register();

    static const DirectUI::PropertyInfo *FirstTabTargetProp;
    ATOM GetTargetId();
};
