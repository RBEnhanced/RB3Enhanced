/*
    RB3Enhanced - ports_wii.h
    Defines port addresses and platform-specific function definitions (for Wii retail)
*/

#ifdef RB3E_WII // Rock Band 3 Wii
#ifndef RB3E_WII_BANK8

#include <string.h>

// instruction patch addresses
#define PORT_SONGLIMIT 0x801cedac                // call to "max_song_count" DataNode::_value
#define PORT_SONGBLACKLIST 0x801d2148            // call to BandSongMgr::IsInExclusionList
#define PORT_DATAINITFUNCS_TAIL 0x80321b7c       // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x8000e2f0          // beq after OptionBool("fast",0) in App::_ct
#define PORT_STRAPSCREEN_1 0x8000e40c            // branch to CustomSplash::Show in App::_ct
#define PORT_STRAPSCREEN_2 0x8000e41c            // branch to CustomSplash::EndShow in App::_ct
#define PORT_NASWII_HOST 0x807e94a0              // branch to the add header function in the DWCDL login function
#define PORT_CHARACTER_CLOTHES_CHECK 0x802607bc  // check to see if the goal required to select a piece of clothing has been unlocked
#define PORT_CHARACTER_CLOTHES_CHECK2 0x802607c0 // check to see if the goal required to select a piece of clothing has been unlocked 2
#define PORT_FACE_PAINT_CHECK 0x801fd9a8         // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x801fd9c4             // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x80227e34        // check to see if video venues are unlocked
#define PORT_OPTIONSTR_DEFINE 0x8030e418         // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RUNLOOP_SPARE 0x8000f740            // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
#define PORT_MICCHECK 0x8024a4e8                 // a bne that throws an error on the song select screen if the mic is not connected
#define PORT_BIGSYMBOLFUNC_TAIL 0x8037a3d4       // blr after a function that initialises a bunch of symbols
#define PORT_UPDATEPRESENCEBLOCK_B 0x80188194    // branch after the failure case in a function that calls UpdatePresence
#define PORT_MULTIPLAYER_CRASH 0x80018a78        // branch to a function that can crash in online multiplayer
#define PORT_MULTIPLAYER_FIX 0x806ec0e8          // the function that doesn't crash
#define PORT_ADDTRACKVECTOR_BL 0x80480a88       // bl to vector_push_back inside of SongData::AddTrack
// #define PORT_LOADOBJS_BCTRL 0x827562e4
// function patch addresses
#define PORT_SETDISKERROR 0x8030ce7c             // PlatformMgr::SetDiskError
#define PORT_NEWFILE 0x802f9ed0                  // NewFile
#define PORT_SETTRACKSPEED 0x80441ee0            // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x801130d0             // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x8022c1b4           // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x8022c830       // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x80363f60                // Symbol::Symbol
#define PORT_LOCALIZE 0x803506f4                 // Locale::Localize
#define PORT_SETVENUE 0x802282dc                 // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_EXECUTEDTA 0x802cf7e0               // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x803b1858  // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x801b68a8     // BandLabel::SetSongAndArtistName
#define PORT_SETSONGNAMEFROMNODE 0x801b6358               // BandLabel::SetSongNameFromNode
#define PORT_KEYSONGUITAR 0x80242ab4             // function that checks "key_keys_on_guitar"
#define PORT_HMXFACTORYFUNCAT 0x8031b2f8         // HmxObjectFactoryFunc::_at
#define PORT_WILLBENOSTRUM 0x80463010            // GameGemList::WillBeNoStrum
#define PORT_ADDGAMEGEM 0x80463198               // GameGemList::AddGameGem
#define PORT_SETADDRESS 0x8001bf74               // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x802ddd60                // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x800d59b0          // GemManager::GetWidgetByName
#define PORT_DATANODEEVALUATE 0x80322e9c         // DataNode::Evaluate
#define PORT_GETSLOTCOLOR 0x800e42a4             // TrackConfig::GetSlotColor
#define PORT_ADDSMASHERPLATETOVECTOR 0x804316d4           // AddSmasherPlateToVector
#define PORT_USBWIIGETTYPE 0x806c1a3c            // UsbWii::GetType
#define PORT_FILE_EXISTS 0x802fa134              // FileExists
#define PORT_QUEUEMESSAGE 0x80253c50             // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x8030f308        // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x8030f280      // IsSupportedLanguage
#define PORT_DATAREADFILE 0x80319bdc             // DataReadFile
#define PORT_GAME_CT 0x80110f20                  // Game::__ct
#define PORT_GAME_DT 0x80111614                  // Game::__dt
#define PORT_GAMEGETACTIVEPLAYER 0x8011346c      // Game::GetActivePlayer
#define PORT_WIINETINIT_DNSLOOKUP 0x8030c3a0     // WiiNetInit::StartDNSLookup
#define PORT_BUILDINSTRUMENTSELECTION 0x802478a8 // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x80247c58   // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x802484a8  // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_VECTORPUSHBACK 0x800a6ef4                    // vector_push_back
#define PORT_POSTPROC_DOPOST 0x806b52b4          // WiiPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x80230d64  // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x8027dd3c    // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x8027dc58    // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x8027dba8         // RecentCmp::__ct
#define PORT_FILESTREAM_CT 0x8034c9f8            // FileStream::__ct (the one that takes a char * path instead of a File object)
#define PORT_CHUNKSTREAM_CT 0x8034aa90           // ChunkStream::__ct
#define PORT_GETBANDUSERFROMSLOT 0x8010021c      // BandUserMgr::GetBandUserFromSlot
#define PORT_OVERSHELLPARTSELECTPROVIDERRELOAD 0x802478a8 // OvershellPartSelectProvider::Reload
#define PORT_GETBANDUSERS 0x80100558             // BandUserMgr::GetBandUsers
#define PORT_GETSONGSHORTNAME 0x80224edc         // MetaPerformer::GetSongShortname(?) - actual name not known
#define PORT_GETMETADATA 0x80515510              // BandSongMgr::Data (function renamed from the original name to avoid any confusion with Data.h)
#define PORT_GETSONGID 0x8051513c                // GetSongID, function used when adding songs to BandSongMgr
#define PORT_SONGMGRGETRANKEDSONGS 0x801d1590    // BandSongMgr::GetRankedSongs(?) - not sure on the real name of the function
#define PORT_GETSONGIDFROMSHORTNAME 0x801d0b44   // BandSongMgr::GetSongIDFromShortname
#define PORT_RNDPROPANIMSETFRAME 0x80632790      // RndPropAnim::SetFrame
#define PORT_DYNAMICCAST 0x806f5e78              // dynamic_cast
#define PORT_OBJECTFINDUIPANEL 0x80101d74        // Object::Find<UIPanel>
#define PORT_JOYPADGETPADDATA 0x80302eec         // JoypadGetPadData
#define PORT_MEMALLOC 0x80353e18                 // MemAlloc
#define PORT_MEMFREE 0x80354238                  // MemFree
#define PORT_SYMBOLPREINIT 0x80364c74            // Symbol::PreInit
#define PORT_QUEUINGSOCKET_BIND 0x800478d4       // Quazal::QueuingSocket::Bind
#define PORT_QUAZALSOCKET_BIND 0x8001cd10        // Quazal::Socket::Bind
#define PORT_INITSONGMETADATA 0x805147a4         // InitSongMetadata
#define PORT_SONGMETADATACONSTRUCTOR 0x80514880           // SongMetadata::__ct
#define PORT_SONGMETADATALOAD 0x801d2090                  // SongMetadata::Load
#define PORT_UPDATEPRESENCE 0x801879d4           // PresenceMgr::UpdatePresence
#define PORT_STEPSEQUENCEJOBSETSTEP 0x80025364   // Quazal::StepSequenceJob::SetStep
#define PORT_RNDTEXNEWOBJECT 0x80639904                   // RndTex::NewObject
#define PORT_RNDMATNEWOBJECT 0x8063996c                   // RndMat::NewObject
#define PORT_RNDTEXSETBITMAP 0x8063fccc                   // RndTex::SetBitmap
#define PORT_RNDTEXSETBITMAP2 0x8063f830                  // RndTex::SetBitmap2
#define PORT_RNDTEXSETBITMAP3 0x8063fb2c                  // RndTex::SetBitmap3
#define PORT_FILEPATHCONSTRUCTOR 0x8000ec5c               // FilePath::__ct
#define PORT_MUSICLIBRARY_CT 0x8022d978                   // MusicLibrary::__ct
#define PORT_MUSICLIBRARYMAT 0x80231c5c                   // MusicLibrary::Mat
#define PORT_NODESORTGETNODE 0x80279314                   // MusicLibrary::GetNodeByIndex
#define PORT_GAMEGEMDB_CT 0x80460f64                      // GameGemDB::__ct
#define PORT_ADDMULTIGEM 0x80461160                       // GameGemDB::AddMultiGem
#define PORT_GETGAMELIST 0x8048553c                       // SongData::GetGameList
#define PORT_SONGSORTMGRGETSORT 0x80281b20                // SongSortMgr::GetSort
#define PORT_RNDMATSETDIFFUSETEX 0x8025ab90               // RndMat::SetDiffuseTex
#define PORT_DYNAMICTEX_CT 0x80292a70                     // DynamicTex::__ct
#define PORT_BINSTREAMWRITE 0x80342f48           // BinStream::Write
#define PORT_BINSTREAMREAD 0x80342e7c            // BinStream::Read
#define PORT_BINSTREAMREADENDIAN 0x803430bc      // BinStream::ReadEndian
#define PORT_BINSTREAMWRITEENDIAN 0x80343190     // BinStream::WriteEndian
#define PORT_SONGPARSERPITCHTOSLOT 0x8048e298    // SongParser::PitchToSlot
#define PORT_DATASET 0x8031b86c                  // DataSetSet
#define PORT_DATASETELEM 0x8031e9a0              // DataSetElem
#define PORT_DATAONELEM 0x8031dc40               // DataOnElem
#define PORT_HEAPINIT 0x80352cbc                 // Heap::Init
#define PORT_DATAREGISTERFUNC 0x8031b2b8         // DataRegisterFunc
#define PORT_FILEISLOCAL 0x802fb548              // FileIsLocal
#define PORT_FILEISDLC 0x802fb54c                // FileIsDLC
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x808fda68 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x80900870  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x8091a528          // address of gDataFuncs
#define PORT_THEARCHIVE 0x80902234          // address of TheArchive (main ARK)
#define PORT_THEBANDUI 0x808fc2b0           // address of TheBandUI
#define PORT_NULLSYMBOL 0x808540e0          // address of gNullSymbol
#define PORT_THESONGDB 0x808fb170           // address of TheSongDB - TODO: check
#define PORT_THEMUSICLIBRARY 0x808fda84     // pointer to TheMusicLibrary
#define PORT_THESONGSORTMGR 0x808ff988      // pointer to TheSongSortMgr
#define PORT_THESONGMGR 0x808fbda4          // address of TheSongMgr
#define PORT_THEMETAPERFORMER 0x808fd6f8    // address of TheMetaPerformer
#define PORT_THEBANDUSERMGR 0x808f9350      // pointer to TheBandUserMgr
#define PORT_THEGAME 0x808f9758             // pointer to TheGame (you lost)
#define PORT_OBJECTDIRMAINDIR 0x8091b1e8    // ObjectDir::sMainDir
// string pointers
#define PORT_NASWII_AC_URL 0x808e2310
#define PORT_NASWII_PR_URL 0x808e2390
// wii stuff
#define PORT_WII_OS0_GLOBALS 0x80000000              // "os0"/"early" globals
#define PORT_WII_OS1_GLOBALS 0x80003000              // "os1"/"late" globals
#define PORT_CONFIGMEM2_52MB_INST 0x80769340         // ConfigMEM2_52MB 0x90000000 length param
#define PORT_CONFIGMEM2_56MB_INST 0x80769420         // ConfigMEM2_56MB 0x90000000 length param
#define PORT_CONFIGMEM2_64MB_INST 0x80769500         // ConfigMEM2_64MB 0x90000000 length param
#define PORT_ENABLEINSTSONMEM2LO16MB_INST 0x80769740 // EnableInstsOnMEM2Lo16MB 0x90000000 length param
#define PORT_VISETMODE_LI_28 0x8077b2ac              // "li r28, 0x1" in VISetRevolutionModeSimple
#define PORT_VISETMODE_STB_28 0x8077b2b4             // "stb r28, 0x21(r1)" in VISetRevolutionModeSimple
#define PORT_VISETMODE_PATCH_CODE 0x800027a0         // stub area to insert the 480p fix code for VISetRevolutionModeSimple
#define PORT_GXSETCOPYFILTER_BEQ 0x807464b0          // "beq 0x40" in GXSetCopyFilter
#define PORT_OSFATAL_HALT_X_OFFSET 0x807673fc        // 'li r7, 48' for the X offset in Halt (OSFatal)
#define PORT_OSFATAL_HALT_Y_OFFSET 0x80767404        // 'li r8, 100' for the Y offset in Halt (OSFatal)
#define PORT_SCREENREPORT_X_NEWLINE 0x80766894       // 'subi rX, rX, 48' for the X newline cutoff in ScreenReport
#define PORT_OSFATAL_HALT_OSREPORT 0x80767460        // call to OSReport in Halt (OSFatal)
#define PORT_OSFATAL_HALT_PPCHALT 0x80767464         // call to PPCHalt in Halt (OSFatal)
#define PORT_OSFATAL 0x80766cb0                      // OSFatal
#define PORT_OSSETERRORHANDLER 0x80764b80            // OSSetErrorHandler
#define PORT_PPCHALT 0x80706390                      // PPCHalt
#define PORT_OSRETURNTOMENU 0x8076a610               // OSReturnToMenu
#define PORT_OSREADROM 0x8076ae20                    // OSReadROM

// define logging functions
#define RB3E_PRINT printf

#endif // RB3E_WII_BANK8
#endif // RB3E_WII
