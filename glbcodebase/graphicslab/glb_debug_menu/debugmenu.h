//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 13
// Brief: Debug Menu System
//----------------------------------------------------------------------
#ifndef DEBUG_MENU_H_
#define DEBUG_MENU_H_

#include <stdint.h>

#include <vector>

namespace debugmenu2 {

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
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
public:
    static void Initialize();
    static void Update();
    static void Destroy();

    static void AddMenuItem(const char* menu, bool& trigger);
    static void AddMenuItem(const char* menu, int32_t& value, int32_t min, int32_t max);
    static void AddMenuItem(const char* menu, float& value, float step, float min, float max, const char* format);
    static DebugMenuItem* GetRootMenu();
};

};  // namespace debugmenu2

#endif  // DEBUG_MENU_H_