# Quick Start: Refactoring Autonomix Actions

**Start Here:** This document gets you up and running in 30 minutes.

---

## Step 1: Verify New Files Compile (5 min)

1. Open your project in Visual Studio
2. Check these files exist:
   ```
   ✅ Source/AutonomixActions/Public/AutonomixActionHelpers.h
   ✅ Source/AutonomixActions/Private/AutonomixActionHelpers.cpp
   ✅ Source/AutonomixCore/Public/AutonomixErrorCodes.h
   ```

3. Compile the project:
   ```bash
   Build\UE5.x\Windows\Build.bat Development Win64 -WaitMutex
   ```
   
   **Expected:** 0 errors, 0 new warnings

---

## Step 2: Understand the New Patterns (10 min)

Read these sections in `REFACTORING_GUIDE.md`:
- [ ] PATTERN 1: Transaction & Asset Modification (FAutonomixTransactionGuard)
- [ ] PATTERN 2: Asset Loading with Error Checking (FAutonomixAssetHelper)
- [ ] PATTERN 3: Package Path Parsing (FAutonomixPathHelper)
- [ ] COMPLETE EXAMPLE: Refactored Handler

Key takeaway: **Old code (200 lines) → New code (80 lines)** with better safety

---

## Step 3: Pick Your First Handler (5 min)

**Recommend: Start with Blueprint handler**

Why? 
- 15 tools (most impactful)
- Most duplicated patterns
- High visibility (frequently used by AI)

Location: `Source/AutonomixActions/Private/Blueprint/AutonomixBlueprintActions.cpp`

---

## Step 4: Do One Refactoring (10 min)

Open `AutonomixBlueprintActions.cpp` and find the first function that does:
1. Asset loading
2. Transactions
3. Error handling

Example to look for:
```cpp
FAutonomixActionResult FAutonomixBlueprintActions::CreateBlueprint(...)
{
    FAutonomixActionResult Result;
    
    // ❌ OLD PATTERN: Manual transaction + Modify + MarkPackageDirty
    FScopedTransaction Transaction(FText::FromString(TEXT("Autonomix Blueprint Action")));
    
    Blueprint->Modify();
    // ... work ...
    Blueprint->GetOutermost()->MarkPackageDirty();
    
    // ❌ OLD PATTERN: Manual error handling
    Result.bSuccess = true;
    Result.ResultMessage = TEXT("Done");
    return Result;
}
```

Replace with:
```cpp
FAutonomixActionResult FAutonomixBlueprintActions::CreateBlueprint(...)
{
    AUTONOMIX_LOG_ACTION("CreateBlueprint");
    
    // ✅ NEW PATTERN: RAII transaction guard
    FAutonomixTransactionGuard Guard(Blueprint, TEXT("Create Blueprint"));
    
    // ... work (destructor handles MarkPackageDirty) ...
    
    // ✅ NEW PATTERN: Helper result
    return FAutonomixResultHelper::MakeSuccess(TEXT("Blueprint created successfully"));
}
```

**That's it!** You've refactored your first pattern.

---

## Step 5: Verify It Works (5 min)

1. Compile:
   ```bash
   Build\UE5.x\Windows\Build.bat Development Win64 -WaitMutex
   ```

2. Check it compiles with **no errors**

3. In Unreal Editor:
   - Open the plugin
   - Test the Blueprint action manually
   - **Verify it works identically to before**

4. Git commit:
   ```bash
   git add Source/AutonomixActions/Private/Blueprint/
   git commit -m "Refactor Blueprint actions to use helper utilities

   - Replaced manual transactions with FAutonomixTransactionGuard
   - Replaced manual errors with FAutonomixResultHelper
   - ~30% code reduction"
   ```

---

## Common Refactoring Tasks (Copy-Paste Ready)

### Task 1: Replace Transaction Pattern

**FIND:**
```cpp
FScopedTransaction Transaction(FText::FromString(TEXT("Autonomix Blueprint Action")));

Blueprint->Modify();
// ... work ...
Blueprint->GetOutermost()->MarkPackageDirty();
```

**REPLACE WITH:**
```cpp
FAutonomixTransactionGuard Guard(Blueprint, TEXT("Action Name"));
// ... work ...
// Guard destructor handles everything
```

---

### Task 2: Replace Asset Loading

**FIND:**
```cpp
USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *SkeletonPath);
if (!Skeleton)
{
    Result.Errors.Add(FString::Printf(TEXT("Skeleton not found: '%s'"), *SkeletonPath));
    return Result;
}
```

**REPLACE WITH:**
```cpp
FString Error;
USkeleton* Skeleton = FAutonomixAssetHelper::LoadAssetChecked<USkeleton>(SkeletonPath, Error);
if (!Skeleton)
{
    return FAutonomixResultHelper::MakeError(TEXT("%s"), *Error);
}
```

---

### Task 3: Replace Result Building

**FIND:**
```cpp
Result.bSuccess = false;
Result.Errors.Add(TEXT("Something failed"));
return Result;
```

**REPLACE WITH:**
```cpp
return FAutonomixResultHelper::MakeError(TEXT("Something failed"));
```

---

### Task 4: Replace Path Parsing

**FIND:**
```cpp
FString PackagePath = FPackageName::GetLongPackagePath(AssetPath);
FString AssetName = FPackageName::GetShortName(AssetPath);
```

**REPLACE WITH:**
```cpp
FAutonomixPackageInfo PackageInfo(AssetPath);
// Use: PackageInfo.LongPath, PackageInfo.ShortName
```

---

### Task 5: Replace Factory Boilerplate

**FIND:**
```cpp
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
```

**REPLACE WITH:**
```cpp
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
```

---

## Refactoring Checklist (Blueprint Handler)

Work through this list systematically:

**Pass 1: Transactions (10 min)**
- [ ] Find all `FScopedTransaction` blocks
- [ ] Replace with `FAutonomixTransactionGuard Guard(...)`
- [ ] Verify compile succeeds

**Pass 2: Asset Loading (10 min)**
- [ ] Find all `LoadObject<...>(...)`
- [ ] Replace with `FAutonomixAssetHelper::LoadAssetChecked<T>()`
- [ ] Verify compile succeeds

**Pass 3: Error Handling (10 min)**
- [ ] Find all `Result.bSuccess = false; Result.Errors.Add(...)`
- [ ] Replace with `return FAutonomixResultHelper::MakeError(...)`
- [ ] Find all `Result.bSuccess = true; return Result`
- [ ] Replace with `return FAutonomixResultHelper::MakeSuccess(...)`
- [ ] Verify compile succeeds

**Pass 4: Path Parsing (5 min)**
- [ ] Find all `FPackageName::GetLongPackagePath` + `GetShortName`
- [ ] Replace with `FAutonomixPackageInfo`
- [ ] Verify compile succeeds

**Pass 5: Factory Creation (10 min)**
- [ ] Find all asset factory boilerplate
- [ ] Replace with `FAutonomixAssetFactory::Create<T>()`
- [ ] Verify compile succeeds

**Pass 6: Cleanup (5 min)**
- [ ] Add `#include "AutonomixActionHelpers.h"` at top if not present
- [ ] Remove now-unused includes
- [ ] Run full compile
- [ ] Manual QA: Test Blueprint creation works

**Total Time: ~50 minutes for Blueprint handler**

Expected result: ~200 lines → ~120 lines (40% reduction)

---

## After One Handler

Once you've successfully refactored Blueprint actions:

1. **Repeat for 4 more high-impact handlers:**
   - Animation (~3-4 hours)
   - Material (~3-4 hours)
   - GAS (~3-4 hours)
   - BehaviorTree (~2-3 hours)

2. **Then the remaining 19 handlers** (lower priority, similar patterns)

3. **See `IMPLEMENTATION_GUIDE.md`** for full timeline and priorities

---

## Troubleshooting

**Q: It won't compile after my changes**
A: 
- Check `#include "AutonomixActionHelpers.h"` is present
- Verify you didn't accidentally delete code
- Clean rebuild: `Clean.bat && Build.bat`

**Q: The refactored code works but looks different**
A: That's expected! The new code is cleaner. Test manually to ensure behavior is identical.

**Q: How do I test if it still works?**
A:
1. In Unreal Editor, use the AI chat to create a Blueprint
2. Verify it creates successfully
3. Verify the Blueprint works correctly
4. Verify you can modify and recompile it

**Q: Can I partially refactor?**
A: Yes! You can refactor one function at a time. Just make sure to compile after each change to catch errors early.

---

## Key Files for Reference

Keep these open while refactoring:

1. **REFACTORING_GUIDE.md** — Detailed before/after examples
2. **AutonomixActionHelpers.h** — API reference (header comments)
3. **AutonomixErrorCodes.h** — Error code reference
4. **Current handler .cpp file** — File you're refactoring

---

## You're Ready!

You have everything needed to make Autonomix production-ready:

✅ New helper system created  
✅ Error codes defined  
✅ Refactoring guide written  
✅ Examples provided  
✅ Timeline planned  

**Start with Blueprint handler today. It'll take ~1 hour and you'll be a refactoring expert.**

---

Questions? Refer to:
- `REFACTORING_GUIDE.md` — Detailed patterns
- `PRODUCTION_READINESS.md` — Full checklist
- `IMPLEMENTATION_GUIDE.md` — Timeline and roadmap
