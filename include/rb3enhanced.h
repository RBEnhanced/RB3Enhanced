/*
    RB3Enhanced - rb3enhanced.h
    Initialisation functions and basic patches for the game.
*/

// Applies "patches" to the game - single instruction/address pokes
void ApplyPatches();

// Initialises functions called by RB3Enhanced
void InitialiseFunctions();

// Hooks functions used by RB3Enhanced
void ApplyHooks();

// Startup hook
void StartupHook(void *ThisApp, int argc, char **argv);

// OS functions - defined in their platform specific C files
extern int RB3E_Mounted;
void RB3E_MountFileSystems();
char *RB3E_GetRawfilePath(char *path, int root_allowed);
int RB3E_FileExists(char *filename);
int RB3E_OpenFile(char *filename, char readWrite);
int RB3E_FileSize(int file);
int RB3E_ReadFile(int file, int offset, void *buffer, int size);
void RB3E_CloseFile(int file);
int RB3E_CreateThread(void *address, void *arg, int stack_size);
void RB3E_Sleep(int ms);
int RB3E_RelaunchGame();

// loaded song count - done in SongHooks.c
extern int RB3E_LoadedSongCount;

// Emulator detection
int RB3E_IsEmulator();

// StageKit set state, needed for fixing LED bug.
#ifdef RB3E_XBOX
  void StagekitSetStateHook(int state1, int state2);
#endif
