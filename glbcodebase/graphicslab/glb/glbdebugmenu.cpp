//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 28
// Brief: Debug Menu System
//----------------------------------------------------------------------
#include "glbdebugmenu.h"

#include <stdint.h>

#include <vector>

#include "glb.h"

namespace glb {

namespace debugmenu {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------
static const int32_t kMaxMenuItemNameLength = 64;
static const char kRootMenuItemName[] = "_RootMenu";
static const float kMenuItemSizeY = 24.0f;
static const float kMenuItemTextExtraSizeXPadding = 32.0f;
static const float kMenuStartXPos = 2.0f;
static const float kMenuStartYPos = 2.0f;
static const float kMenuTextXOffset = 5.0f;
static const float kMenuTextYOffset = 2.5f;
static const float kPaddingXBetweenMenu = 2.0f;
static const float kMenuOpenCharacterSizeX = 12.0f;

//-----------------------------------------------------------------------------------
// FUNCTION
//----------------------------------------------------------------------------------
float ScreenPosToNDCPosWidth(float pixel);
float ScreenPosToNDCPosHeight(float pixel);
float ScreenSizeToNDCSizeWidth(float pixel);
float ScreenSizeToNDCSizeHeight(float pixel);

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------
// FUNCTION
//----------------------------------------------------------------------------------
float ScreenPosToNDCPosWidth(float pixel) {
    int32_t width = glb::app::Application::GetWindowWidth();
    return ((pixel / width) - 0.5f) * 2.0f;
}

float ScreenPosToNDCPosHeight(float pixel) {
    int32_t height = glb::app::Application::GetWindowHeight();
    return (((height - pixel) / height) - 0.5f) * 2.0f;
}

float ScreenSizeToNDCSizeWidth(float pixel) {
    int32_t width = glb::app::Application::GetWindowWidth();
    return (pixel / width) * 2.0f;
}

float ScreenSizeToNDCSizeHeight(float pixel) {
    int32_t height = glb::app::Application::GetWindowHeight();
    return (pixel / height) * 2.0f;
}

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DebugMenuItem DECLARATION
//----------------------------------------------------------------------------------
class DebugMenuItem {
public:
    DebugMenuItem(const char* menuName);
    virtual ~DebugMenuItem();

    virtual std::vector<DebugMenuItem*> GetChildrenMenuItems();
    virtual const char* GetMenuName();
    virtual const char* GetDisplayText();
    virtual bool IsOpened();
    virtual void SetOpened(bool bOpened);
    virtual bool IsSelected();
    virtual void SetSelected(bool bSelected);

    virtual DebugMenuItem* FindMenuItem(const char* menu);
    virtual void AddMenuItem(DebugMenuItem* item);
    virtual float GetSizeX() const;
    virtual float GetSizeY() const;
    virtual float GetPosX() const;
    virtual float GetPosY() const;
    virtual void CalculateChildrenMenuSize(float& sizeX, float& sizeY);
    virtual DebugMenuItem* GetChildrenOpenedMenuItem();

    virtual void HandleMouseEvent();

protected:
    void CalculateChildrenMenuPos();

protected:
    char                                    m_MenuItemName[kMaxMenuItemNameLength];
    DebugMenuItem*                          m_ParentMenuItem;
    std::vector<DebugMenuItem*>             m_ChildMenuItems;
    float                                   m_NeedSizeX;
    float                                   m_NeedSizeY;
    float                                   m_RealSizeX;
    float                                   m_RealSizeY;
    float                                   m_PosX;
    float                                   m_PosY;
    bool                                    m_IsOpened;
    bool                                    m_IsSelected;

    friend class MgrImp;
};

//-----------------------------------------------------------------------------------
// DebugMemuTriggerItem DECLARATION
//----------------------------------------------------------------------------------
class DebugMenuTriggerItem : public DebugMenuItem {
public:
    DebugMenuTriggerItem(const char* menuName, bool& trigger);
    virtual ~DebugMenuTriggerItem();

    virtual const char* GetDisplayText() override;
    virtual void AddMenuItem(DebugMenuItem* item) override;
    virtual void HandleMouseEvent() override;

protected:
    bool&   m_Trigger;
    char    m_DisplayText[kMaxMenuItemNameLength];
    float   m_PressDelta;
};

//-----------------------------------------------------------------------------------
// DebugMenuAdjustIntItem DECLARATION
//----------------------------------------------------------------------------------
class DebugMenuAdjustIntItem : public DebugMenuItem {
public:
    DebugMenuAdjustIntItem(const char* menuName, int32_t& value, int32_t min, int32_t max);
    virtual ~DebugMenuAdjustIntItem();

    virtual const char* GetDisplayText() override;
    virtual void AddMenuItem(DebugMenuItem* item) override;
    virtual void HandleMouseEvent() override;

protected:
    char                    m_DisplayText[kMaxMenuItemNameLength];
    int32_t&                m_Value;
    int32_t                 m_Min;
    int32_t                 m_Max;
};

//-----------------------------------------------------------------------------------
// DebugMenuAdjustFloatItem DECLARATION
//----------------------------------------------------------------------------------
class DebugMenuAdjustFloatItem : public DebugMenuItem {
public:
    DebugMenuAdjustFloatItem(const char* menu, float& value, float step, float min, float max, const char* format);
    virtual ~DebugMenuAdjustFloatItem();

    virtual const char* GetDisplayText() override;
    virtual void AddMenuItem(DebugMenuItem* item);
    virtual void HandleMouseEvent() override;

protected:
    char                    m_DisplayText[kMaxMenuItemNameLength];
    float&                  m_Value;
    float                   m_Min;
    float                   m_Max;
    float                   m_Step;
    char                    m_Format[kMaxMenuItemNameLength];
};

//-----------------------------------------------------------------------------------
// MgrImp DECLARATION
//----------------------------------------------------------------------------------
class MgrImp {
public:
    MgrImp();
    virtual~ MgrImp();

    void Initialize();
    void Update();
    void Destroy();

    void AddMenuItem(const char* menu, bool& trigger);
    void AddMenuItem(const char* menu, int32_t& value, int32_t min, int32_t max);
    void AddMenuItem(const char* menu, float& value, float step, float min, float max, const char* format);
    DebugMenuItem* GetRootMenu();

protected:
    std::vector<std::string> ParseMenu(const char* menu);
    bool CheckIfInMenuItem(DebugMenuItem* item, float mouseX, float mouseY);

    void OpenRootMenu();
    void HandleMouseEvent();
    void CalculateMenuMesh();
    void CalculateMenuText();

    void ChangeCurSelectedMenuItem(DebugMenuItem* item);
    void CalculateMenuBackground();
    void CalculateMenuOpenedBackground();
    void CalculateMenuActivedBackground();

protected:
    DebugMenuItem*          m_RootMenu;
    DebugMenuItem*          m_CurSelectedMenu;
    int32_t                 m_PressDelta;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DebugMenuItem DEFINITION
//-----------------------------------------------------------------------------------
DebugMenuItem::DebugMenuItem(const char* menuItemName)
: m_ParentMenuItem(nullptr)
, m_NeedSizeX(0.0f)
, m_NeedSizeY(0.0f)
, m_RealSizeX(0.0f)
, m_RealSizeY(0.0f)
, m_PosX(0.0f)
, m_PosY(0.0f)
, m_IsOpened(false)
, m_IsSelected(false) {
    // Copy menu item name
    int32_t len = strlen(menuItemName);
    len = min(len, kMaxMenuItemNameLength - 1);
    memcpy(m_MenuItemName, menuItemName, len);
    m_MenuItemName[len] = '\0';

    m_ChildMenuItems.clear();

    render::font::Mgr::GetTextSize(menuItemName, m_NeedSizeX, m_NeedSizeY);
    m_NeedSizeX = m_NeedSizeX + ScreenSizeToNDCSizeWidth(kMenuItemTextExtraSizeXPadding);
    m_RealSizeY = m_NeedSizeY = ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
}

DebugMenuItem::~DebugMenuItem() {
    for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
        GLB_SAFE_DELETE(m_ChildMenuItems[i]);
    }

    m_ChildMenuItems.clear();
    m_ParentMenuItem = nullptr;
}

std::vector<DebugMenuItem*> DebugMenuItem::GetChildrenMenuItems() {
    return m_ChildMenuItems;
}

const char* DebugMenuItem::GetMenuName() {
    return m_MenuItemName;
}

const char* DebugMenuItem::GetDisplayText() {
    return m_MenuItemName;
}

bool DebugMenuItem::IsOpened() {
    return m_IsOpened;
}

void DebugMenuItem::SetOpened(bool bOpened) {
    if (m_ChildMenuItems.size() > 0) {  // Only parent menu item can have open flag
        m_IsOpened = bOpened;
    }
}

bool DebugMenuItem::IsSelected() {
    return m_IsSelected;
}

void DebugMenuItem::SetSelected(bool bSelected) {
    m_IsSelected = bSelected;
}

DebugMenuItem* DebugMenuItem::FindMenuItem(const char* menu) {
    DebugMenuItem* result = nullptr;

    if (menu) {
        if (!strcmp(m_MenuItemName, menu)) {
            // Found it, return
            result = this;
        } else {
            // Find it in children menu item
            for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
                if (m_ChildMenuItems[i]) {
                    result = m_ChildMenuItems[i]->FindMenuItem(menu);
                    if (result != nullptr) {
                        break;
                    }
                }
            }
        }
    }

    return result;
}

void DebugMenuItem::AddMenuItem(DebugMenuItem* item) {
    if (item) {
        m_ChildMenuItems.push_back(item);
        item->m_ParentMenuItem = this;

        float sizeX = 0.0f, sizeY = 0.0f;
        CalculateChildrenMenuSize(sizeX, sizeY);
        for (auto item : m_ChildMenuItems) {
            item->m_RealSizeX = sizeX;
        }

        CalculateChildrenMenuPos();
    }
}

float DebugMenuItem::GetSizeX() const {
    return m_RealSizeX;
}

float DebugMenuItem::GetSizeY() const {
    return m_RealSizeY;
}

float DebugMenuItem::GetPosX() const {
    return m_PosX;
}

float DebugMenuItem::GetPosY() const {
    return m_PosY;
}

void DebugMenuItem::CalculateChildrenMenuSize(float& sizeX, float& sizeY) {
    sizeX = 0, sizeY = m_ChildMenuItems.size() * ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
    for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
        if (m_ChildMenuItems[i]) {
            if (sizeX < m_ChildMenuItems[i]->m_NeedSizeX) {
                sizeX = m_ChildMenuItems[i]->m_NeedSizeX;
            }
        }
    }
}

DebugMenuItem* DebugMenuItem::GetChildrenOpenedMenuItem() {
    DebugMenuItem* menuItem = nullptr;

    for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
        if (m_ChildMenuItems[i] && m_ChildMenuItems[i]->m_IsOpened) {
            menuItem = m_ChildMenuItems[i];
            break;
        }
    }

    return menuItem;
}

void DebugMenuItem::HandleMouseEvent() {
    // Do noting
}

void DebugMenuItem::CalculateChildrenMenuPos() {
    if (!m_ParentMenuItem) {
        // Current menu is root menu
        for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
            auto item = m_ChildMenuItems[i];
            if (item) {
                item->m_PosX = -1.0f;
                item->m_PosY = 1.0f - ScreenSizeToNDCSizeHeight(kMenuItemSizeY) * i;
            }
        }
    } else {
        // Current menu is child menu
        for (int32_t i = 0; i < m_ChildMenuItems.size(); i++) {
            auto item = m_ChildMenuItems[i];
            if (item) {
                item->m_PosX = m_PosX + m_RealSizeX + ScreenSizeToNDCSizeWidth(kPaddingXBetweenMenu);
                item->m_PosY = 1.0f - ScreenSizeToNDCSizeHeight(kMenuItemSizeY) * i;
            }
        }
    }

    // Recursion
    for (auto item : m_ChildMenuItems) {
        if (item) {
            item->CalculateChildrenMenuPos();
        }
    }
}

//-----------------------------------------------------------------------------------
// DebugMenuTriggerItem DEFINITION
//-----------------------------------------------------------------------------------
DebugMenuTriggerItem::DebugMenuTriggerItem(const char* menuName, bool& trigger)
: DebugMenuItem(menuName)
, m_Trigger(trigger)
, m_DisplayText()
, m_PressDelta(0.0f) {
}

DebugMenuTriggerItem::~DebugMenuTriggerItem() {
}

const char* DebugMenuTriggerItem::GetDisplayText() {
    if (m_Trigger) {
        sprintf_s(m_DisplayText, "[*]%s", m_MenuItemName);
    } else {
        sprintf_s(m_DisplayText, "[_]%s", m_MenuItemName);
    }

    return m_DisplayText;
}

void DebugMenuTriggerItem::AddMenuItem(DebugMenuItem* item) {
    static_cast<void*>(item);
}

void DebugMenuTriggerItem::HandleMouseEvent() {
    if (glb::Input::IsMouseButtonPressed(glb::BM_LEFT) && m_PressDelta > 10.0f) {
        m_Trigger = !m_Trigger;
        m_PressDelta = 0.0f;
    }

    m_PressDelta = m_PressDelta + 1.0f;
}

//-----------------------------------------------------------------------------------
// DebugMenuTriggerItem DEFINITION
//-----------------------------------------------------------------------------------
DebugMenuAdjustIntItem::DebugMenuAdjustIntItem(const char* menuName, int32_t& value, int32_t min, int32_t max)
: DebugMenuItem(menuName)
, m_Value(value)
, m_Min(min)
, m_Max(max) {
    float minNeedSizeX = 0.0f, minNeedSizeY = 0.0f;
    char buffer[kMaxMenuItemNameLength];
    sprintf_s(buffer, "%s  %d", menuName, min);
    render::font::Mgr::GetTextSize(buffer, minNeedSizeX, minNeedSizeY);

    float maxNeedSizeX = 0.0f, maxNeedSizeY = 0.0f;
    sprintf_s(buffer, "%s  %d", menuName, max);
    render::font::Mgr::GetTextSize(buffer, maxNeedSizeX, maxNeedSizeY);

    m_NeedSizeX = max(minNeedSizeX, maxNeedSizeX);
    m_NeedSizeY = max(minNeedSizeY, maxNeedSizeY);

    m_NeedSizeX = m_NeedSizeX + ScreenSizeToNDCSizeWidth(kMenuItemTextExtraSizeXPadding);
    m_RealSizeY = m_NeedSizeY = ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
}

DebugMenuAdjustIntItem::~DebugMenuAdjustIntItem() {
}

const char* DebugMenuAdjustIntItem::GetDisplayText() {
    sprintf_s(m_DisplayText, "%s  %d", m_MenuItemName, m_Value);
    return m_DisplayText;
}

void DebugMenuAdjustIntItem::AddMenuItem(DebugMenuItem* item) {
    static_cast<void*>(item);
}

void DebugMenuAdjustIntItem::HandleMouseEvent() {
    int64_t move = glb::Input::GetMouseMoveZ();
    if (move != 0) {
        m_Value = m_Value + move / abs(move);
    }

    if (m_Value < m_Min) {
        m_Value = m_Min;
    }
    if (m_Value > m_Max) {
        m_Value = m_Max;
    }
}

//-----------------------------------------------------------------------------------
// DebugMenuAdjustFloatItem DEFINITION
//----------------------------------------------------------------------------------
DebugMenuAdjustFloatItem::DebugMenuAdjustFloatItem(const char* menuName, float& value, float step, float min, float max, const char* format)
: DebugMenuItem(menuName)
, m_Value(value)
, m_Step(step)
, m_Min(min)
, m_Max(max) {
    sprintf_s(m_Format, "%%s  %s", format);

    float minNeedSizeX = 0.0f, minNeedSizeY = 0.0f;
    char buffer[kMaxMenuItemNameLength];
    sprintf_s(buffer, m_Format, menuName, min);
    render::font::Mgr::GetTextSize(buffer, minNeedSizeX, minNeedSizeY);

    float maxNeedSizeX = 0.0f, maxNeedSizeY = 0.0f;
    sprintf_s(buffer, m_Format, menuName, max);
    render::font::Mgr::GetTextSize(buffer, maxNeedSizeX, maxNeedSizeY);

    m_NeedSizeX = max(minNeedSizeX, maxNeedSizeX);
    m_NeedSizeY = max(minNeedSizeY, maxNeedSizeY);

    m_NeedSizeX = m_NeedSizeX + ScreenSizeToNDCSizeWidth(kMenuItemTextExtraSizeXPadding);
    m_RealSizeY = m_NeedSizeY = ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
}

DebugMenuAdjustFloatItem::~DebugMenuAdjustFloatItem() {
}

const char* DebugMenuAdjustFloatItem::GetDisplayText() {
    sprintf_s(m_DisplayText, m_Format, m_MenuItemName, m_Value);
    return m_DisplayText;
}

void DebugMenuAdjustFloatItem::AddMenuItem(DebugMenuItem* item) {
    static_cast<void*>(item);
}

void DebugMenuAdjustFloatItem::HandleMouseEvent() {
    int64_t move = glb::Input::GetMouseMoveZ();
    if (move != 0) {
        m_Value = m_Value + m_Step * move / abs(move);
    }

    if (m_Value < m_Min) {
        m_Value = m_Min;
    }
    if (m_Value > m_Max) {
        m_Value = m_Max;
    }
}

//-----------------------------------------------------------------------------------
// MgrImp DECLARATION
//----------------------------------------------------------------------------------
MgrImp::MgrImp()
: m_RootMenu(nullptr)
, m_CurSelectedMenu(nullptr)
, m_PressDelta(0) {
}

MgrImp::~MgrImp() {
    GLB_SAFE_DELETE(m_RootMenu);
}

void MgrImp::Initialize() {
    m_RootMenu = new DebugMenuItem(kRootMenuItemName);
}

void MgrImp::Update() {
    OpenRootMenu();
    HandleMouseEvent();
    CalculateMenuMesh();
    CalculateMenuText();
}

void MgrImp::Destroy() {
}

void MgrImp::AddMenuItem(const char* menu, bool& trigger) {
    std::vector<std::string>&& menuItemNames = ParseMenu(menu);

    DebugMenuItem* parentMenu = m_RootMenu;
    for (int32_t i = 0; i < menuItemNames.size(); i++) {
        if (i != menuItemNames.size() - 1) {
            // Find if already exist
            DebugMenuItem* item = parentMenu->FindMenuItem(menuItemNames[i].c_str());

            if (item) { // If true
                parentMenu = item;
            } else {
                // If false, create a menu item
                item = new DebugMenuItem(menuItemNames[i].c_str());
                parentMenu->AddMenuItem(item);
                parentMenu = item;
            }
        } else {
            DebugMenuItem* item = new DebugMenuTriggerItem(menuItemNames[i].c_str(), trigger);
            parentMenu->AddMenuItem(item);
        }
    }
}

void MgrImp::AddMenuItem(const char* menu, int32_t& value, int32_t min, int32_t max) {
    std::vector<std::string>&& menuItemNames = ParseMenu(menu);

    DebugMenuItem* parentMenu = m_RootMenu;
    for (int32_t i = 0; i < menuItemNames.size(); i++) {
        if (i != menuItemNames.size() - 1) {
            // Find if already exist
            DebugMenuItem* item = parentMenu->FindMenuItem(menuItemNames[i].c_str());

            if (item) { // If true
                parentMenu = item;
            } else {
                // If false, create a menu item
                item = new DebugMenuItem(menuItemNames[i].c_str());
                parentMenu->AddMenuItem(item);
                parentMenu = item;
            }
        } else {
            DebugMenuItem* item = new DebugMenuAdjustIntItem(menuItemNames[i].c_str(), value, min, max);
            parentMenu->AddMenuItem(item);
        }
    }
}

void MgrImp::AddMenuItem(const char* menu, float& value, float step, float min, float max, const char* format) {
    std::vector<std::string>&& menuItemNames = ParseMenu(menu);

    DebugMenuItem* parentMenu = m_RootMenu;
    for (int32_t i = 0; i < menuItemNames.size(); i++) {
        if (i != menuItemNames.size() - 1) {
            // Find if already exist
            DebugMenuItem* item = parentMenu->FindMenuItem(menuItemNames[i].c_str());

            if (item) { // If true
                parentMenu = item;
            } else {
                // If false, create a menu item
                item = new DebugMenuItem(menuItemNames[i].c_str());
                parentMenu->AddMenuItem(item);
                parentMenu = item;
            }
        } else {
            DebugMenuItem* item = new DebugMenuAdjustFloatItem(menuItemNames[i].c_str(), value, step, min, max, format);
            parentMenu->AddMenuItem(item);
        }
    }
}

DebugMenuItem* MgrImp::GetRootMenu() {
    return m_RootMenu;
}

std::vector<std::string> MgrImp::ParseMenu(const char* menu) {
    std::vector<std::string> menuItemNames;

    if (menu) {
        int32_t preIndex = 0, index = 0;
        std::string totalMenu = menu;
        while (true) {
            index = totalMenu.find_first_of("|", preIndex);
            if (index == -1) {
                menuItemNames.push_back(totalMenu.substr(preIndex));
                break;
            } else {
                menuItemNames.push_back(totalMenu.substr(preIndex, index - preIndex));
                preIndex = index + 1;
            }
        }
    }

    return menuItemNames;
}

bool MgrImp::CheckIfInMenuItem(DebugMenuItem* item, float mouseX, float mouseY) {
    bool result = true;

    if (ScreenPosToNDCPosWidth(mouseX) < item->GetPosX()) result = false;
    if (ScreenPosToNDCPosWidth(mouseX) > item->GetPosX() + item->GetSizeX()) result = false;
    if (ScreenPosToNDCPosHeight(mouseY) > item->GetPosY()) result = false;
    if (ScreenPosToNDCPosHeight(mouseY) < item->GetPosY() - item->GetSizeY()) result = false;

    return result;
}

void MgrImp::OpenRootMenu() {
    if (Input::IsKeyboardButtonPressed(glb::BK_F1) && m_PressDelta > 15) {
        GetRootMenu()->SetOpened(!GetRootMenu()->IsOpened());
        m_PressDelta = 0;
    }
    m_PressDelta++;
}

void MgrImp::HandleMouseEvent() {
    float mouseX = glb::Input::GetMousePosX();
    float mouseY = glb::Input::GetMousePosY();

    DebugMenuItem* menuItem = m_RootMenu;
    while (menuItem && menuItem->IsOpened()) {
        std::vector<DebugMenuItem*>&& childrenItems = menuItem->GetChildrenMenuItems();

        bool bFound = false;
        for (auto item : childrenItems) {
            if (item && CheckIfInMenuItem(item, mouseX, mouseY)) {
                ChangeCurSelectedMenuItem(item);
                bFound = true;
                break;
            }
        }

        if (bFound) {
            break;
        }

        menuItem = menuItem->GetChildrenOpenedMenuItem();
    }

    if (m_CurSelectedMenu) {
        m_CurSelectedMenu->HandleMouseEvent();
    }
}

void MgrImp::CalculateMenuMesh() {
    CalculateMenuBackground();
    CalculateMenuOpenedBackground();
    CalculateMenuActivedBackground();
}

void MgrImp::CalculateMenuText() {
    float xOffset = ScreenPosToNDCPosWidth(kMenuStartXPos);
    float yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    DebugMenuItem* menuItem = GetRootMenu();

    while (menuItem && menuItem->IsOpened()) {
        float sizeX = 0.0f, sizeY = 0.0f;
        menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

        std::vector<DebugMenuItem*>&& childItems = menuItem->GetChildrenMenuItems();
        for (int32_t i = 0; i < childItems.size(); i++) {
            render::Render::AddText(childItems[i]->GetDisplayText(), math::Vector(xOffset + ScreenSizeToNDCSizeWidth(kMenuTextXOffset), yOffset - ScreenSizeToNDCSizeHeight(kMenuTextYOffset), 0.0f), math::Vector(213.0f / 256.0f, 211.0f / 256.0f, 214.0f / 256.0f, 1.0f), 1.0f);
            if (childItems[i]->IsOpened()) {
                render::Render::AddText(">", math::Vector(xOffset + sizeX - ScreenSizeToNDCSizeWidth(kMenuOpenCharacterSizeX), yOffset - ScreenSizeToNDCSizeHeight(kMenuTextYOffset), 0.0f), math::Vector(213.0f / 256.0f, 211.0f / 256.0f, 214.0f / 256.0f, 1.0f), 1.0f);
            }

            yOffset = yOffset - childItems[i]->GetSizeY();
        }

        menuItem = menuItem->GetChildrenOpenedMenuItem();
        xOffset = xOffset + sizeX + ScreenSizeToNDCSizeWidth(kPaddingXBetweenMenu);
        yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    }
}

void MgrImp::ChangeCurSelectedMenuItem(DebugMenuItem* item) {
    if (item == nullptr) return;

    // Clear previous selected menu item open&selected flag
    DebugMenuItem* preCurSelMenuItem = m_CurSelectedMenu;
    while (preCurSelMenuItem) {
        preCurSelMenuItem->SetOpened(false);
        preCurSelMenuItem->SetSelected(false);

        preCurSelMenuItem = preCurSelMenuItem->m_ParentMenuItem;
        if (preCurSelMenuItem->m_ParentMenuItem == nullptr) {  // Root menu
            break;
        }
    }

    // Set current selected menu item open&selected flag
    DebugMenuItem* curSelMenuItem = item;
    curSelMenuItem->SetSelected(true);
    while (curSelMenuItem) {
        curSelMenuItem->SetOpened(true);

        curSelMenuItem = curSelMenuItem->m_ParentMenuItem;
        if (curSelMenuItem->m_ParentMenuItem == nullptr) {  // Root menu
            break;
        }
    }

    // Save current selected item
    m_CurSelectedMenu = item;
}

void MgrImp::CalculateMenuBackground() {
    float xOffset = ScreenPosToNDCPosWidth(kMenuStartXPos);
    float yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    DebugMenuItem* menuItem = GetRootMenu();

    while (menuItem && menuItem->IsOpened()) {
        float sizeX = 0.0f, sizeY = 0.0f;
        menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

        render::Render::AddMenuMesh(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - sizeY, 0.0f), math::Vector(0.0f, 0.0f, 0.0f, 1.0f));
        xOffset = xOffset + sizeX + ScreenSizeToNDCSizeWidth(kPaddingXBetweenMenu);

        menuItem = menuItem->GetChildrenOpenedMenuItem();
    }
}

void MgrImp::CalculateMenuOpenedBackground() {
    float xOffset = ScreenPosToNDCPosWidth(kMenuStartXPos);
    float yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    DebugMenuItem* menuItem = GetRootMenu();

    while (menuItem && menuItem->IsOpened()) {
        float sizeX = 0.0f, sizeY = 0.0f;
        menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

        std::vector<DebugMenuItem*>&& childrenItems = menuItem->GetChildrenMenuItems();
        for (int32_t i = 0; i < childrenItems.size(); i++) {
            if (childrenItems[i] && childrenItems[i]->IsOpened() && !childrenItems[i]->IsSelected()) {
                render::Render::AddMenuMesh(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - childrenItems[i]->GetSizeY(), 0.0f), math::Vector(0.4f, 0.4f, 0.4f, 0.25f));
            }

            yOffset = yOffset - ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
        }

        menuItem = menuItem->GetChildrenOpenedMenuItem();

        xOffset = xOffset + sizeX + ScreenSizeToNDCSizeWidth(kPaddingXBetweenMenu);
        yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    }
}

void MgrImp::CalculateMenuActivedBackground() {
    float xOffset = ScreenPosToNDCPosWidth(kMenuStartXPos);
    float yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    DebugMenuItem* menuItem = GetRootMenu();

    while (menuItem && menuItem->IsOpened()) {
        float sizeX = 0.0f, sizeY = 0.0f;
        menuItem->CalculateChildrenMenuSize(sizeX, sizeY);

        std::vector<DebugMenuItem*>&& childrenItems = menuItem->GetChildrenMenuItems();
        for (int32_t i = 0; i < childrenItems.size(); i++) {
            if (childrenItems[i] && childrenItems[i]->IsSelected()) {
                render::Render::AddMenuMesh(math::Vector(xOffset, yOffset, 0.0f), math::Vector(xOffset + sizeX, yOffset - childrenItems[i]->GetSizeY(), 0.0f), math::Vector(0.4f, 0.4f, 0.4f, 0.8f));
                break;
            }

            yOffset = yOffset - ScreenSizeToNDCSizeHeight(kMenuItemSizeY);
        }

        menuItem = menuItem->GetChildrenOpenedMenuItem();

        xOffset = xOffset + sizeX + ScreenSizeToNDCSizeWidth(kPaddingXBetweenMenu);
        yOffset = ScreenPosToNDCPosHeight(kMenuStartYPos);
    }
}

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (!s_MgrImp) {
        s_MgrImp = new MgrImp;
        s_MgrImp->Initialize();
    }
}

void Mgr::Update() {
    if (s_MgrImp) {
        s_MgrImp->Update();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::Destroy() {
    if (s_MgrImp) {
        s_MgrImp->Destroy();
        GLB_SAFE_DELETE(s_MgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::AddMenuItem(const char* menu, bool& trigger) {
    if (s_MgrImp) {
        s_MgrImp->AddMenuItem(menu, trigger);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::AddMenuItem(const char* menu, int32_t& value, int32_t min, int32_t max) {
    if (s_MgrImp) {
        s_MgrImp->AddMenuItem(menu, value, min, max);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::AddMenuItem(const char* menu, float& value, float step, float min, float max, const char* format) {
    if (s_MgrImp) {
        s_MgrImp->AddMenuItem(menu, value, step, min, max, format);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

};  // namespace debugmenu

};  // namespace glb