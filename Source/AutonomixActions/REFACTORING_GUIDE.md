// Copyright Autonomix. All Rights Reserved.

#pragma once

/**
 * REFACTORING GUIDELINES FOR AUTONOMIX ACTION HANDLERS
 * 
 * This document shows how to migrate existing action handlers to use
 * the new FAutonomixActionHelpers utilities to eliminate code duplication.
 * 
 * KEY PATTERNS TO ELIMINATE:
 * 1. Input validation boilerplate
 * 2. Asset loading with error checking
 * 3. Transaction/Modify/MarkPackageDirty patterns
 * 4. File backup logic
 * 5. Package path parsing
 * 6. Asset factory boilerplate
 * 7. Result building patterns
 */

// ============================================================================
// PATTERN 1: Transaction & Asset Modification
// ============================================================================

/*
BEFORE (repeated in 30+ locations):
---
void FAutonomixBlueprintActions::CreateBlueprint()
{
    FScopedTransaction Transaction(FText::FromString(TEXT("Autonomix Blueprint Action")));
    
    Blueprint->Modify();
    // ... do work ...
    Blueprint->GetOutermost()->MarkPackageDirty();
}
---

AFTER (one-liner):
---
void FAutonomixBlueprintActions::CreateBlueprint()
{
    FAutonomixTransactionGuard Guard(Blueprint, TEXT("Create Blueprint"));
    // ... do work ...
    // Guard destructor automatically MarkPackageDirty() + ends transaction
}
---

USAGE EXAMPLE:
    UBlueprint* Blueprint = /* ... */;
    {
        FAutonomixTransactionGuard Guard(Blueprint, TEXT("Add Blueprint Component"));
        Blueprint->GetBlueprintGeneratedClass()->AddProperty(/* ... */);
        // On scope exit, MarkPackageDirty() happens automatically
    }
*/

// ============================================================================
// PATTERN 2: Asset Loading with Error Checking
// ============================================================================

/*
BEFORE (repeated 50+ times):
---
USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *SkeletonPath);
if (!Skeleton)
{
    Result.Errors.Add(FString::Printf(TEXT("Skeleton not found: '%s'"), *SkeletonPath));
    return Result;
}
---

AFTER:
---
FString Error;
USkeleton* Skeleton = FAutonomixAssetHelper::LoadAssetChecked<USkeleton>(SkeletonPath, Error);
if (!Skeleton)
{
    return FAutonomixResultHelper::MakeError(TEXT("Skeleton not found: %s"), *SkeletonPath);
}
---

FOR CLASS LOADING (with _C suffix fallback):
    FString Error;
    UClass* AnimBPClass = FAutonomixAssetHelper::LoadClassChecked(AnimBPPath, Error);
    if (!AnimBPClass)
    {
        return FAutonomixResultHelper::MakeError(TEXT("%s"), *Error);
    }
*/

// ============================================================================
// PATTERN 3: Package Path Parsing
// ============================================================================

/*
BEFORE (repeated 13 times):
---
FString PackagePath = FPackageName::GetLongPackagePath(AssetPath);
FString AssetName = FPackageName::GetShortName(AssetPath);
---

AFTER:
---
FAutonomixPackageInfo PackageInfo(AssetPath);
// Uses: PackageInfo.LongPath, PackageInfo.ShortName
---

OR use the helper directly:
    FAutonomixPackageInfo PackageInfo = FAutonomixPathHelper::GetPackageInfo(AssetPath);
*/

// ============================================================================
// PATTERN 4: Asset Factory Creation Boilerplate
// ============================================================================

/*
BEFORE (repeated 20+ times):
---
FString PackagePath = FPackageName::GetLongPackagePath(AssetPath);
FString AssetName = FPackageName::GetShortName(AssetPath);

IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

UAnimBlueprintFactory* Factory = NewObject<UAnimBlueprintFactory>();
Factory->TargetSkeleton = Skeleton;
Factory->ParentClass = UAnimInstance::StaticClass();

UObject* NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UAnimBlueprint::StaticClass(), Factory);
if (!NewAsset)
{
    Result.Errors.Add(TEXT("Failed to create AnimBlueprint"));
    return Result;
}
---

AFTER:
---
UAnimBlueprint* NewAnimBP = FAutonomixAssetFactory::Create<UAnimBlueprint, UAnimBlueprintFactory>(
    AssetPath,
    UAnimBlueprint::StaticClass(),
    [Skeleton](UAnimBlueprintFactory* Factory)
    {
        Factory->TargetSkeleton = Skeleton;
        Factory->ParentClass = UAnimInstance::StaticClass();
    },
    Result
);
if (!NewAnimBP)
{
    return Result;  // Errors already added
}
---

The template handles:
- Package path parsing
- Path validation
- Package creation
- Factory instantiation
- Asset creation
- Error reporting
- Result tracking
*/

// ============================================================================
// PATTERN 5: File I/O with Backup
// ============================================================================

/*
BEFORE (only in CppActions, but should be everywhere):
---
bool FAutonomixCppActions::WriteFileWithBackup(const FString& FilePath, const FString& Content)
{
    if (FPaths::FileExists(FilePath))
    {
        FString BackupDir = FPaths::Combine(
            FPaths::ProjectSavedDir(), TEXT("Autonomix"), TEXT("Backups"),
            FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S")));
        IFileManager::Get().MakeDirectory(*BackupDir, true);

        FString BackupPath = FPaths::Combine(BackupDir, FPaths::GetCleanFilename(FilePath));
        IFileManager::Get().Copy(*BackupPath, *FilePath);
    }

    return FFileHelper::SaveStringToFile(Content, *FilePath, 
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}
---

AFTER:
---
FString BackupPath;
if (!FAutonomixFileHelper::WriteFileWithBackup(FilePath, Content, &BackupPath))
{
    return FAutonomixResultHelper::MakeError(TEXT("Failed to write file: %s"), *FilePath);
}

if (!BackupPath.IsEmpty())
{
    Result.ResultMessage = FString::Printf(TEXT("File backed up to: %s"), *BackupPath);
}
---
*/

// ============================================================================
// PATTERN 6: Validation Helpers
// ============================================================================

/*
BEFORE (scattered validation logic):
---
if (!Params->HasField(TEXT("asset_path")))
{
    OutErrors.Add(TEXT("Missing required field: asset_path"));
    return false;
}

if (Params->GetField(TEXT("asset_path"))->Type != EJson::String)
{
    OutErrors.Add(TEXT("asset_path must be a string"));
    return false;
}
---

AFTER:
---
FString Error;
if (!FAutonomixValidationHelper::ValidateJsonField(Params, TEXT("asset_path"), EJson::String, Error))
{
    OutErrors.Add(Error);
    return false;
}
---

VALIDATE ASSET EXISTS:
    FString Error;
    if (!FAutonomixValidationHelper::ValidateAssetExists(AssetPath, UBlueprint::StaticClass(), Error))
    {
        return FAutonomixResultHelper::MakeError(TEXT("%s"), *Error);
    }

VALIDATE ASSET ARRAY:
    TArray<FString> Errors;
    if (!FAutonomixValidationHelper::ValidateAssetArray(AssetPaths, Errors))
    {
        return FAutonomixResultHelper::MakeError(TEXT("Invalid asset paths"));
    }
*/

// ============================================================================
// PATTERN 7: Result Building & Error Handling
// ============================================================================

/*
BEFORE (repeated pattern, 50+ times):
---
FAutonomixActionResult Result;
Result.bSuccess = false;
Result.Errors.Add(TEXT("Something failed"));
return Result;
---

AFTER:
---
return FAutonomixResultHelper::MakeError(TEXT("Something failed"));
---

FOR SUCCESS:
    return FAutonomixResultHelper::MakeSuccess(TEXT("Blueprint '%s' created successfully"), *AssetName);

FOR MULTIPLE ERRORS:
    TArray<FString> ValidationErrors;
    // ... collect errors ...
    FAutonomixActionResult Result;
    FAutonomixResultHelper::AddErrors(Result, ValidationErrors);
    return Result;
*/

// ============================================================================
// PATTERN 8: Scoped Action Logging
// ============================================================================

/*
USAGE:
---
FAutonomixActionResult FAutonomixBlueprintActions::CreateBlueprint(const TSharedRef<FJsonObject>& Params)
{
    AUTONOMIX_LOG_ACTION("CreateBlueprint");
    
    // Validation
    FString Error;
    if (!FAutonomixValidationHelper::ValidateJsonField(Params, TEXT("path"), EJson::String, Error))
    {
        _ActionLogger.LogError(TEXT("%s"), *Error);
        return FAutonomixResultHelper::MakeError(TEXT("%s"), *Error);
    }
    
    // Do work
    _ActionLogger.LogSuccess(TEXT("Blueprint created"));
    return FAutonomixResultHelper::MakeSuccess(TEXT("Done"));
}
---

Output:
    >>> Starting action: CreateBlueprint
    [CreateBlueprint] ERROR: Missing required field: path
    <<< Completed action: CreateBlueprint (0.042 sec)
*/

// ============================================================================
// COMPLETE EXAMPLE: Refactored Handler
// ============================================================================

/*
OLD CODE (Animation handler - ~200 lines, heavy duplication):
---
FAutonomixActionResult FAutonomixAnimationActions::CreateAnimBlueprint(const TSharedRef<FJsonObject>& Params)
{
    FAutonomixActionResult Result;
    
    // Validation
    if (!Params->HasField(TEXT("path")))
    {
        Result.Errors.Add(TEXT("Missing path"));
        Result.bSuccess = false;
        return Result;
    }
    
    FString AssetPath = Params->GetStringField(TEXT("path"));
    
    // Load skeleton
    FString SkeletonPath = Params->GetStringField(TEXT("skeleton_path"));
    USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *SkeletonPath);
    if (!Skeleton)
    {
        Result.Errors.Add(FString::Printf(TEXT("Skeleton not found: '%s'"), *SkeletonPath));
        Result.bSuccess = false;
        return Result;
    }
    
    // Parse paths
    FString PackagePath = FPackageName::GetLongPackagePath(AssetPath);
    FString AssetName = FPackageName::GetShortName(AssetPath);
    
    // Create asset
    FScopedTransaction Transaction(FText::FromString(TEXT("Autonomix Create AnimBlueprint")));
    
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    UAnimBlueprintFactory* Factory = NewObject<UAnimBlueprintFactory>();
    Factory->TargetSkeleton = Skeleton;
    Factory->ParentClass = UAnimInstance::StaticClass();
    
    UObject* NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UAnimBlueprint::StaticClass(), Factory);
    if (!NewAsset)
    {
        Result.Errors.Add(TEXT("Failed to create AnimBlueprint"));
        Result.bSuccess = false;
        return Result;
    }
    
    UAnimBlueprint* AnimBP = Cast<UAnimBlueprint>(NewAsset);
    if (AnimBP)
    {
        AnimBP->Modify();
        AnimBP->GetOutermost()->MarkPackageDirty();
        
        Result.bSuccess = true;
        Result.ResultMessage = FString::Printf(TEXT("Created AnimBlueprint: %s"), *AssetPath);
        Result.CreatedAssets.Add(AssetPath);
    }
    else
    {
        Result.Errors.Add(TEXT("Cast failed"));
        Result.bSuccess = false;
    }
    
    return Result;
}
---

REFACTORED CODE (using helpers - ~80 lines, clear intent):
---
FAutonomixActionResult FAutonomixAnimationActions::CreateAnimBlueprint(const TSharedRef<FJsonObject>& Params)
{
    AUTONOMIX_LOG_ACTION("CreateAnimBlueprint");
    
    // Validation
    TArray<FString> ValidationErrors;
    if (!FAutonomixValidationHelper::ValidateJsonField(Params, TEXT("path"), EJson::String, ValidationErrors.Add_GetRef(FString())))
    {
        return FAutonomixResultHelper::MakeError(TEXT("Missing required field: path"));
    }
    
    FString AssetPath = Params->GetStringField(TEXT("path"));
    FString SkeletonPath = Params->GetStringField(TEXT("skeleton_path"));
    
    // Load skeleton
    FString SkeletonError;
    USkeleton* Skeleton = FAutonomixAssetHelper::LoadAssetChecked<USkeleton>(SkeletonPath, SkeletonError);
    if (!Skeleton)
    {
        _ActionLogger.LogError(TEXT("%s"), *SkeletonError);
        return FAutonomixResultHelper::MakeError(TEXT("%s"), *SkeletonError);
    }
    
    // Create AnimBlueprint using factory template
    FAutonomixActionResult Result;
    UAnimBlueprint* AnimBP = FAutonomixAssetFactory::Create<UAnimBlueprint, UAnimBlueprintFactory>(
        AssetPath,
        UAnimBlueprint::StaticClass(),
        [Skeleton](UAnimBlueprintFactory* Factory)
        {
            Factory->TargetSkeleton = Skeleton;
            Factory->ParentClass = UAnimInstance::StaticClass();
        },
        Result
    );
    
    if (!AnimBP)
    {
        _ActionLogger.LogError(TEXT("Failed to create AnimBlueprint"));
        return Result;
    }
    
    // Mark as modified (Create factory already does this, but shown for clarity)
    {
        FAutonomixTransactionGuard Guard(AnimBP, TEXT("Setup AnimBlueprint"));
        // Any post-creation configuration here
    }
    
    _ActionLogger.LogSuccess(TEXT("Created AnimBlueprint: %s"), *AssetPath);
    return Result;
}
---

COMPARISON:
- Lines: 200 → 80 (60% reduction)
- Duplicated patterns: 7 → 0
- Clarity: Medium → High (intent is obvious)
- Testability: Low → High (each component is testable)
- Maintenance: High (duplicated code) → Low (shared utilities)
*/

// ============================================================================
// MIGRATION CHECKLIST FOR EACH HANDLER
// ============================================================================

/*
For each of the 24 action handlers, apply this checklist:

[ ] Replace FScopedTransaction + Modify() with FAutonomixTransactionGuard
[ ] Replace LoadObject<T>() with FAutonomixAssetHelper::LoadAssetChecked<T>()
[ ] Replace LoadObject<UClass>() with FAutonomixAssetHelper::LoadClassChecked()
[ ] Replace FPackageName::GetLongPackagePath/GetShortName with FAutonomixPathHelper/FAutonomixPackageInfo
[ ] Replace ValidateJsonField boilerplate with FAutonomixValidationHelper::ValidateJsonField()
[ ] Replace Result.bSuccess/Result.Errors patterns with FAutonomixResultHelper::MakeError/MakeSuccess()
[ ] Add AUTONOMIX_LOG_ACTION() at start of major functions
[ ] Replace factory boilerplate with FAutonomixAssetFactory::Create<T>() template
[ ] Replace custom WriteFileWithBackup with FAutonomixFileHelper::WriteFileWithBackup()

Priority order for migration:
1. CRITICAL: Blueprint (15 tools, heavy duplication)
2. HIGH: Animation, Material, GAS, BehaviorTree
3. MEDIUM: Mesh, PCG, Sequencer, DataTable
4. LOW: Smaller handlers (Performance, Settings, Validation, etc.)
*/
