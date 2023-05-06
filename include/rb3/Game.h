#ifndef _GAME_H
#define _GAME_H

extern void SetMusicSpeed(void *thisGame, float speed);
extern void *GameConstruct(void *thisGame);
extern void *GameDestruct(void *thisGame, int r4);
extern int *GameGetActivePlayer(void *thisGame, int playerIndex);

#endif // _GAME_H