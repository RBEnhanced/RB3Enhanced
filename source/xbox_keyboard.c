#ifdef RB3E_XBOX
#include <xtl.h>
#include "rb3/Data.h"
#include "ports.h"

// Reimplementation of DC3's TranslateVK
// This function translates the system XInput VirtualKey value into Harmonix's keycode values
int TranslateVK(DWORD virtualKey, char shift)
{
    // ASCII range
    // Honestly not sure why this range is even mapped, at these ranges the Unicode translation should be handling it
    if (virtualKey >= 0x41 && virtualKey <= 0x5a) {
        if (!shift) {
            return virtualKey;
        } else {
            return tolower(virtualKey);
        }
    }

    // Function key range
    if (virtualKey >= VK_F1 && virtualKey <= VK_F12) {
        return virtualKey + 0x121;
    }

    // Other keycode mappings
    switch (virtualKey) {
    case VK_HOME:
        return 0x138;
    case VK_CAPITAL:
        return 0x122;
    case VK_BACK:
        return 0x08;
    case VK_TAB:
        return 0x09;
    case VK_RETURN:
        return 0x0a;
    case VK_PAUSE:
        return 0x12d;
    case VK_ESCAPE:
        return 0x12e;
    case VK_PRIOR:
        return 0x13a;
    case VK_NEXT:
        return 0x13b;
    case VK_END:
        return 0x139;
    case VK_PRINT:
        return 0x12c;
    case VK_LEFT:
        return 0x140;
    case VK_UP:
        return 0x142;
    case VK_RIGHT:
        return 0x141;
    case VK_DOWN:
        return 0x143;
    case VK_INSERT:
        return 0x136;
    case VK_DELETE:
        return 0x137;
    case VK_NUMLOCK:
        return 0x123;
    case VK_SCROLL:
        return 0x124;
    }

    return virtualKey;
}


// Reimplementation of KeyboardPoll from DC3
void KeyboardPoll() {
    XINPUT_KEYSTROKE keystroke;
    DWORD keystrokeResult;
    static DataNode msgNodes[6] = {0};
    static DataArray keyMsg = {0};
    static DataNode retNode;
    Symbol uiSym;
    Symbol keySym;
    char unicodeChar[4];
    int keyCode;
    
    keystrokeResult = XInputGetKeystroke(XUSER_INDEX_ANY, 2, &keystroke);
    if (keystrokeResult == ERROR_SUCCESS && (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP) == 0) {
        WideCharToMultiByte(0, 0, &keystroke.Unicode, 1, unicodeChar, 2, 0x0, 0x0);
        if (unicodeChar[0] == '\0' || unicodeChar[0] < ' ' || unicodeChar[0] > '~') {
            keyCode = TranslateVK(keystroke.VirtualKey, (keystroke.Flags & XINPUT_KEYSTROKE_SHIFT) > 0);
        } else {
            keyCode = (int)unicodeChar[0];
        }

        keyMsg.mNodes = (DataNodes *)&msgNodes;
        keyMsg.mNodeCount = 6;
        keyMsg.mRefCount = 1;
        keyMsg.mFile = *(Symbol *)PORT_NULLSYMBOL;

        SymbolConstruct(&uiSym, "ui");
        SymbolConstruct(&keySym, "key");

        // Construct a command array to do this:
        // {ui key $key $shift $ctrl $alt}

        // ui object
        msgNodes[0].type = SYMBOL;
        msgNodes[0].value.string = uiSym.sym;

        // key handler
        msgNodes[1].type = SYMBOL;
        msgNodes[1].value.string = keySym.sym;

        // key
        msgNodes[2].type = INT_VALUE;
        msgNodes[2].value.intVal = keyCode;

        // shift
        msgNodes[3].type = INT_VALUE;
        msgNodes[3].value.intVal = (keystroke.Flags & XINPUT_KEYSTROKE_SHIFT) > 0;

        // ctrl
        msgNodes[4].type = INT_VALUE;
        msgNodes[4].value.intVal = (keystroke.Flags & XINPUT_KEYSTROKE_CTRL) > 0;

        // alt
        msgNodes[5].type = INT_VALUE;
        msgNodes[5].value.intVal = (keystroke.Flags & XINPUT_KEYSTROKE_ALT) > 0;
        
        DataArrayExecute(&retNode, &keyMsg);

    }
}
#endif