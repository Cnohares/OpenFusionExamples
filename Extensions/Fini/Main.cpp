// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// You can manually enter these, or use CICK (recommended)
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

// Common Include
#include	"common.h"

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each condition
short conditionsInfos[]=
		{
		IDMN_CONDITION_ITS, M_CONDITION_ITS, CND_CONDITION_ITS, 0, 1, PARAM_EXPSTRING,M_CND_ITN,
		IDMN_CONDITION_ITI, M_CONDITION_ITI, CND_CONDITION_ITI, 0, 1, PARAM_EXPSTRING,M_CND_ITN,
		};

// Definitions of parameters for each action
short actionsInfos[]=
		{
		IDMN_ACTION_NF, M_ACTION_NF,	ACT_ACTION_NF,	0, 0,
		IDMN_ACTION_DF, M_ACTION_DF,	ACT_ACTION_DF,	0, 0,
		IDMN_ACTION_LF, M_ACTION_LF,	ACT_ACTION_LF,	0, 1,PARAM_FILENAME2,ACT_ACTION_F,
		IDMN_ACTION_SF, M_ACTION_SF,	ACT_ACTION_SF,	0, 1,PARAM_FILENAME2,ACT_ACTION_F,
		IDMN_ACTION_SSIV, M_ACTION_SSIV,	ACT_ACTION_SSIV,	0, 3,PARAM_EXPSTRING,PARAM_EXPSTRING,PARAM_EXPRESSION,ACT_ACTION_SSI_S,ACT_ACTION_SSI_I,ACT_ACTION_SSI_VAL,
		IDMN_ACTION_SSIS, M_ACTION_SSIS,	ACT_ACTION_SSIS,	0, 3,PARAM_EXPSTRING,PARAM_EXPSTRING,PARAM_EXPSTRING,ACT_ACTION_SSI_S,ACT_ACTION_SSI_I,ACT_ACTION_SSI_STR,
		IDMN_ACTION_DSI, M_ACTION_DSI,	ACT_ACTION_DSI,	0, 2,PARAM_EXPSTRING,PARAM_EXPSTRING,ACT_ACTION_SSI_S,ACT_ACTION_SSI_I,
		IDMN_ACTION_ITS, M_ACTION_ITS,	ACT_ACTION_ITS,	0, 1,PARAM_EXPSTRING,ACT_ACTION_IT,
		IDMN_ACTION_ITI, M_ACTION_ITI,	ACT_ACTION_ITI,	0, 2,PARAM_EXPSTRING,PARAM_EXPSTRING,ACT_ACTION_ITSN,ACT_ACTION_IT
		};

// Definitions of parameters for each expression
short expressionsInfos[]=
		{
		IDMN_EXPRESSION_GSIV, M_EXPRESSION_GSIV, EXP_EXPRESSION_GSIV, 0, 2, EXPPARAM_STRING, EXPPARAM_STRING, ACT_ACTION_SSI_S, ACT_ACTION_SSI_I,
		IDMN_EXPRESSION_GSIS, M_EXPRESSION_GSIS, EXP_EXPRESSION_GSIS, EXPFLAG_STRING, 2, EXPPARAM_STRING, EXPPARAM_STRING, ACT_ACTION_SSI_S, ACT_ACTION_SSI_I,
		IDMN_EXPRESSION_GCS, M_EXPRESSION_GCS, EXP_EXPRESSION_GCS, EXPFLAG_STRING, 0,
		IDMN_EXPRESSION_GCI, M_EXPRESSION_GCI, EXP_EXPRESSION_GCI, EXPFLAG_STRING, 0,
		};

// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================

long WINAPI DLLExport OnIterate_Section(LPRDATA rdPtr, long param1, long param2) {
	LPCTSTR LoopName = (LPCTSTR)CNC_GetStringParameter(rdPtr);

	return (wcscmp(LoopName, rdPtr->SecLoopName) == 0) ? TRUE : FALSE;
}

long WINAPI DLLExport OnIterate_Item(LPRDATA rdPtr, long param1, long param2) {
	LPCTSTR LoopName = (LPCTSTR)CNC_GetStringParameter(rdPtr);

	return (wcscmp(LoopName, rdPtr->ItemLoopName) == 0) ? TRUE : FALSE;
}


// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================

short WINAPI DLLExport New(LPRDATA rdPtr, long param1, long param2) {
	Init();

	return 0;
}

short WINAPI DLLExport Release(LPRDATA rdPtr, long param1, long param2) {
	release();

	return 0;
}

short WINAPI DLLExport LoadFromFile(LPRDATA rdPtr, long param1, long param2) {
	LPCTSTR FilePath = (LPCTSTR)param1;
	
	Init();

	Fini->LoadFile(FilePath);

	return 0;
}

short WINAPI DLLExport SaveToFile(LPRDATA rdPtr, long param1, long param2) {
	invalid();
	
	LPCTSTR FilePath = (LPCTSTR)param1;	
	
	Fini->SaveFile(FilePath, false);

	return 0;
}

short WINAPI DLLExport SetSecItem_Value(LPRDATA rdPtr, long param1, long param2) {
	invalid();
	
	LPCTSTR Section = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	LPCTSTR Item = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	
	InvalidSecItem(0);

	long p = CNC_GetFloatParameter(rdPtr);
	float Value = *(float*)&p;

	LPTSTR String = new WCHAR[FLOAT_MAX];

	swprintf(String, FLOAT_MAX, _T("%f"), Value);

	Fini->SetValue(Section, Item, String);

	delete[] String;

	return 0;
}

short WINAPI DLLExport SetSecItem_String(LPRDATA rdPtr, long param1, long param2) {
	invalid();
	
	LPCTSTR Section = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	LPCTSTR Item = (LPCTSTR)CNC_GetStringParameter(rdPtr);

	InvalidSecItem(0);

	LPCTSTR String = (LPCTSTR)CNC_GetStringParameter(rdPtr);	

	Fini->SetValue(Section, Item, String);

	return 0;
}

short WINAPI DLLExport DeleteSecItem(LPRDATA rdPtr, long param1, long param2) {
	invalid();

	LPCTSTR Section = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	LPCTSTR Item = (LPCTSTR)CNC_GetStringParameter(rdPtr);

	InvalidSecItem(0);

	if (wcscmp(Item, Empty_Str) == 0) {
		Item = nullptr;
	}

	Fini->Delete(Section, Item, true);

	return 0;
}

short WINAPI DLLExport Iterate_Section(LPRDATA rdPtr, long param1, long param2) {
	invalid();

	LPCTSTR LoopName = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	NewStr(rdPtr->SecLoopName, LoopName);

	INILIST Temp;
	Fini->GetAllSections(Temp);

	INIIT it;
	for (it = Temp.begin(); it != Temp.end(); ++it) {
		NewStr(rdPtr->CurrentSec, it->pItem);
		callRunTimeFunction(rdPtr, RFUNCTION_GENERATEEVENT, ONIT_SEC, 0);
	}

	return 0;
}

short WINAPI DLLExport Iterate_Item(LPRDATA rdPtr, long param1, long param2) {
	invalid();

	LPCTSTR SectionName = (LPCTSTR)CNC_GetStringParameter(rdPtr);
	LPCTSTR LoopName = (LPCTSTR)CNC_GetStringParameter(rdPtr);	
	
	NewStr(rdPtr->ItemLoopName, LoopName);	

	if (!(wcscmp(SectionName, Empty_Str) == 0)) {
		if ((rdPtr->CurrentSec == nullptr) || (!(wcscmp(SectionName, rdPtr->CurrentSec) == 0))) {
			NewStr(rdPtr->CurrentSec, SectionName);
		}
	}

	INILIST Temp;
	Fini->GetAllKeys(rdPtr->CurrentSec, Temp);

	INIIT it;
	for (it = Temp.begin(); it != Temp.end(); ++it) {
		NewStr(rdPtr->CurrentItem, it->pItem);
		callRunTimeFunction(rdPtr, RFUNCTION_GENERATEEVENT, ONIT_ITEM, 0);
	}

	return 0;
}

// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================

long WINAPI DLLExport GetSecItem_Value(LPRDATA rdPtr, long param1) {
	LPCTSTR Section = (LPCTSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	LPCTSTR Item = (LPCTSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	
	float Val = 0;

	InvalidSecItem(*((long*)&Val));

	LPCTSTR String = Fini->GetValue(Section, Item, Default_Val);

	Val = std::stof(String);
	
	//Setting the HOF_FLOAT flag lets MMF know that you are returning a float.
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	
	//Return the float without conversion
	return *((long*)&Val);
}

long WINAPI DLLExport GetSecItem_String(LPRDATA rdPtr, long param1) {
	LPCTSTR Section = (LPCTSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	LPCTSTR Item = (LPCTSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	
	InvalidSecItem((long)Default_Str);

	NewStr(OStr,Fini->GetValue(Section, Item, Default_Str));

	//Setting the HOF_STRING flag lets MMF know that you are a string.
	rdPtr->rHo.hoFlags |= HOF_STRING;
	
	//This returns a pointer to the string for MMF.
	return (long)OStr;
}

long WINAPI DLLExport GetCurrentSection(LPRDATA rdPtr, long param1) {
	//Setting the HOF_STRING flag lets MMF know that you are a string.
	rdPtr->rHo.hoFlags |= HOF_STRING;

	//This returns a pointer to the string for MMF.
	if (rdPtr->CurrentSec == nullptr) {
		return (long)Empty_Str;
	}
	else {
		return (long)rdPtr->CurrentSec;
	}
}

long WINAPI DLLExport GetCurrentItem(LPRDATA rdPtr, long param1) {
	//Setting the HOF_STRING flag lets MMF know that you are a string.
	rdPtr->rHo.hoFlags |= HOF_STRING;

	//This returns a pointer to the string for MMF.
	if (rdPtr->CurrentItem == nullptr) {
		return (long)Empty_Str;
	}
	else {
		return (long)rdPtr->CurrentItem;
	}	
}

// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) = 
			{ 
			OnIterate_Section,
			OnIterate_Item,
			};
	
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
			{
			New,
			Release,
			LoadFromFile,
			SaveToFile,
			SetSecItem_Value,
			SetSecItem_String,
			DeleteSecItem,
			Iterate_Section,
			Iterate_Item,
			0
			};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = 
			{     
			GetSecItem_Value,
			GetSecItem_String,
			GetCurrentSection,
			GetCurrentItem,
			0
			};