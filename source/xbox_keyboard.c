#ifdef RB3E_XBOX
#include <xtl.h>
#include "rb3/Data.h"
#include "ports.h"

// Copy-pasted from DC3 ghidra decompile output
// This function translates the system XInput VirtualKey value into Harmonix's keycode values
int TranslateVK(unsigned short virtualKey, char shift)
{
  unsigned int _C;
  unsigned int uVar1;
  
  _C = (unsigned int)virtualKey;
  if ((((0x2f < _C) && (_C < 0x3a)) || ((uVar1 = 0, 0x40 < _C && (_C < 0x5b)))) &&
     (uVar1 = _C, !shift)) {
    uVar1 = tolower(_C);
  }
  if ((0x6f < _C) && (_C < 0x7c)) {
    uVar1 = _C + 0x121;
  }
  if (virtualKey < 0x25) {
    if (_C == 0x24) {
      uVar1 = 0x138;
    }
    else if (virtualKey < 0x15) {
      if (_C == 0x14) {
        uVar1 = 0x122;
      }
      else if (_C == 8) {
        uVar1 = 8;
      }
      else if (_C == 9) {
        uVar1 = 9;
      }
      else if (_C == 0xd) {
        uVar1 = 10;
      }
      else if (_C == 0x13) {
        uVar1 = 0x12d;
      }
    }
    else if (_C == 0x1b) {
      uVar1 = 0x12e;
    }
    else if (_C == 0x21) {
      uVar1 = 0x13a;
    }
    else if (_C == 0x22) {
      uVar1 = 0x13b;
    }
    else if (_C == 0x23) {
      uVar1 = 0x139;
    }
  }
  else if (virtualKey < 0x2b) {
    if (_C == 0x2a) {
      uVar1 = 300;
    }
    else if (_C == 0x25) {
      uVar1 = 0x140;
    }
    else if (_C == 0x26) {
      uVar1 = 0x142;
    }
    else if (_C == 0x27) {
      uVar1 = 0x141;
    }
    else if (_C == 0x28) {
      uVar1 = 0x143;
    }
  }
  else if (_C == 0x2d) {
    uVar1 = 0x136;
  }
  else if (_C == 0x2e) {
    uVar1 = 0x137;
  }
  else if (_C == 0x90) {
    uVar1 = 0x123;
  }
  else if (_C == 0x91) {
    uVar1 = 0x124;
  }
  return uVar1;
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