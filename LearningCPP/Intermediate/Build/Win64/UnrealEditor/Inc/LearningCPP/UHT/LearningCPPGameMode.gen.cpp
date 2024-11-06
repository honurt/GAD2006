// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "LearningCPP/LearningCPPGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLearningCPPGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
LEARNINGCPP_API UClass* Z_Construct_UClass_ALearningCPPGameMode();
LEARNINGCPP_API UClass* Z_Construct_UClass_ALearningCPPGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_LearningCPP();
// End Cross Module References

// Begin Class ALearningCPPGameMode
void ALearningCPPGameMode::StaticRegisterNativesALearningCPPGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ALearningCPPGameMode);
UClass* Z_Construct_UClass_ALearningCPPGameMode_NoRegister()
{
	return ALearningCPPGameMode::StaticClass();
}
struct Z_Construct_UClass_ALearningCPPGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "LearningCPPGameMode.h" },
		{ "ModuleRelativePath", "LearningCPPGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALearningCPPGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ALearningCPPGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_LearningCPP,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALearningCPPGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ALearningCPPGameMode_Statics::ClassParams = {
	&ALearningCPPGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ALearningCPPGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ALearningCPPGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ALearningCPPGameMode()
{
	if (!Z_Registration_Info_UClass_ALearningCPPGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ALearningCPPGameMode.OuterSingleton, Z_Construct_UClass_ALearningCPPGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ALearningCPPGameMode.OuterSingleton;
}
template<> LEARNINGCPP_API UClass* StaticClass<ALearningCPPGameMode>()
{
	return ALearningCPPGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ALearningCPPGameMode);
ALearningCPPGameMode::~ALearningCPPGameMode() {}
// End Class ALearningCPPGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Onur_Documents_Unreal_Projects_LearningCPP_Source_LearningCPP_LearningCPPGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ALearningCPPGameMode, ALearningCPPGameMode::StaticClass, TEXT("ALearningCPPGameMode"), &Z_Registration_Info_UClass_ALearningCPPGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ALearningCPPGameMode), 116147332U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Onur_Documents_Unreal_Projects_LearningCPP_Source_LearningCPP_LearningCPPGameMode_h_564360414(TEXT("/Script/LearningCPP"),
	Z_CompiledInDeferFile_FID_Users_Onur_Documents_Unreal_Projects_LearningCPP_Source_LearningCPP_LearningCPPGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Onur_Documents_Unreal_Projects_LearningCPP_Source_LearningCPP_LearningCPPGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
