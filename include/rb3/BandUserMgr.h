#ifndef _BANDUSERMGR_H
#define _BANDUSERMGR_H

#include "rb3/BandUser.h"

// gives you the number of band users in the current band
extern int GetBandUsers(int thisBandUserMgr, int pUsers, int flags);
// returns the BandUser in a particular overshell slot (overshell slots are left to right, 0-3, so vocals for example is nearly always going to be 2)
extern BandUser *GetBandUserFromSlot(int thisBandUserMgr, int slot);

#endif // _BANDUSERMGR_H