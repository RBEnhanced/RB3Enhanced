/*
    RB3Enhanced - ports_wii.h
    Defines port addresses and platform-specific function definitions (for Wii "bank 8"/100901_A)

    Commented out defines are either not needed or not
*/

#ifdef RB3E_WII // Rock Band 3 Wii 2010-09-01 "bank 8" build
#ifdef RB3E_WII_BANK8

#include <string.h>

// instruction patch addresses
#define PORT_SONGLIMIT 0x8026fbf0                // call to "max_song_count" DataNode::_value
#define PORT_SONGBLACKLIST 0x80273298            // call to BandSongMgr::IsInExclusionList
#define PORT_DATAINITFUNCS_TAIL 0x8045d080       // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x8000e464          // beq after OptionBool("fast",0) in App::_ct
#define PORT_STRAPSCREEN_1 0x8000e5a4            // branch to CustomSplash::Show in App::_ct
#define PORT_STRAPSCREEN_2 0x8000e5b4            // branch to CustomSplash::EndShow in App::_ct
#define PORT_NASWII_HOST 0x80b2df90              // branch to the add header function in the DWCDL login function
#define PORT_CHARACTER_CLOTHES_CHECK 0x80348410  // (ProfileAssets::HasAsset) check to see if the goal required to select a piece of clothing has been unlocked
#define PORT_CHARACTER_CLOTHES_CHECK2 0x80348414 // check to see if the goal required to select a piece of clothing has been unlocked 2
#define PORT_FACE_PAINT_CHECK 0x802b58b0         // check to see if face paint is unlocked (key_unlocked_face_paint BandProfile::HasCampaignKey)
#define PORT_TATTOO_CHECK 0x802b58cc             // check to see if tattoos are unlocked (key_unlocked_tattoos BandProfile::HasCampaignKey)
#define PORT_VIDEO_VENUE_CHECK 0x802f0ce8        // check to see if video venues are unlocked (key_video_venues BandProfile::HasCampaignKey)
#define PORT_OPTIONSTR_DEFINE 0x8044219c         // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RUNLOOP_SPARE 0x80010788            // branch to a function that only has a "blr" in App::Run(WithoutDebugging) (branch to UIStats::Poll)
// #define PORT_MICCHECK 0x8024a4e8              // NOT NEEDED? a bne that throws an error on the song select screen if the mic is not connected
#define PORT_BIGSYMBOLFUNC_TAIL 0x804d3bac    // blr after a function that initialises a bunch of symbols
#define PORT_UPDATEPRESENCEBLOCK_B 0x8021cbd0 // branch after the failure case in a function that calls UpdatePresence
// FIX ME INLINED? #define PORT_ADDTRACKVECTOR_BL 0x8064e288       // bl to vector_push_back inside of SongData::AddTrack // actually SongDB::AddTrack
#define PORT_GETGAMELIST 0x801dbfa0             // SongData::GetGameList // actually SongDB::GetGems
#define PORT_MULTIPLAYER_CRASH 0x80023fec     // branch to a function that can crash in online multiplayer
#define PORT_MULTIPLAYER_FIX 0x80a2f0c4       // the function that doesn't crash
// #define PORT_LOADOBJS_BCTRL 0x827562e4
// function patch addresses
#define PORT_SETDISKERROR 0x804400a0            // PlatformMgr::SetDiskError
#define PORT_NEWFILE 0x804204a0                 // NewFile
#define PORT_SETTRACKSPEED 0x805f9060           // TrackPanelDirBase::UpdateTrackSpeed
#define PORT_SETSONGSPEED 0x8017fa80            // Game::SetMusicSpeed
#define PORT_MODIFIERMGR_CT 0x802f7130          // ModifierMgr::__ct
#define PORT_MODIFIERMGR_ACTIVE 0x802f7ab0      // ModifierMgr::GetModifier
#define PORT_SYMBOL_CT 0x804bd1c0               // Symbol::Symbol
#define PORT_LOCALIZE 0x8049b7f0                // Locale::Localize
#define PORT_SETVENUE 0x802f12f0                // MetaPerformer::SetVenue
#define PORT_EXECUTEDTA 0x803edbd0              // RockCentralGateway::ExecuteConfig
#define PORT_BANDLABELSETDISPLAYTEXT 0x80518360 // BandLabel::SetDisplayText
#define PORT_SETSONGANDARTISTNAME 0x80254650    // BandLabel::SetSongAndArtistName
#define PORT_SETSONGNAMEFROMNODE 0x80253ec0     // BandLabel::SetSongNameFromNode
#define PORT_KEYSONGUITAR 0x8031c1b0            // OvershellPanel::CanGuitarPlayKeys
// #define PORT_HMXFACTORYFUNCAT 0x8031b2f8         // FIXME HmxObjectFactoryFunc::_at - inlined in bank8?
#define PORT_WILLBENOSTRUM 0x8062b2d0            // GameGemList::WillBeNoStrum
#define PORT_ADDGAMEGEM 0x8062b460               // GameGemList::AddGameGem
#define PORT_SETADDRESS 0x80028d00               // Quazal::InetAddress::SetAddress
#define PORT_RANDOMINT 0x803fef30                // RandomInt(min, max)
#define PORT_GETWIDGETBYNAME 0x80135f30          // GemManager::GetWidgetByName
#define PORT_DATANODEEVALUATE 0x8045e030         // DataNode::Evaluate
#define PORT_GETSLOTCOLOR 0x801484d0             // TrackConfig::GetSlotColor
// FIX ME INLINED? #define PORT_ADDSMASHERPLATETOVECTOR 0x80bf0828      // AddSmasherPlateToVector
#define PORT_USBWIIGETTYPE 0x809fd840            // UsbWii::GetType
#define PORT_FILE_EXISTS 0x804207c0              // FileExists
#define PORT_QUEUEMESSAGE 0x80333140             // PassiveMessagesPanel::QueueMessage
#define PORT_SETSYSTEMLANGUAGE 0x80443570        // SetSystemLanguage
#define PORT_ISSUPPORTEDLANGUAGE 0x804434e0      // IsSupportedLanguage
#define PORT_DATAREADFILE 0x80452ef0             // DataReadFile
#define PORT_GAME_CT 0x8017d250                  // Game::__ct
#define PORT_GAME_DT 0x8017dbd0                  // Game::__dt
#define PORT_GAMEGETACTIVEPLAYER 0x8017fe40      // Game::GetActivePlayer
#define PORT_WIINETINIT_DNSLOOKUP 0x8043f1c0     // WiiNetInit::StartDNSLookup
// #define PORT_BUILDINSTRUMENTSELECTION 0x802478a8 // FIXME BuildInstrumentSelectionList(?) - actual name not known
// #define PORT_PREPARESOMEVECTORMAYBE 0x80247c58   // FIXME Prepares some vector, used by BuildInstrumentSelectionList
// #define PORT_SOMEVECTORPUSHBACKMAYBE 0x802484a8  // FIXME vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
// FIX ME (?) #define PORT_VECTORPUSHBACK 0x801dced0               // vector_push_back
#define PORT_POSTPROC_DOPOST 0x809ef2d0          // WiiPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x802ff140  // UNSURE MusicLibrary::TryToSetHighlight, Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x80374910    // RecentCmp::RecentTypeToOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x80374870    // RecentCmp::OriginToRecentType
#define PORT_SONGSORTBYRECENT 0x803747d0         // RecentCmp::__ct
#define PORT_FILESTREAM_CT 0x80494fd0            // FileStream::__ct (the one that takes a char * path instead of a File object)
#define PORT_CHUNKSTREAM_CT 0x804929a0           // ChunkStream::__ct
#define PORT_GETBANDUSERFROMSLOT 0x80168010      // BandUserMgr::GetUserFromSlot
#define PORT_OVERSHELLPARTSELECTPROVIDERRELOAD 0x80322620 // OvershellPartSelectProvider::Reload
#define PORT_GETBANDUSERS 0x801683e0             // BandUserMgr::GetBandUsers
#define PORT_GETSONGSHORTNAME 0x802f5dd0         // MetaPerformer::GetSongSymbol
#define PORT_GETMETADATA 0x80271de0              // BandSongMgr::Data (function renamed from the original name to avoid any confusion with Data.h)
#define PORT_GETSONGID 0x8075af40                // DataArray::GetSongID ???, function used when adding songs to BandSongMgr
#define PORT_SONGMGRGETRANKEDSONGS 0x802726a0    // BandSongMgr::GetRankedSongs(?) - not sure on the real name of the function
#define PORT_GETSONGIDFROMSHORTNAME 0x80271ad0   // BandSongMgr::GetSongIDFromShortname
#define PORT_RNDPROPANIMSETFRAME 0x80928020      // RndPropAnim::SetFrame
#define PORT_DYNAMICCAST 0x80a38f54              // dynamic_cast
// #define PORT_OBJECTFINDUIPANEL 0x80101d74      // FIXME Object::Find<UIPanel> - inlined on bank8?
#define PORT_JOYPADGETPADDATA 0x8042f6f0       // JoypadGetPadData
#define PORT_MEMALLOC 0x804a0a70               // MemAlloc
#define PORT_MEMFREE 0x804a12c0                // MemFree
#define PORT_SYMBOLPREINIT 0x804be150          // Symbol::PreInit
#define PORT_QUEUINGSOCKET_BIND 0x80068820     // Quazal::QueuingSocket::Bind
#define PORT_QUAZALSOCKET_BIND 0x80029dc0      // Quazal::Socket::Bind
#define PORT_METAMUSICISLOADED 0x809975d0      // MetaMusic::Loaded
#define PORT_METAMUSICLOAD 0x809970d0          // MetaMusic::Load
#define PORT_METAMUSICPOLL 0x80997610          // MetaMusic::Poll
#define PORT_METAMUSICSTART 0x80997990         // MetaMusic::Start
#define PORT_INITSONGMETADATA 0x8075a0c0       // InitSongMetadata
#define PORT_SONGMETADATACONSTRUCTOR 0x8075a1a0       // SongMetadata::__ct
#define PORT_SONGMETADATALOAD 0x8075a910                  // SongMetadata::Load
#define PORT_UPDATEPRESENCE 0x8021c3d0         // PresenceMgr::UpdatePresence
#define PORT_STEPSEQUENCEJOBSETSTEP 0x80035cf0 // Quazal::StepSequenceJob::SetStep
#define PORT_RNDTEXNEWOBJECT 0x80931050                   // RndTex::NewObject
#define PORT_RNDMATNEWOBJECT 0x80931010                   // RndMat::NewObject
#define PORT_RNDTEXSETBITMAP 0x8093b810                   // RndTex::SetBitmap
#define PORT_RNDTEXSETBITMAP2 0x8093b430                  // RndTex::SetBitmap2
#define PORT_RNDTEXSETBITMAP3 0x8093b5b0                  // RndTex::SetBitmap3
#define PORT_FILEPATHCONSTRUCTOR 0x80462460               // FilePath::__ct
#define PORT_MUSICLIBRARY_CT 0x802f9e10                   // MusicLibrary::__ct
#define PORT_MUSICLIBRARYMAT 0x80300390                   // MusicLibrary::Mat
#define PORT_NODESORTGETNODE 0x8036f5e0                   // MusicLibrary::GetNodeByIndex //actually NodeSort::GetNode
#define PORT_GAMEGEMDB_CT 0x80628230                      // GameGemDB::__ct
#define PORT_ADDMULTIGEM 0x806284e0                       // GameGemDB::AddMultiGem
#define PORT_SONGSORTMGRGETSORT 0x8037a510                // SongSortMgr::GetSort
#define PORT_DYNAMICTEX_CT 0x80393bd0                     // DynamicTex::__ct
#define PORT_DYNAMICTEX_DT 0x80393ef0                     // DynamicTex::__dt
#define PORT_MUSICLIBRARYONENTER 0x802fa750              // MusicLibrary::OnEnter
#define PORT_MUSICLIBRARYONUNLOAD 0x802fb2a0               // MusicLibrary::OnExit
#define PORT_RNDMATSETDIFFUSETEX 0x808d36b0               // RndMat::SetDiffuseTex
#define PORT_BINSTREAMWRITE 0x804892b0         // BinStream::Write
#define PORT_BINSTREAMREAD 0x80489140          // BinStream::Read
#define PORT_BINSTREAMREADENDIAN 0x804894f0    // BinStream::ReadEndian
#define PORT_BINSTREAMWRITEENDIAN 0x80489640   // BinStream::WriteEndian
#define PORT_SONGPARSERPITCHTOSLOT 0x8065d3d0  // SongParser::PitchToSlot
#define PORT_DATASET 0x80454b00                // DataSet
#define PORT_DATASETELEM 0x80459240            // DataSetElem
#define PORT_DATAONELEM 0x80457bf0             // DataOnElem
#define PORT_HEAPINIT 0x8049f030               // Heap::Init
#define PORT_DATAREGISTERFUNC 0x804545e0       // DataRegisterFunc
#define PORT_FILEISLOCAL 0x80422560            // FileIsLocal
#define PORT_FILEISDLC 0x80422570              // FileIsDLC
#define PORT_SDMODECHECK 0x8041a790            // WiiContentMgr::SDModeCheck
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x80c904a8 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x80c91818  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x80cab890          // address of gDataFuncs
#define PORT_THEARCHIVE 0x80c92dd0          // address of TheArchive (main ARK)
#define PORT_THEBANDUI 0x80c8f800           // address of TheBandUI
#define PORT_NULLSYMBOL 0x80bb5d30          // address of gNullSymbol
#define PORT_THESONGDB 0x80c8f048           // address of TheSongDB - TODO: check
#define PORT_THEMUSICLIBRARY 0x80c904b8     // pointer to TheMusicLibrary
#define PORT_THESONGSORTMGR 0x80c91248      // pointer to TheSongSortMgr
#define PORT_THESONGMGR 0x80c8f61c          // address of gSongMgr
#define PORT_THEMETAPERFORMER 0x80c90470    // address of TheMetaPerformer / MetaPerformer::sMetaPerformer
#define PORT_THEBANDUSERMGR 0x80c8e9b8      // pointer to TheBandUserMgr
#define PORT_THEGAME 0x80c8eb18             // pointer to TheGame (you lost)
#define PORT_OBJECTDIRMAINDIR 0x80cac538    // ObjectDir::sMainDir
// string pointers
#define PORT_NASWII_AC_URL 0x80c77fdc
#define PORT_NASWII_PR_URL 0x80c7805c
// wii stuff
#define PORT_WII_OS0_GLOBALS 0x80000000              // "os0"/"early" globals
#define PORT_WII_OS1_GLOBALS 0x80003000              // "os1"/"late" globals
#define PORT_CONFIGMEM2_52MB_INST 0x80aac4e0         // ConfigMEM2_52MB 0x90000000 length param
#define PORT_CONFIGMEM2_56MB_INST 0x80aac5c0         // ConfigMEM2_56MB 0x90000000 length param
#define PORT_CONFIGMEM2_64MB_INST 0x80aac6a0         // ConfigMEM2_64MB 0x90000000 length param
#define PORT_ENABLEINSTSONMEM2LO16MB_INST 0x80aac8e0 // EnableInstsOnMEM2Lo16MB 0x90000000 length param
#define PORT_VISETMODE_LI_28 0x80abee6c              // "li r28, 0x1" in VISetRevolutionModeSimple
#define PORT_VISETMODE_STB_28 0x80abee74             // "stb r28, 0x21(r1)" in VISetRevolutionModeSimple
#define PORT_VISETMODE_PATCH_CODE 0x800027a0         // stub area to insert the 480p fix code for VISetRevolutionModeSimple
#define PORT_GXSETCOPYFILTER_BEQ 0x80a89650          // "beq 0x40" in GXSetCopyFilter
#define PORT_OSFATAL_HALT_X_OFFSET 0x80aaa59c        // 'li r7, 48' for the X offset in Halt (OSFatal)
#define PORT_OSFATAL_HALT_Y_OFFSET 0x80aaa59c        // 'li r8, 100' for the Y offset in Halt (OSFatal)
#define PORT_SCREENREPORT_X_NEWLINE 0x80aaa5a4       // 'subi rX, rX, 48' for the X newline cutoff in ScreenReport
#define PORT_OSFATAL_HALT_OSREPORT 0x80aaa600        // call to OSReport in Halt (OSFatal)
#define PORT_OSFATAL_HALT_PPCHALT 0x80aaa604         // call to PPCHalt in Halt (OSFatal)
#define PORT_OSFATAL 0x80aa9e50                      // OSFatal
#define PORT_OSSETERRORHANDLER 0x80aa7d20            // OSSetErrorHandler
#define PORT_PPCHALT 0x80a49600                      // PPCHalt
#define PORT_OSRETURNTOMENU 0x80aad970               // OSReturnToMenu
#define PORT_OSREADROM 0x80aae180                    // OSReadROM
#define PORT_ARCINITHANDLE 0x80a43020                // ARCInitHandle
#define PORT_CONTENTINITHANDLETITLENAND 0x80a76fd0   // contentInitHandleTitleNAND
#define PORT_CNTRELEASEHANDLE 0x80a77890             // CNTReleaseHandle
#define PORT_CNTREAD 0x80a77c40                      // CNTRead
#define PORT_ECGETCONTENTINFOS 0x80aed870            // EC_GetContentInfos
// bank8 specific stuff
#define PORT_BANK8_MEM2_RSO_ASSERT1 0x804428e8
#define PORT_BANK8_MEM2_RSO_ASSERT2 0x80442940
#define PORT_BANK8_MEM2_RSO_ASSERT3 0x804b42a8
#define PORT_BANK8_MEM2_RSO_ASSERT4 0x804b4198
#define PORT_BANK8_KEYBOARD_RESOLVED 0x80448750 // ResolvedModule<keyboardModule>?

// define logging functions
#define RB3E_PRINT printf

#endif // RB3E_WII_BANK8
#endif // RB3E_WII
