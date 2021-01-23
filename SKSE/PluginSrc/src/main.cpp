// When Removing SKSE64 Src Files From Project, Change Headers To Appropriate Paths For Build Against It
#include "../SKSE64(Remove When Done)/common/IDebugLog.h"        //------------------ For _MESSAGE
#include "../SKSE64(Remove When Done)/skse64/skse64/PluginAPI.h" //----------- For SKSEInterface and PluginInfo
#include "ShlObj.h" //-------------------------------------------------------- Windows Header - For CSIDL_MYDOCUMENTS
#include "../SKSE64(Remove When Done)/skse64/skse64_common/skse_version.h" //-- For Runtime Version Checking

/*
    As Of Right Now, Solely Writing This For The Current SE Runtime Of 1.5.97
    Don't Have LE To Test It On And It Really Only Makes Sense To Start Writing
    For The Current Version On SE
*/

// Globally Defined Interface Handles
PluginHandle pluginHandle                          = kPluginHandle_Invalid;
SKSETaskInterface* taskInterface                   = nullptr;
SKSEPapyrusInterface* papyrusInterface             = nullptr;
SKSEMessagingInterface* messageInterface           = nullptr;
SKSESerializationInterface* serializationInterface = nullptr;
// I Know I'll Need That Serialization Interface For Better Performance,
// Just Don't Know A Lot About Serializing/De-serializing Data Quite Yet...

extern "C" {

	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		gameLog.OpenRelative(CSIDL_MYDOCUMENTS,
				     "\\My Games\\Skyrim Special Edition\\SKSE\\Dynamic Weather Effects.log");
		gameLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gameLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
		_MESSAGE("Dynamic Weather Effects Currently Loading");

		// A Lil Something About Us (Filling In The PluginInfo Struct)
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name        = "Dynamic Weather Effects Plugin For 1.5.97";
		info->version     = 1.0;
		// Store SKSE's Handle To This So That We Can Communicate With SKSE
		pluginHandle = skse->GetPluginHandle( );

		// Not Sure What Behavior I Want For Editor Loading Yet...As This Pertains
		// To Weather Effects, Might Be Useful To Be Able To Load It Into The Editor
		if(skse->isEditor) {
			_MESSAGE("Dynamic Weather Effects Plugin For 1.5.97 Was Loaded In The Editor");
			return true;
		}

		// Checking Against The Runtime Version For Compatibility (Currently: SE=1.5.97, LE=1.5.73)
		switch(skse->runtimeVersion) {
			case RUNTIME_VERSION_1_5_97: break;
			// %08X = Padding For 8 Bits With Leading 0's On Empty Fields
			default:
				_MESSAGE("Unsupported Runtime Detected: Version %08X", skse->runtimeVersion);
				return false;
		}

		// Checking Against The Task Version For Acquiring
		taskInterface = ( SKSETaskInterface* ) skse->QueryInterface(kInterface_Task);
		if(!taskInterface) {
			_FATALERROR("Was Unable To Acquire The Task Interface");
			return false;
		}

		// Checking Against The Papyrus Version For Acquiring
		papyrusInterface = ( SKSEPapyrusInterface* ) skse->QueryInterface(kInterface_Papyrus);
		if(!papyrusInterface) {
			_FATALERROR("Was Unable To Acquire The Papyrus Interface");
			return false;
		}

		// And Finally, Checking Against The Serialization Interface For Acquiring
		serializationInterface =
		  ( SKSESerializationInterface* ) skse->QueryInterface(kInterface_Serialization);
		if(!serializationInterface) {
			_FATALERROR("Was Unable To Acquire The Serialization Interface");
			return false;
		}

		/*
		    Since This Is A Callback For SKSE -> This Is All That Needs To Be Done In The Query Stage So
		   Return True i.e. (Taken From PluginAPI.h): Do not do anything other than fill out the PluginInfo
		   structure and return true/false in this callback.
		*/
		// NOTE(Ryan): Probably Should Be Doing Some Version Checks Here To Validate User's Runtime vs.
		// Current SKSE
		return true;
	}

	// This Is Where The Magic Happens - SKSE's Call To War For Us To Do Our Thang
	bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		_MESSAGE("Dynamic Weather Effects Plugin For 1.5.97 Was Successfully Loaded");

		messageInterface = ( SKSEMessagingInterface* ) skse->QueryInterface(kInterface_Messaging);
		if(messageInterface) {
			messageInterface->RegisterListener(pluginHandle, "SKSE", MessageManager);
			_MESSAGE("Successfully Registered For Message Handling")
		} else {
			_FATALERROR("Was Unable To Acquire The Messaging Interface");
		}

		// ToDo: Need To Implement From PapyrusCell.h And PapyrusEvents.h
		if(papyrusInterface) {
			// papyrusInterface->Register(FunctionHandler::RegisterFunctions);
			// papyrusInterface->Register(EventHandler::RegisterEvents);
			_MESSAGE("Successfully Registered For Papyrus Functions And Events")
		}
		// ToDo: After Learning How To Efficiently (De)Serialize Data, Add That Functionality Here
		return true;
	}
};

void MessageManager(SKSEMessagingInterface::Message* message)
{
	switch(message->type) {
		case SKSEMessagingInterface::kMessage_InputLoaded:
			{
			}
			break;
	}
}