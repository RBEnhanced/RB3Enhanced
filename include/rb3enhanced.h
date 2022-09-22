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

// File functions - defined in their platform specific C files
extern int RB3E_Mounted;
void RB3E_MountFileSystems();
int RB3E_FileExists(const char *filename);
int RB3E_OpenFile(const char *filename, char readWrite);
int RB3E_FileSize(int file);
int RB3E_ReadFile(int file, int offset, void *buffer, int size);
void RB3E_CloseFile(int file);

// Emulator detection
int RB3E_IsEmulator();