#ifndef HEADER_297BD1FD68834B09A1FBE55C2A4AAE8B// -*- mode:c++ -*-
#define HEADER_297BD1FD68834B09A1FBE55C2A4AAE8B

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CommandKeymapsUI {
public:
    CommandKeymapsUI();

    void DoImGui();

    bool WantsKeyboardFocus() const;
    bool DidConfigChange() const;
protected:
private:
    bool m_edited=false;
    bool m_wants_keyboard_focus=false;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif