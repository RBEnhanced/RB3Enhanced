/*
    RB3Enhanced - ports_xbox360.h
    Defines port addresses and platform-specific function definitions (for 360 TU5)
*/

#ifdef RB3E_XBOX // Rock Band 3 Xbox 360 Title Update 5

// instruction patch addresses
#define PORT_SONGLIMIT 0x82579880                // call to "max_song_count" DataNode::_value
#define PORT_APP_CALL 0x82272e88                 // call to App::_ct from main()
#define PORT_SONGBLACKLIST 0x82579098            // call to BandSongMgr::IsInExclusionList
#define PORT_DATAINITFUNCS_TAIL 0x82765980       // blr of DataInitFuncs
#define PORT_FASTSTART_CHECK 0x82270f40          // beq after OptionBool("fast",0) in App::_ct
#define PORT_SYSTEMINIT_BLANK 0x825113a4         // call to a stub function in SystemInit
#define PORT_XL_BYPASS_SG 0x82a88268             // lbz r11, bypassSecureGateway in Quazal::XboxClient::Login2
#define PORT_RCG_POLL_LOGGEDIN 0x824f7178        // check for login status in RockCentralGateway::Poll
#define PORT_AUD_PATCH_CRASH 0x82b8c928          // patch somewhere to prevent Audition Mode crashes - blr
#define PORT_AUD_PATCH_NET1 0x82b8f7fc           // patch somewhere to force Audition Mode to be connected (1) - nop
#define PORT_AUD_PATCH_NET2 0x82b8f80c           // patch somewhere to force Audition Mode to be connected (2) - nop
#define PORT_AUD_PATCH_NET3 0x82b8f814           // patch somewhere to force Audition Mode to be connected (3) - nop
#define PORT_AUD_PATCH_NET4 0x82b8f81c           // patch somewhere to force Audition Mode to be connected (4) - li r3, 0
#define PORT_AUD_PATCH_NET5 0x82562638           // patch somewhere to force Audition Mode to be connected (5) - li r3, 1
#define PORT_AUD_PATCH_REPL 0x82b8e978           // patch to allow Audition to recieve insecure packets from Magma
#define PORT_AUD_PATCH_HDD 0x82515dd4            // patch to make has_hard_drive always return true, probably broken
#define PORT_AUD_PATCH_UNK 0x823f6074            // idk
#define PORT_AUD_HANDLE_MESSAGES 0x82563edc      // part of AuditionMgr::Handle that handles disconnected from Rock Central/XBL/etc. messages
#define PORT_AUD_HANDLE_MSG_JUMP 0x82564048      // address to jump PORT_AUD_HANDLE_MESSAGES to, to allow audition mode without GoCentral
#define PORT_AUD_INVALID_DATA_CHECK 0x825629fc   // check for invalid data in a song submitted to audition mode
#define PORT_XNQOS_PROBE1 0x823ee7f8             // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE2 0x823ee800             // instruction that checks xnqos probe results
#define PORT_XNQOS_PROBE3 0x823ee80c             // instruction that checks xnqos probe results
#define PORT_VDP_DISABLE1 0x82b39ba0             // nop over VDP packet sending
#define PORT_VDP_DISABLE2 0x82b39e60             // nop over VDP packet sending
#define PORT_VDP_DISABLE3 0x82b3a5e4             // nop over VDP packet sending
#define PORT_VDP_DISABLE4 0x82b3a5f0             // nop over VDP packet sending
#define PORT_SESSION_MASK_CHECK 0x82652acc       // beq in while loop for instrument mask check
#define PORT_CHARACTER_CLOTHES_CHECK 0x82655148  // check to see if the goal required to select a piece of clothing has been achieved or not
#define PORT_CHARACTER_CLOTHES_CHECK2 0x8265514c // check to see if the goal required to select a piece of clothing has been achieved or not 2
#define PORT_FACE_PAINT_CHECK 0x82614a60         // check to see if face paint is unlocked
#define PORT_TATTOO_CHECK 0x82614a9c             // check to see if tattoos are unlocked
#define PORT_VIDEO_VENUE_CHECK 0x82581634        // check to see if video venues are unlocked
#define PORT_GOLD_STAR_CHECK1 0x826a15b0         // sets the gold threshold to a massive number if not on expert 1
#define PORT_GOLD_STAR_CHECK2 0x826a1654         // sets the gold threshold to a massive number if not on expert 2
#define PORT_GOLD_STAR_CHECK3 0x826a118c         // sets the gold threshold to a massive number if not on expert 3
#define PORT_OPTIONSTR_DEFINE 0x82510cc8         // bl to OptionStr("define", NULL) in PreInitSystem
#define PORT_RENDER_RES_X_PATCH1 0x8273bf20      // instruction patch to force render width
#define PORT_RENDER_RES_X_PATCH2 0x8273bf24      // instruction patch to force render width
#define PORT_RENDER_RES_Y_PATCH1 0x8273bedc      // instruction patch to force render height
#define PORT_RUNLOOP_SPARE 0x822700e0            // branch to a function that only has a "blr" in App::Run(WithoutDebugging)
#define PORT_SONG_ID_EVALUATE 0x827aa7d4         // branch to DataNode::Evaluate in SongMetadata::__ct
#define PORT_LOADOBJS_BCTRL 0x827562e4           // bctrl to Object::PreLoad insie of DirLoader::LoadObjs
#define PORT_SONGMGR_ISDEMO_CHECK 0x82575f9c     // "bne" after IsUGC check inside SongMgr::IsDemo
#define PORT_STAGEKIT_EXISTS 0x8228d03c          // StageKit check. nop over to allow for fog command without a physical StageKit connected.
#define PORT_VERTEX_READ_1 0x82418704            // call to Vector3::operator>> to read vertex position
#define PORT_VERTEX_READ_2 0x82418748            // call to Vector3::operator>> to read vertex normals
#define PORT_BANDCHARDESC_MAKEOUTFITPATH 0x82335220 // BandCharDesc::MakeOutfitPath
#define PORT_MULTIPLAYER_CRASH 0x82ae6880        // branch to a function that can crash in online multiplayer
#define PORT_MULTIPLAYER_FIX 0x8282b238          // the function that doesn't crash
#define PORT_QUAZAL_BREAKPOINT 0x828410c0        // address to DbgBreakPoint in Quazal::Platform::Breakpoint
#define PORT_MAINSEH 0x82272e60                  // address to __CxxFrameHandler above the main() function
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
#define PORT_SONGDATAADDMULTIGEM 0x827719b0          // SongData::AddMultiGem
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
#define PORT_XL_XINVITEGETACCEPTEDINFO 0x82a6a7c8    // XInviteGetAcceptedInfo
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
#define PORT_GAMEGETACTIVEPLAYER 0x82678e88          // Game::GetActivePlayer
#define PORT_GETBANDUSERS 0x82683b78                 // BandUserMgr::GetBandUsers
#define PORT_GETBANDUSERFROMSLOT 0x82682b60          // BandUserMgr::GetBandUserFromSlot
#define PORT_GETSONGID 0x827a87f0                    // GetSongID, function used when adding songs to BandSongMgr
#define PORT_SONGMGRGETRANKEDSONGS 0x82577340        // BandSongMgr::GetRankedSongs(?) - not sure on the real name of the function
#define PORT_BUILDINSTRUMENTSELECTION 0x82668c70     // BuildInstrumentSelectionList(?) - actual name not known
#define PORT_PREPARESOMEVECTORMAYBE 0x82796d90       // Prepares some vector, used by BuildInstrumentSelectionList
#define PORT_SOMEVECTORPUSHBACKMAYBE 0x82b6aa10      // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back
#define PORT_POSTPROC_DOPOST 0x82b89a08              // NgPostProc::DoPost
#define PORT_MUSICLIBRARYSELECTMAYBE 0x8253EB00      // Selects an entry in the Music Library screen - actual name not known
#define PORT_GETSYMBOLBYGAMEORIGIN 0x8265bb78        // SongSortByRecent::GetSymbolByGameOrigin
#define PORT_GETGAMEORIGINBYSYMBOL 0x8265b910        // SongSortByRecent::GetGameOriginBySymbol
#define PORT_SONGSORTBYRECENT 0x8265bde8             // RecentCmp::__ct
#define PORT_FILESTREAM_CT 0x827c3340                // FileStream::__ct (the one that takes a char * path instead of a File object)
#define PORT_CHUNKSTREAM_CT 0x827ca488               // ChunkStream::__ct
#define PORT_RNDPROPANIMSETFRAME 0x82426dd0          // RndPropAnim::SetFrame
#define PORT_DYNAMICCAST 0x8282a0c8                  // dynamic_cast
#define PORT_OBJECTFINDUIPANEL 0x82537430            // Object::Find<UIPanel>
#define PORT_JOYPADGETCACHEDXINPUTCAPS 0x82531F08    // JoypadGetCachedXInputCaps
#define PORT_JOYPADGETPADDATA 0x82524998             // JoypadGetPadData
#define PORT_MEMFREE 0x827bc430                      // MemFree
#define PORT_MEMALLOC 0x827bcd38                     // MemAlloc
#define PORT_SYMBOLPREINIT 0x827c04f8                // Symbol::PreInit
#define PORT_QUEUINGSOCKET_BIND 0x82b397b0           // Quazal::QueuingSocket::Bind
#define PORT_QUAZALSOCKET_BIND 0x82b1a830            // Quazal::Socket::Bind
#define PORT_MEMPRINTOVERVIEW 0x827bc838             // MemPrintOverview
#define PORT_MEMPRINT 0x827bc970                     // MemPrint
#define PORT_MEMNUMHEAPS 0x827bb628                  // MemNumHeaps
#define PORT_METAMUSICISLOADED 0x8270fab8            // MetaMusic::Loaded
#define PORT_METAMUSICLOAD 0x82710ab0                // MetaMusic::Load
#define PORT_METAMUSICPOLL 0x82711438                // MetaMusic::Poll
#define PORT_METAMUSICSTART 0x82711a00               // MetaMusic::Start
#define PORT_INITSONGMETADATA 0x827aa450             // InitSongMetadata
#define PORT_UPDATEPRESENCE 0x82680430               // PresenceMgr::UpdatePresence
#define PORT_STEPSEQUENCEJOBSETSTEP 0x82af92b8       // Quazal::StepSequenceJob::SetStep
#define PORT_BINSTREAMWRITE 0x827c4f58               // BinStream::Write
#define PORT_BINSTREAMREAD 0x827c4ea8                // BinStream::Read
#define PORT_BINSTREAMREADENDIAN 0x827c5058          // BinStream::ReadEndian
#define PORT_BINSTREAMWRITEENDIAN 0x827c5098         // BinStream::WriteEndian
#define PORT_SONGPARSERPITCHTOSLOT 0x82783c20        // SongParser::PitchToSlot
#define PORT_DATASET 0x8275d670                      // DataSet
#define PORT_DATASETELEM 0x82760b38                  // DataSetElem
#define PORT_DATAONELEM 0x8275ff50                   // DataOnElem
#define PORT_DATANODEGETOBJ 0x8274b088               // DataNode::GetObj
#define PORT_DATAARRAYEXECUTE 0x8274d198             // DataArray::Execute
#define PORT_DXRND_SUSPEND 0x8273A370                // DxRnd::Suspend
#define PORT_XBOXCONTENT_CONSTRUCTOR 0x8251fb40      // XboxContent::__ct
#define PORT_CACHEMGRXBOX_MOUNTASYNC 0x827d7b38      // CacheMgrXbox::MountAsync
#define PORT_BANDCHARDESC_NEWOBJECT 0x823399c0        // BandCharDesc::NewObject
#define PORT_BANDCHARDESC_RELOADPREFABS 0x82339048        // BandCharDesc::ReloadPrefabs
#define PORT_PREFABCHAR_NEWOBJECT 0x82553f40         // PrefabChar::NewObject
#define PORT_DIRLOADER_LOADOBJS 0x82757b00           // DirLoader::LoadObjs
#define PORT_GETPREFABPORTRAITPATH 0x825d8078        // GetPortraitPath
// instance addresses
#define PORT_MODIFIERMGR_POINTER 0x82dfec08 // pointer to ModifierManager
#define PORT_ROCKCENTRALGATEWAY 0x82cc8f60  // address of RockCentralGateway
#define PORT_GDATAFUNCS 0x82e05d30          // address of gDataFuncs
#define PORT_THEARCHIVE 0x82cc9c60          // address of TheArchive (main ARK)
#define PORT_THEBANDUI 0x82dfd2b0           // address of TheBandUI
#define PORT_NULLSYMBOL 0x82c71838          // address of gNullSymbol
#define PORT_THESONGDB 0x82e023f8           // address of TheSongDB
#define PORT_THESONGMGR 0x82dfe7b4          // address of TheSongMgr
#define PORT_THEMETAPERFORMER 0x82dfe954    // address of TheMetaPerformer
#define PORT_THEBANDUSERMGR 0x82e023b8      // address of TheBandUserMgr
#define PORT_THESONGSORTMGR 0x82dfee5c      // pointer to TheSongSortMgr
#define PORT_THEMUSICLIBRARY 0x82dfd3a8     // pointer to TheMusicLibrary
#define PORT_THEGAME 0x82e02128             // pointer to TheGame (you lost)
#define PORT_OBJECTDIRMAINDIR 0x82e054b8    // ObjectDir::sMainDir
#define PORT_MESH_GREV 0x82cc2638           // address of RndMesh::gRev
#define PORT_DXRND 0x82e04b38               // address of TheDxRnd
#define PORT_GPREFABS 0x82cbe1b8            // gPrefabs (directory for prefabs)
// import function stubs
#define PORT_XEKEYSSETKEY_STUB 0x82c4c47c
#define PORT_XEKEYSAESCBC_STUB 0x82c4c48c
#define PORT_SOCKET_STUB 0x8284da48
#define PORT_XNETSTARTUP 0x8284d7e8
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
#define PORT_XAMSHOWFRIENDSUI 0x8283d710
#define PORT_XAMLOADERTERMINATETITLE 0x82c4bccc

// define logging functions
void DbgPrint(const char *s, ...);
#define RB3E_PRINT DbgPrint

#endif // RB3E_XBOX
