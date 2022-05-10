/*
    RB3Enhanced - ports.h
    Defines port addresses and platform-specific function definitions.
*/
#ifdef RB3E_XBOX // Rock Band 3 Xbox 360 Title Update 5

// instruction patch addresses
#define PORT_SONGLIMIT 0x82579880          // call to "max_song_count" DataNode::_value
#define PORT_APP_CALL 0x82272e88           // call to App::_ct from main()
#define PORT_SONGBLACKLIST 0x82579098      // call to a function that checks song blacklist
#define PORT_DATAINITFUNCS_TAIL 0x82765980 // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x82270f40    // beq after OptionBool("fast",0) in App::_ct
#define PORT_SYSTEMINIT_BLANK 0x825113a4   // call to a stub function in SystemInit
#define PORT_XL_BYPASS_SG 0x82a88268       // lbz r11, bypassSecureGateway in Quazal::XboxClient::Login2
#define PORT_RCG_POLL_LOGGEDIN 0x824f7178  // check for login status in RockCentralGateway::Poll
#define PORT_AUD_PATCH_CRASH 0x82b8c928    // patch somewhere to prevent Audition Mode crashes - blr
#define PORT_AUD_PATCH_NET1 0x82b8f7fc     // patch somewhere to force Audition Mode to be connected (1) - nop
#define PORT_AUD_PATCH_NET2 0x82b8f80c     // patch somewhere to force Audition Mode to be connected (2) - nop
#define PORT_AUD_PATCH_NET3 0x82b8f814     // patch somewhere to force Audition Mode to be connected (3) - nop
#define PORT_AUD_PATCH_NET4 0x82b8f81c     // patch somewhere to force Audition Mode to be connected (4) - li r3, 0
#define PORT_AUD_PATCH_NET5 0x82562638     // patch somewhere to force Audition Mode to be connected (5) - li r3, 1
#define PORT_AUD_PATCH_REPL 0x82b8e978     // patch to allow Audition to recieve insecure packets from Magma
#define PORT_AUD_PATCH_HDD 0x82515dd4      // patch to make has_hard_drive always return true, probably broken
#define PORT_AUD_PATCH_UNK 0x823f6074      // idk
#define PORT_XNQOS_PROBE1 0x823ee7f8       // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE2 0x823ee800       // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE3 0x823ee80c       // instruction that checks xnqos probe results
#define PORT_SESSION_MASK_CHECK 0x82652acc // beq in while loop for instrument mask check
// function patch addresses
#define PORT_SETDISKERROR 0x82516320            // PlatformMgr::SetDiskError
#define PORT_APP_RUN 0x82272e90                 // App::Run
#define PORT_APP_RUNNODEBUG 0x82270080          // App::RunWithoutDebugging
#define PORT_APP_CT 0x82270e68                  // App::_ct
#define PORT_NEWFILE 0x825173e0                 // NewFile
#define PORT_SETTRACKSPEED 0x827dd118           // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x82678C88            // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x82589c48          // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x82588d80      // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x827c0728               // Symbol::Symbol
#define PORT_LOCALIZE 0x827c96d8                // Locale::Localize
#define PORT_ADDGAMEGEM 0x8278e530              // GameGemList::AddGameGem
#define PORT_WILLBENOSTRUM 0x8278cbb0           // GameGemList::WillBeNoStrum
#define PORT_SETVENUE 0x8257d1c0                // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_ISUGCPLUS 0x8259e890               // function that checks song source(?)
#define PORT_KEYSONGUITAR 0x825b50f8            // function that checks "key_keys_on_guitar"
#define PORT_EXECUTEDTA 0x824f7e50              // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x823406f8 // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x825c66f8    // BandLabel::SetSongAndArtistName
#define PORT_DATANODEEVALUATE 0x8274ae98        // DataNode::Evaluate
#define PORT_HMXFACTORYFUNCAT 0x82359f28        // HmxObjectFactoryFunc::_at
#define PORT_SETADDRESS 0x82aeb888              // Quazal::InetAddress::SetAddress
#define PORT_XL_USESECURESOCKETS 0x82a8eca8     // Inet::UseSecureSockets
#define PORT_XL_XSESSIONCREATE 0x82a69c90       // XSessionCreate
#define PORT_XL_XSESSIONJOINREMOTE 0x82a69fb0   // XSessionJoinRemote
#define PORT_XL_XSESSIONMODIFY 0x82a69e40       // XSessionModify
#define PORT_XL_XSESSIONSEARCHEX 0x82a6a490     // XSessionSearchEx
#define PORT_RANDOMINT 0x824f2f90               // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x82b9b880         // GemManager::GetWidgetByName
#define PORT_GETSLOTCOLOR 0x82baa308            // TrackConfig::GetSlotColor
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x82dfec08 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x82cc8f60  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x82e05d30          // address of gDataFuncs
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
#define PORT_SONGLIMIT 0x801cedac          // call to "max_song_count" DataNode::_value
#define PORT_SONGBLACKLIST 0x801d2148      // call to a function that checks song blacklist
#define PORT_DATAINITFUNCS_TAIL 0x80321b7c // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x8000e2f0    // beq after OptionBool("fast",0) in App::_ct
#define PORT_STRAPSCREEN_1 0x8000e40c      // branch to CustomSplash::Show in App::_ct
#define PORT_STRAPSCREEN_2 0x8000e41c      // branch to CustomSplash::EndShow in App::_ct
#define PORT_NASWII_HOST 0x807e94a0        // branch to the add header function in the DWCDL login function
// function patch addresses
#define PORT_SETDISKERROR 0x8030ce7c            // PlatformMgr::SetDiskError
#define PORT_NEWFILE 0x802f9ed0                 // NewFile
#define PORT_SETTRACKSPEED 0x80441ee0           // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x801130d0            // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x8022c1b4          // ModifierManager::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x8022c830      // ModifierManager::ModifierActive
#define PORT_SYMBOL_CT 0x80363f60               // Symbol::Symbol
#define PORT_LOCALIZE 0x803506f4                // Locale::Localize
#define PORT_SETVENUE 0x802282dc                // MetaPerformer::SetVenue(?) (actual func name is not known)
#define PORT_EXECUTEDTA 0x802cf7e0              // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x803b1858 // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x801b68a8    // BandLabel::SetSongAndArtistName
#define PORT_KEYSONGUITAR 0x80242ab4            // function that checks "key_keys_on_guitar"
#define PORT_HMXFACTORYFUNCAT 0x8031b2f8        // HmxObjectFactoryFunc::_at
#define PORT_WILLBENOSTRUM 0x80463010           // GameGemList::WillBeNoStrum
#define PORT_ADDGAMEGEM 0x80463198              // GameGemList::AddGameGem
#define PORT_SETADDRESS 0x8001bf74              // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x802ddd60               // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x800d59b0         // GemManager::GetWidgetByName
#define PORT_DATANODEEVALUATE 0x80322e9c        // DataNode::Evaluate
#define PORT_GETSLOTCOLOR 0x800e42a4            // TrackConfig::GetSlotColor
#define PORT_USBWIIGETTYPE 0x806c1a3c           // UsbWii::GetType
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x808fda68 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x80900870  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x8091a528          // address of gDataFuncs
// string pointers
#define PORT_NASWII_AC_URL 0x808e2310
#define PORT_NASWII_PR_URL 0x808e2390

// define logging functions
#define RB3E_PRINT printf

#endif // RB3E_WII

#define RB3E_MSG(x, ...) RB3E_PRINT("[RB3E:MSG] " x "\n", __VA_ARGS__)
#ifdef RB3EDEBUG
#define RB3E_DEBUG(x, ...) RB3E_PRINT("[RB3E:DBG] " x "\n", __VA_ARGS__)
#else
#define RB3E_DEBUG(x, ...)
#endif // RB3EDEBUG