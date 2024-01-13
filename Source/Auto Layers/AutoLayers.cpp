#include "AutoLayers.h"
#include "Commands.h"

static A_long				S_idle_count		=	0L;

AEGP_PluginID S_my_id = 0L;
SPBasicSuite *sP = NULL;

// Death
static A_Err
DeathHook(
	AEGP_GlobalRefcon	plugin_refconP,
	AEGP_DeathRefcon	refconP)
{
	A_Err	err			= A_Err_NONE;
	AEGP_SuiteHandler	suites(sP);

	A_char report[AEGP_MAX_ABOUT_STRING_SIZE] = {'\0'};
		
	suites.ANSICallbacksSuite1()->sprintf(report, STR(StrID_IdleCount), S_idle_count); 
	
	return err;
}

// Idle
static A_Err	
IdleHook(
	AEGP_GlobalRefcon	plugin_refconP,	
	AEGP_IdleRefcon		refconP,		
	A_long				*max_sleepPL)
{
	A_Err	err			= A_Err_NONE;
	AEGP_SuiteHandler suites(sP);
	S_idle_count++;

	DoCommands();

	return err;
}

static A_Err
CommandHook(
	AEGP_GlobalRefcon	plugin_refconPV,		
	AEGP_CommandRefcon	refconPV,				
	AEGP_Command		command,				
	AEGP_HookPriority	hook_priority,			
	A_Boolean			already_handledB,	
	A_Boolean			*handledPB)	
{
	A_Err			err = A_Err_NONE;
					
	AEGP_SuiteHandler	suites(sP);

	return err;
}

A_Err
EntryPointFunc(
	struct SPBasicSuite		*pica_basicP,		
	A_long				 	major_versionL,			
	A_long					minor_versionL,		
	AEGP_PluginID			aegp_plugin_id,		
	AEGP_GlobalRefcon		*global_refconV)	
{
	S_my_id										= aegp_plugin_id;
	A_Err 					err 				= A_Err_NONE, 
							err2 				= A_Err_NONE;
	
	sP = pica_basicP;
	
	AEGP_SuiteHandler suites(pica_basicP);


	ERR(suites.RegisterSuite5()->AEGP_RegisterCommandHook(	S_my_id, 
															AEGP_HP_BeforeAE, 
															AEGP_Command_ALL, 
															CommandHook, 
															0));

	ERR(suites.RegisterSuite5()->AEGP_RegisterDeathHook(S_my_id, DeathHook,	NULL));

	ERR(suites.RegisterSuite5()->AEGP_RegisterIdleHook(S_my_id, IdleHook, NULL));
	
	if (err){ // not !err, err!
		ERR2(suites.UtilitySuite3()->AEGP_ReportInfo(S_my_id, "Auto Layers : Could not register command hook."));
	}
	return err;
}
			  
