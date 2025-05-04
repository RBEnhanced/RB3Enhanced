/*
    RB3Enhanced - ports_ps3.h
    Defines port addresses and platform-specific function definitions (for PS3 1.06 EUR)
*/

#ifdef RB3E_PS3 // Rock Band 3 PS3 BLES00986 1.06

// instruction patch addresses
#define PORT_SONGLIMIT 0x00281580               // call to "max_song_count" DataNode::_value
#define PORT_APP_CALL 0x00017384                // call to App::_ct from main()
#define PORT_SONGBLACKLIST 0x00280BE4           // call to BandSongMgr::IsInExclusionList
#define PORT_DATAINITFUNCS_TAIL 0x008F1D48      // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x00012744         // beq after OptionBool("fast",0) in App::_ct
#define PORT_SYSTEMINIT_BLANK 0x008BFBD0        // call to a stub function in SystemInit
#define PORT_CHARACTER_CLOTHES_CHECK 0x002A92DC // check to see if the goal required to select a piece of clothing has been achieved or not
#define PORT_FACE_PAINT_CHECK 0x002A4C60        // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x002A4CA0            // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x002D06A8       // check to see if video venues are unlocked
#define PORT_OPTIONSTR_DEFINE 0x008C005C        // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RUNLOOP_SPARE 0x00010960           // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
#define PORT_SONG_ID_EVALUATE 0x0063E6B8        // branch to DataNode::Evaluate in SongMetadata::__ct
// #define PORT_LOADOBJS_BCTRL 0                   // bctrl to Object::PreLoad insie of DirLoader::LoadObjs
#define PORT_STAGEKIT_EXISTS 0x00442FAC               // StageKit check. nop over to allow for fog command without a physical StageKit connected.
#define PORT_PS3_VORBISREADER_CTR_CHECK 0x00883F40    // check in the SPU VorbisReader decoder that checks if there is a CTR state
#define PORT_NPDRM_UNAVAILABLE_FALLTHROUGH 0x008C8BD0 // fallthrough after sceNpDrmIsAvailable (r == CELL_OK) beq instruction
#define PORT_NPDRM_FSOPEN_JUMP 0x008C8C6C             // statement that runs after sceNpDrmIsAvailable returns CELL_OK (starts "li r0, 2")
#define PORT_NPDRM_FSOPEN_RETURN 0x008C8C80           // branch to a "extsw r4, r9" after the npdrm cellFsOpen completes
#define PORT_NPDRM_SETERRORCODE_JUMP 0x008C8C5C       // "extsw r4, r3" after the call to cellFsOpen for unencrypted midi files
// function patch addresses
#define PORT_SETDISKERROR 0x008CB888       // PlatformMgr::SetDiskError
#define PORT_APP_RUN 0x000108C8            // App::Run
#define PORT_APP_CT 0x000125CC             // App::_ct
#define PORT_NEWFILE 0x008AB024            // NewFile
#define PORT_SETTRACKSPEED 0x0051A3D8      // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x0017BBE0       // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x002D7718     // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x002D5B64 // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x00956C00          // Symbol::Symbol
#define PORT_LOCALIZE 0x00937554           // Locale::Localize
// #define PORT_ADDGAMEGEM 0                            // GameGemList::AddGameGem
// #define PORT_WILLBENOSTRUM 0                         // GameGemList::WillBeNoStrum
#define PORT_SETVENUE 0x002CC1E0                     // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_ISUGCPLUS 0x00276470                    // function that checks song source(?)
#define PORT_KEYSONGUITAR 0x0030028C                 // function that checks "key_keys_on_guitar"
#define PORT_EXECUTEDTA 0x00416A58                   // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x00465D78      // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x00265A44         // BandLabel::SetSongAndArtistName
#define PORT_SETSONGNAMEFROMNODE 0x00263C28          // BandLabel::SetSongNameFromNode
#define PORT_DATANODEEVALUATE 0x008FBE24             // DataNode::Evaluate
#define PORT_DATAARRAYFINDARRAY 0x008E95B4           // DataArray::FindArray
#define PORT_DATAARRAYFINDDATA 0x008E9D14            // DataArray::FindData
#define PORT_ADDDTAFUNCTIONPS3 0x008F0AC0            // unknown name function, adds DTA function to gDataFuncs, PS3 only
#define PORT_SETADDRESS 0x000D12B4                   // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x0096F5D4                    // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x009CC848              // GemManager::GetWidgetByName
#define PORT_GETSLOTCOLOR 0x009DC30C                 // TrackConfig::GetSlotColor
#define PORT_ARCHIVE_CT 0x008A0934                   // Archive::_ct
#define PORT_ARCHIVE_SETLOCATIONHARDDRIVE 0x0089E46C // Archive::SetLocationHardDrive
#define PORT_ARCHIVE_MERGE 0x008A0CB4                // Archive::Merge
// #define PORT_ARCHIVE_DT 0                            // Archive::_dt
#define PORT_FILE_EXISTS 0x008AB2AC         // FileExists
#define PORT_QUEUEMESSAGE 0x00323570        // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x008BEDB0   // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x008BE934 // IsSupportedLanguage
#define PORT_DATAREADFILE 0x008EF84C        // DataReadFile
#define PORT_STAGEKIT_SET_STATE 0x008B2FBC  // StageKit::SetState(?) - actual name not known
// #define PORT_GETSONGIDFROMSHORTNAME 0                // BandSongMgr::GetSongIDFromShortname
// #define PORT_GETMETADATA 0                           // BandSongMgr::Data (function renamed from the original name to avoid any confusion with Data.h)
#define PORT_GETSONGSHORTNAME 0x002C9960         // MetaPerformer::GetSongShortname(?) - actual name not known
#define PORT_GAME_CT 0x00181CE4                  // Game::__ct
#define PORT_GAME_DT 0x0018337C                  // Game::__dt
#define PORT_GAMEGETACTIVEPLAYER 0x0017BB48      // Game::GetActivePlayer
#define PORT_GETBANDUSERS 0x00178E00             // BandUserMgr::GetBandUsers
#define PORT_GETBANDUSERFROMSLOT 0x00176FA0      // BandUserMgr::GetBandUserFromSlot
#define PORT_GETSONGID 0x0063F7A8                // GetSongID, function used when adding songs to BandSongMgr
#define PORT_SONGMGRGETRANKEDSONGS 0x002802B8    // BandSongMgr::GetRankedSongs(?) - not sure on the real name of the function
#define PORT_BUILDINSTRUMENTSELECTION 0x003E4EC4 // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x003E4EAC   // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x003E4E1C  // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_POSTPROC_DOPOST 0x00A04218          // NgPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x002E09A0  // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x003EA7F8    // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x003EA5E8    // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x003EAA9C         // RecentCmp::__ct
#define PORT_FILESTREAM_CT 0x00935764            // FileStream::__ct (the one that takes a char * path instead of a File object)
#define PORT_CHUNKSTREAM_CT 0x0093100C           // ChunkStream::__ct
// #define PORT_RNDPROPANIMSETFRAME 0               // RndPropAnim::SetFrame
#define PORT_DYNAMICCAST 0x00A3BA7C       // dynamic_cast
#define PORT_OBJECTFINDUIPANEL 0x008FFF64 // Object::Find<UIPanel>
#define PORT_JOYPADGETPADDATA 0x008B18C0  // JoypadGetPadData
#define PORT_MEMFREE 0x0093EEC0           // MemFree
#define PORT_MEMALLOC 0x0093FAE0          // MemAlloc
#define PORT_SYMBOLPREINIT 0x00956F68     // Symbol::PreInit
// #define PORT_QUEUINGSOCKET_BIND 0                    // Quazal::QueuingSocket::Bind
// #define PORT_QUAZALSOCKET_BIND 0                     // Quazal::Socket::Bind
// #define PORT_MEMPRINTOVERVIEW 0                      // MemPrintOverview
// #define PORT_MEMPRINT 0                              // MemPrint
// #define PORT_MEMNUMHEAPS 0                           // MemNumHeaps
#define PORT_INITSONGMETADATA 0x0063DAB4 // InitSongMetadata
// #define PORT_UPDATEPRESENCE 0                        // PresenceMgr::UpdatePresence
#define PORT_STEPSEQUENCEJOBSETSTEP 0x000DD0D8 // Quazal::StepSequenceJob::SetStep
#define PORT_CTR_DECRYPT 0x00A1FBF8            // ctr_decrypt
#define PORT_TITLEIDREGISTER 0x008A6224        // TitleIDRegister(?) - function that gets passed title IDs at startup / config parsing
#define PORT_CANPLAYSONG_PARENTAL 0x00391648   // broken function that checks against parental controls whether a song is playable
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x00F2B870 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x00F33948  // address of RockCentralGateway
// #define PORT_GDATAFUNCS 0                   // address of gDataFuncs
// #define PORT_THEARCHIVE 0                   // address of TheArchive (main ARK)
// #define PORT_THEBANDUI 0                    // address of TheBandUI
#define PORT_NULLSYMBOL 0x00F1BEF0 // address of gNullSymbol
// #define PORT_THESONGDB 0                    // address of TheSongDB
// #define PORT_THESONGMGR 0                   // address of TheSongMgr
// #define PORT_THEMETAPERFORMER 0             // address of TheMetaPerformer
#define PORT_THEBANDUSERMGR 0x00F23460 // address of TheBandUserMgr
// #define PORT_THESONGSORTMGR 0               // pointer to TheSongSortMgr
// #define PORT_THEMUSICLIBRARY 0              // pointer to TheMusicLibrary
// #define PORT_THEGAME 0                      // pointer to TheGame (you lost)
// #define PORT_OBJECTDIRMAINDIR 0             // ObjectDir::sMainDir
// ps3 usb specific hooks
#define PORT_LDDREGISTERTAIL 0x008e4240  // the "ld r2" instruction after the final call to cellUsbdRegisterExtraLdd2
#define PORT_LDDADDRESS 0x00f1bf1c       // just one of the LDDs. doesn't matter which. this one is RockBandKeyboard
#define PORT_ISUSBDEVICEVALID 0x008e1074 // function called by the Probe LDD function that checks if a device is valid

// define logging functions
extern int _sys_printf(char *fmt, ...);
#define RB3E_PRINT _sys_printf

// do some definitions of stuff
extern int _sys_sprintf(char *buf, char *fmt, ...);
#define printf _sys_printf
#define sprintf _sys_sprintf

#define PLUGIN_PTR(x) (uint32_t)(*(uint32_t *)&x)

#endif // RB3E_PS3
