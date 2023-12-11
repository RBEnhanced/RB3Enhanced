/*
    RB3Enhanced - ports.h
    Defines port addresses and platform-specific function definitions.
*/
#ifdef RB3E_XBOX // Rock Band 3 Xbox 360 Title Update 5

// instruction patch addresses
#define PORT_SONGLIMIT 0x82579880               // call to "max_song_count" DataNode::_value
#define PORT_APP_CALL 0x82272e88                // call to App::_ct from main()
#define PORT_SONGBLACKLIST 0x82579098           // call to a function that checks song blacklist
#define PORT_DATAINITFUNCS_TAIL 0x82765980      // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x82270f40         // beq after OptionBool("fast",0) in App::_ct
#define PORT_SYSTEMINIT_BLANK 0x825113a4        // call to a stub function in SystemInit
#define PORT_XL_BYPASS_SG 0x82a88268            // lbz r11, bypassSecureGateway in Quazal::XboxClient::Login2
#define PORT_RCG_POLL_LOGGEDIN 0x824f7178       // check for login status in RockCentralGateway::Poll
#define PORT_AUD_PATCH_CRASH 0x82b8c928         // patch somewhere to prevent Audition Mode crashes - blr
#define PORT_AUD_PATCH_NET1 0x82b8f7fc          // patch somewhere to force Audition Mode to be connected (1) - nop
#define PORT_AUD_PATCH_NET2 0x82b8f80c          // patch somewhere to force Audition Mode to be connected (2) - nop
#define PORT_AUD_PATCH_NET3 0x82b8f814          // patch somewhere to force Audition Mode to be connected (3) - nop
#define PORT_AUD_PATCH_NET4 0x82b8f81c          // patch somewhere to force Audition Mode to be connected (4) - li r3, 0
#define PORT_AUD_PATCH_NET5 0x82562638          // patch somewhere to force Audition Mode to be connected (5) - li r3, 1
#define PORT_AUD_PATCH_REPL 0x82b8e978          // patch to allow Audition to recieve insecure packets from Magma
#define PORT_AUD_PATCH_HDD 0x82515dd4           // patch to make has_hard_drive always return true, probably broken
#define PORT_AUD_PATCH_UNK 0x823f6074           // idk
#define PORT_AUD_HANDLE_MESSAGES 0x82563edc     // part of AuditionMgr::Handle that handles disconnected from Rock Central/XBL/etc. messages
#define PORT_AUD_HANDLE_MSG_JUMP 0x82564048     // address to jump PORT_AUD_HANDLE_MESSAGES to, to allow audition mode without GoCentral
#define PORT_AUD_INVALID_DATA_CHECK 0x825629fc  // check for invalid data in a song submitted to audition mode
#define PORT_XNQOS_PROBE1 0x823ee7f8            // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE2 0x823ee800            // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE3 0x823ee80c            // instruction that checks xnqos probe results
#define PORT_VDP_DISABLE1 0x82b39ba0            // nop over VDP packet sending
#define PORT_VDP_DISABLE2 0x82b39e60            // nop over VDP packet sending
#define PORT_VDP_DISABLE3 0x82b3a5e4            // nop over VDP packet sending
#define PORT_VDP_DISABLE4 0x82b3a5f0            // nop over VDP packet sending
#define PORT_SESSION_MASK_CHECK 0x82652acc      // beq in while loop for instrument mask check
#define PORT_CHARACTER_CLOTHES_CHECK 0x82618120 // check to see if the goal required to select a piece of clothing has been achieved or not
#define PORT_FACE_PAINT_CHECK 0x82614a60        // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x82614a9c            // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x82581634       // check to see if video venues are unlocked
#define PORT_OPTIONSTR_DEFINE 0x82510cc8        // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RENDER_RES_X_PATCH1 0x8273bf20     // instruction patch to force render width
#define PORT_RENDER_RES_X_PATCH2 0x8273bf24     // instruction patch to force render width
#define PORT_RENDER_RES_Y_PATCH1 0x8273bedc     // instruction patch to force render height
#define PORT_RUNLOOP_SPARE 0x822700e0           // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
#define PORT_SONG_ID_EVALUATE 0x827aa7d4        // branch to DataNode::Evaluate in SongMetadata::__ct
// function patch addresses
#define PORT_SETDISKERROR 0x82516320                 // PlatformMgr::SetDiskError
#define PORT_APP_RUN 0x82272e90                      // App::Run
#define PORT_APP_RUNNODEBUG 0x82270080               // App::RunWithoutDebugging
#define PORT_APP_CT 0x82270e68                       // App::_ct
#define PORT_NEWFILE 0x825173e0                      // NewFile
#define PORT_SETTRACKSPEED 0x827dd118                // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x82678C88                 // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x82589c48               // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x82588d80           // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x827c0728                    // Symbol::Symbol
#define PORT_LOCALIZE 0x827c96d8                     // Locale::Localize
#define PORT_ADDGAMEGEM 0x8278e530                   // GameGemList::AddGameGem
#define PORT_WILLBENOSTRUM 0x8278cbb0                // GameGemList::WillBeNoStrum
#define PORT_SETVENUE 0x8257d1c0                     // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_ISUGCPLUS 0x8259e890                    // function that checks song source(?)
#define PORT_KEYSONGUITAR 0x825b50f8                 // function that checks "key_keys_on_guitar"
#define PORT_EXECUTEDTA 0x824f7e50                   // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x823406f8      // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x825c66f8         // BandLabel::SetSongAndArtistName
#define PORT_SETSONGNAMEFROMNODE 0x825c56a0          // BandLabel::SetSongNameFromNode
#define PORT_DATANODEEVALUATE 0x8274ae98             // DataNode::Evaluate
#define PORT_DATAARRAYFINDARRAY 0x8274c5a0           // DataArray::FindArray
#define PORT_DATAARRAYFINDDATA 0x8274c7f0            // DataArray::FindData
#define PORT_HMXFACTORYFUNCAT 0x82359f28             // HmxObjectFactoryFunc::_at
#define PORT_SETADDRESS 0x82aeb888                   // Quazal::InetAddress::SetAddress
#define PORT_XL_USESECURESOCKETS 0x82a8eca8          // Inet::UseSecureSockets
#define PORT_XL_XSESSIONCREATE 0x82a69c90            // XSessionCreate
#define PORT_XL_XSESSIONJOINREMOTE 0x82a69fb0        // XSessionJoinRemote
#define PORT_XL_XSESSIONMODIFY 0x82a69e40            // XSessionModify
#define PORT_XL_XSESSIONSEARCHEX 0x82a6a490          // XSessionSearchEx
#define PORT_RANDOMINT 0x824f2f90                    // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x82b9b880              // GemManager::GetWidgetByName
#define PORT_GETSLOTCOLOR 0x82baa308                 // TrackConfig::GetSlotColor
#define PORT_ARCHIVE_CT 0x82514408                   // Archive::_ct
#define PORT_ARCHIVE_SETLOCATIONHARDDRIVE 0x82512b00 // Archive::SetLocationHardDrive
#define PORT_ARCHIVE_MERGE 0x82513ee8                // Archive::Merge
#define PORT_ARCHIVE_DT 0x82513af8                   // Archive::_dt
#define PORT_FILE_EXISTS 0x825175b0                  // FileExists
#define PORT_QUEUEMESSAGE 0x82628e50                 // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x82510590            // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x82510510          // IsSupportedLanguage
#define PORT_DATAREADFILE 0x8276c700                 // DataReadFile
#define PORT_STAGEKIT_SET_STATE 0x82524DE0           // StageKit::SetState(?) - actual name not known
#define PORT_GETSONGIDFROMSHORTNAME 0x82577140       // BandSongMgr::GetSongIDFromShortname
#define PORT_GETMETADATA 0x827a8e30                  // BandSongMgr::Data (function renamed from the original name to avoid any confusion with Data.h)
#define PORT_GETSONGSHORTNAME 0x8257c498             // MetaPerformer::GetSongShortname(?) - actual name not known
#define PORT_GAME_CT 0x8267bf30                      // Game::__ct
#define PORT_GAME_DT 0x8267b1f0                      // Game::__dt
#define PORT_GETBANDUSERS 0x82683b78                 // BandUserMgr::GetBandUsers
#define PORT_GETBANDUSERFROMSLOT 0x82682b60          // BandUserMgr::GetBandUserFromSlot
#define PORT_GETSONGID 0x827a87f0                    // GetSongID, function used when adding songs to BandSongMgr
#define PORT_BUILDINSTRUMENTSELECTION 0x82668c70     // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x82796d90       // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x82b6aa10      // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_POSTPROC_DOPOST 0x82b89a08              // NgPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x8253EB00      // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x8265bb78        // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x8265b910        // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x8265bde8             // RecentCmp::__ct
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x82dfec08 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x82cc8f60  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x82e05d30          // address of gDataFuncs
#define PORT_THEARCHIVE 0x82cc9c60          // address of TheArchive (main ARK)
#define PORT_THEUI 0x82dfd2b0               // address of TheUI
#define PORT_NULLSYMBOL 0x82c71838          // address of gNullSymbol
#define PORT_THESONGDB 0x82e023f8           // address of TheSongDB
#define PORT_THESONGMGR 0x82dfe7b4          // address of TheSongMgr
#define PORT_THEMETAPERFORMER 0x82dfe954    // address of TheMetaPerformer
#define PORT_THEBANDUSERMGR 0x82e023b8      // address of TheBandUserMgr
#define PORT_THESONGSORTMGR 0x82dfee5c      // pointer to TheSongSortMgr
#define PORT_THEMUSICLIBRARY 0x82dfd3a8     // pointer to TheMusicLibrary
// import function stubs
#define PORT_XEKEYSSETKEY_STUB 0x82c4c47c
#define PORT_XEKEYSAESCBC_STUB 0x82c4c48c
#define PORT_SOCKET_STUB 0x8284da48
#define PORT_XNETXNADDRTOINADDR 0x8284d840
#define PORT_XNETCONNECT 0x8284d890
#define PORT_XNETUNREGISTERKEY 0x8284d830
#define PORT_XNETUNREGISTERINADDR 0x8284d870
#define PORT_XNETREGISTERKEY 0x8284d820
#define PORT_XNETGETTITLEXNADDR 0x8284d968
#define PORT_XNETQOSLOOKUP 0x8284d8f8
#define PORT_XAMUSERGETSIGNININFO 0x82c4be1c
#define PORT_XAMUSERGETSIGNINSTATE 0x82c4bcfc
#define PORT_XAMUSERCHECKPRIVILEGE 0x82c4bd1c

// define logging functions
void DbgPrint(const char *s, ...);
#define RB3E_PRINT DbgPrint

#endif // RB3E_XBOX

#ifdef RB3E_WII // Rock Band 3 Wii

#include <string.h>

// instruction patch addresses
#define PORT_SONGLIMIT 0x801cedac               // call to "max_song_count" DataNode::_value
#define PORT_SONGBLACKLIST 0x801d2148           // call to a function that checks song blacklist
#define PORT_DATAINITFUNCS_TAIL 0x80321b7c      // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x8000e2f0         // beq after OptionBool("fast",0) in App::_ct
#define PORT_STRAPSCREEN_1 0x8000e40c           // branch to CustomSplash::Show in App::_ct
#define PORT_STRAPSCREEN_2 0x8000e41c           // branch to CustomSplash::EndShow in App::_ct
#define PORT_NASWII_HOST 0x807e94a0             // branch to the add header function in the DWCDL login function
#define PORT_CHARACTER_CLOTHES_CHECK 0x801fec58 // check to see if the goal required to select a piece of clothing has been unlocked
#define PORT_FACE_PAINT_CHECK 0x801fd9a8        // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x801fd9c4            // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x80227e34       // check to see if video venues are unlocked
#define PORT_OPTIONSTR_DEFINE 0x8030e418        // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RUNLOOP_SPARE 0x8000f740           // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
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
#define PORT_KEYSONGUITAR 0x80242ab4             // function that checks "key_keys_on_guitar"
#define PORT_HMXFACTORYFUNCAT 0x8031b2f8         // HmxObjectFactoryFunc::_at
#define PORT_WILLBENOSTRUM 0x80463010            // GameGemList::WillBeNoStrum
#define PORT_ADDGAMEGEM 0x80463198               // GameGemList::AddGameGem
#define PORT_SETADDRESS 0x8001bf74               // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x802ddd60                // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x800d59b0          // GemManager::GetWidgetByName
#define PORT_DATANODEEVALUATE 0x80322e9c         // DataNode::Evaluate
#define PORT_GETSLOTCOLOR 0x800e42a4             // TrackConfig::GetSlotColor
#define PORT_USBWIIGETTYPE 0x806c1a3c            // UsbWii::GetType
#define PORT_FILE_EXISTS 0x802fa134              // FileExists
#define PORT_QUEUEMESSAGE 0x80253c50             // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x8030f308        // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x8030f280      // IsSupportedLanguage
#define PORT_DATAREADFILE 0x80319bdc             // DataReadFile
#define PORT_GAME_CT 0x80110f20                  // Game::__ct
#define PORT_GAME_DT 0x80111614                  // Game::__dt
#define PORT_WIINETINIT_DNSLOOKUP 0x8030c3a0     // WiiNetInit::StartDNSLookup
#define PORT_BUILDINSTRUMENTSELECTION 0x802478a8 // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x80247c58   // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x802484a8  // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_POSTPROC_DOPOST 0x806b52b4          // WiiPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x80230d64  // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x8027dd3c    // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x8027dc58    // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x8027dba8         // RecentCmp::__ct
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x808fda68 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x80900870  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x8091a528          // address of gDataFuncs
#define PORT_THEUI 0x808fc398               // address of TheUI
#define PORT_NULLSYMBOL 0x808540e0          // address of gNullSymbol
#define PORT_THEMUSICLIBRARY 0x808fda84     // pointer to TheMusicLibrary
#define PORT_THESONGSORTMGR 0x808ff988      // pointer to TheSongSortMgr
// string pointers
#define PORT_NASWII_AC_URL 0x808e2310
#define PORT_NASWII_PR_URL 0x808e2390

// define logging functions
#define RB3E_PRINT printf

#endif // RB3E_WII

#ifdef RB3E_PS3 // Rock Band 3 1.05 BLUS30463 PS3

#include <string.h>

// instruction patch addresses
#define PORT_SONGLIMIT 0x00e53500                 /* call to "max_song_count" DataNode::_value
                                                    its either this or 0x271314 or 0x281544
                                                  */
#define PORT_SONGBLACKLIST 0x4141414141           // call to a function that checks song blacklist
#define PORT_DATAINITFUNCS_TAIL 0x4141414141      // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x4141414141         // beq after OptionBool("fast",0) in App::_ct
#define PORT_STRAPSCREEN_1 0x4141414141           // branch to CustomSplash::Show in App::_ct
#define PORT_STRAPSCREEN_2 0x4141414141           // branch to CustomSplash::EndShow in App::_ct
#define PORT_NASWII_HOST 0x4141414141             // branch to the add header function in the DWCDL login function
#define PORT_CHARACTER_CLOTHES_CHECK 0x4141414141 // check to see if the goal required to select a piece of clothing has been unlocked
#define PORT_FACE_PAINT_CHECK 0x4141414141        // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x4141414141            // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x4141414141       // check to see if video venues are unlocked
#define PORT_OPTIONSTR_DEFINE 0x4141414141        // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RUNLOOP_SPARE 0x4141414141           // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
// function patch addresses
#define PORT_SETDISKERROR 0x4141414141             // PlatformMgr::SetDiskError
#define PORT_APP_CT 0x4141414141                   // App::_ct
#define PORT_NEWFILE 0x4141414141                  // NewFile
#define PORT_SETTRACKSPEED 0x4141414141            // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x4141414141             // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x4141414141           // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x4141414141       // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x4141414141                // Symbol::Symbol
#define PORT_LOCALIZE 0x4141414141                 // Locale::Localize
#define PORT_SETVENUE 0x4141414141                 // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_EXECUTEDTA 0x4141414141               // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x4141414141  // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x4141414141     // BandLabel::SetSongAndArtistName
#define PORT_KEYSONGUITAR 0x4141414141             // function that checks "key_keys_on_guitar"
#define PORT_HMXFACTORYFUNCAT 0x4141414141         // HmxObjectFactoryFunc::_at
#define PORT_WILLBENOSTRUM 0x4141414141            // GameGemList::WillBeNoStrum
#define PORT_ADDGAMEGEM 0x4141414141               // GameGemList::AddGameGem
#define PORT_SETADDRESS 0x4141414141               // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x4141414141                // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x4141414141          // GemManager::GetWidgetByName
#define PORT_DATANODEEVALUATE 0x4141414141         // DataNode::Evaluate
#define PORT_GETSLOTCOLOR 0x4141414141             // TrackConfig::GetSlotColor
#define PORT_FILE_EXISTS 0x4141414141              // FileExists
#define PORT_QUEUEMESSAGE 0x4141414141             // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x4141414141        // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x4141414141      // IsSupportedLanguage
#define PORT_DATAREADFILE 0x4141414141             // DataReadFile
#define PORT_GAME_CT 0x4141414141                  // Game::__ct
#define PORT_GAME_DT 0x4141414141                  // Game::__dt
#define PORT_BUILDINSTRUMENTSELECTION 0x4141414141 // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x4141414141   // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x4141414141  // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_POSTPROC_DOPOST 0x4141414141          // WiiPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x4141414141  // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x4141414141    // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x4141414141    // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x4141414141         // RecentCmp::__ct
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x4141414141 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x4141414141  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x4141414141          // address of gDataFuncs
#define PORT_THEUI 0x4141414141               // address of TheUI
#define PORT_NULLSYMBOL 0x4141414141          // address of gNullSymbol
#define PORT_THEMUSICLIBRARY 0x4141414141     // pointer to TheMusicLibrary
#define PORT_THESONGSORTMGR 0x4141414141      // pointer to TheSongSortMgr

// define logging functions
#define RB3E_PRINT printf

#endif

#define RB3E_MSG(x, ...) RB3E_PRINT("[RB3E:MSG] " x "\n", __VA_ARGS__)
#ifdef RB3EDEBUG
#define RB3E_DEBUG(x, ...) RB3E_PRINT("[RB3E:DBG] " x "\n", __VA_ARGS__)
#else
#define RB3E_DEBUG(x, ...)
#endif // RB3EDEBUG
