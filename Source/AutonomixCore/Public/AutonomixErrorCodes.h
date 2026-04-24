// Copyright Autonomix. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutonomixErrorCodes.generated.h"

/**
 * Comprehensive error code system for Autonomix.
 * Ensures consistent error reporting, logging, and user feedback.
 * 
 * Error codes are categorized by domain and numbered sequentially within each domain:
 * - 1000-1099:   General/Core errors
 * - 2000-2099:   Asset loading/management errors
 * - 3000-3099:   Validation errors
 * - 4000-4099:   File operation errors
 * - 5000-5099:   Blueprint-specific errors
 * - 6000-6099:   Material-specific errors
 * - 7000-7099:   C++ code generation errors
 * - 8000-8099:   Animation system errors
 * - 9000-9099:   Safety/Security errors
 * - 10000-10099: LLM/API errors
 * - 11000-11099: Context/Memory errors
 */

UENUM(BlueprintType)
enum class EAutonomixErrorCode : int32
{
	// ========================================================================
	// GENERAL / CORE ERRORS (1000-1099)
	// ========================================================================
	
	/** Operation completed successfully */
	Success = 0,

	/** Unknown/unclassified error */
	UnknownError = 1000,

	/** Operation was cancelled by user */
	Cancelled = 1001,

	/** Operation timed out */
	Timeout = 1002,

	/** Operation not implemented */
	NotImplemented = 1003,

	/** Invalid parameters provided */
	InvalidParameters = 1004,

	/** Operation not permitted in current mode */
	OperationNotPermitted = 1005,

	// ========================================================================
	// ASSET LOADING & MANAGEMENT ERRORS (2000-2099)
	// ========================================================================

	/** Asset not found at specified path */
	AssetNotFound = 2000,

	/** Asset path is invalid or malformed */
	InvalidAssetPath = 2001,

	/** Asset is of wrong type (e.g., expected Blueprint, got Material) */
	AssetTypeMismatch = 2002,

	/** Failed to load asset */
	AssetLoadFailed = 2003,

	/** Asset is read-only and cannot be modified */
	AssetReadOnly = 2004,

	/** Asset is already locked by another operation */
	AssetLocked = 2005,

	/** Failed to create asset package */
	PackageCreationFailed = 2006,

	/** Failed to save asset */
	AssetSaveFailed = 2007,

	/** Asset is in use (e.g., open in editor) */
	AssetInUse = 2008,

	// ========================================================================
	// VALIDATION ERRORS (3000-3099)
	// ========================================================================

	/** Required JSON field missing */
	MissingJsonField = 3000,

	/** JSON field has wrong type */
	WrongJsonFieldType = 3001,

	/** Array field is empty when non-empty required */
	EmptyArrayField = 3002,

	/** String field is empty when non-empty required */
	EmptyStringField = 3003,

	/** Numeric field out of valid range */
	NumericFieldOutOfRange = 3004,

	/** Invalid file path provided */
	InvalidFilePath = 3005,

	/** Directory does not exist */
	DirectoryNotFound = 3006,

	// ========================================================================
	// FILE OPERATION ERRORS (4000-4099)
	// ========================================================================

	/** Failed to read file */
	FileReadFailed = 4000,

	/** Failed to write file */
	FileWriteFailed = 4001,

	/** File does not exist */
	FileNotFound = 4002,

	/** File is read-only */
	FileReadOnly = 4003,

	/** Failed to create directory */
	DirectoryCreationFailed = 4004,

	/** Failed to delete file/directory */
	DeletionFailed = 4005,

	/** Failed to copy file */
	FileCopyFailed = 4006,

	/** Failed to back up file */
	BackupFailed = 4007,

	/** Insufficient disk space */
	InsufficientDiskSpace = 4008,

	/** File path is too long */
	FilePathTooLong = 4009,

	// ========================================================================
	// BLUEPRINT-SPECIFIC ERRORS (5000-5099)
	// ========================================================================

	/** Blueprint parent class not found */
	BlueprintParentNotFound = 5000,

	/** Failed to create Blueprint */
	BlueprintCreationFailed = 5001,

	/** Blueprint compilation failed */
	BlueprintCompilationFailed = 5002,

	/** Failed to inject Blueprint nodes (T3D format error) */
	BlueprintNodeInjectionFailed = 5003,

	/** Pin connection failed (type mismatch, pin not found, etc.) */
	PinConnectionFailed = 5004,

	/** Node not found in Blueprint */
	NodeNotFound = 5005,

	/** Pin not found on node */
	PinNotFound = 5006,

	/** Component not found in Blueprint */
	ComponentNotFound = 5007,

	/** Variable not found in Blueprint */
	VariableNotFound = 5008,

	/** Function not found in Blueprint */
	FunctionNotFound = 5009,

	/** Graph not found in Blueprint */
	GraphNotFound = 5010,

	/** T3D format parsing failed */
	T3DParsingFailed = 5011,

	/** GUID placeholder resolution failed */
	GUIDPlaceholderResolutionFailed = 5012,

	/** DAG layout algorithm failed */
	DAGLayoutFailed = 5013,

	// ========================================================================
	// MATERIAL-SPECIFIC ERRORS (6000-6099)
	// ========================================================================

	/** Failed to create Material */
	MaterialCreationFailed = 6000,

	/** Material compilation failed */
	MaterialCompilationFailed = 6001,

	/** Texture not found for Material */
	TextureNotFound = 6002,

	/** Material graph node not found */
	MaterialNodeNotFound = 6003,

	/** Material parameter not found */
	MaterialParameterNotFound = 6004,

	/** Failed to set material parameter */
	MaterialParameterSetFailed = 6005,

	// ========================================================================
	// C++ CODE GENERATION ERRORS (7000-7099)
	// ========================================================================

	/** Failed to create C++ class files */
	CppClassCreationFailed = 7000,

	/** Generated C++ code failed to compile */
	CppCompilationFailed = 7001,

	/** Failed to apply C++ code diff */
	CppDiffApplyFailed = 7002,

	/** C++ code contains unsafe patterns */
	CppCodeUnsafe = 7003,

	/** Failed to parse existing C++ file */
	CppParsingFailed = 7004,

	/** Header file not found */
	HeaderFileNotFound = 7005,

	/** Source file not found */
	SourceFileNotFound = 7006,

	/** Build.cs file not found */
	BuildCsFileNotFound = 7007,

	// ========================================================================
	// ANIMATION SYSTEM ERRORS (8000-8099)
	// ========================================================================

	/** Failed to create Animation Blueprint */
	AnimBlueprintCreationFailed = 8000,

	/** Animation compilation failed */
	AnimCompilationFailed = 8001,

	/** Skeleton not found for Animation Blueprint */
	SkeletonNotFound = 8002,

	/** Animation sequence not found */
	AnimSequenceNotFound = 8003,

	/** Montage not found */
	MontageNotFound = 8004,

	// ========================================================================
	// SAFETY & SECURITY ERRORS (9000-9099)
	// ========================================================================

	/** Operation blocked by safety gate (insufficient permissions) */
	SafetyGateRejected = 9000,

	/** File is protected and cannot be modified */
	ProtectedFileModification = 9001,

	/** Operation exceeds safety limits */
	SafetyLimitExceeded = 9002,

	/** Insufficient approval level */
	InsufficientApprovalLevel = 9003,

	/** File pattern matches ignore list */
	FileIgnored = 9004,

	// ========================================================================
	// LLM / API ERRORS (10000-10099)
	// ========================================================================

	/** API key is missing or invalid */
	MissingAPIKey = 10000,

	/** API request failed */
	APIRequestFailed = 10001,

	/** API returned error response */
	APIErrorResponse = 10002,

	/** API rate limit exceeded */
	APIRateLimitExceeded = 10003,

	/** API response parsing failed */
	APIResponseParsingFailed = 10004,

	/** API timeout */
	APITimeout = 10005,

	/** LLM provider not configured */
	LLMProviderNotConfigured = 10006,

	/** Model not available */
	ModelNotAvailable = 10007,

	// ========================================================================
	// CONTEXT / MEMORY ERRORS (11000-11099)
	// ========================================================================

	/** Context window exceeded */
	ContextWindowExceeded = 11000,

	/** Token limit exceeded */
	TokenLimitExceeded = 11001,

	/** Failed to condense context */
	ContextCondensationFailed = 11002,

	/** Insufficient memory for operation */
	InsufficientMemory = 11003,

	/** Checkpoint not found */
	CheckpointNotFound = 11004,

	/** Checkpoint restore failed */
	CheckpointRestoreFailed = 11005,

	/** Checkpoint corruption detected */
	CheckpointCorrupted = 11006
};

/**
 * Mapping from error codes to user-friendly messages and recovery hints.
 */
namespace AutonomixErrorMessages
{
	/**
	 * Get user-friendly error message for an error code.
	 * @param ErrorCode The error code
	 * @return Human-readable error description
	 */
	FORCEINLINE FString GetErrorMessage(EAutonomixErrorCode ErrorCode)
	{
		switch (ErrorCode)
		{
			// General
			case EAutonomixErrorCode::Success:
				return TEXT("Operation completed successfully");
			case EAutonomixErrorCode::UnknownError:
				return TEXT("An unknown error occurred");
			case EAutonomixErrorCode::Cancelled:
				return TEXT("Operation was cancelled");
			case EAutonomixErrorCode::Timeout:
				return TEXT("Operation timed out");
			case EAutonomixErrorCode::InvalidParameters:
				return TEXT("Invalid parameters provided");

			// Asset loading
			case EAutonomixErrorCode::AssetNotFound:
				return TEXT("Asset not found at specified path");
			case EAutonomixErrorCode::InvalidAssetPath:
				return TEXT("Asset path is invalid or malformed");
			case EAutonomixErrorCode::AssetTypeMismatch:
				return TEXT("Asset is of unexpected type");
			case EAutonomixErrorCode::AssetReadOnly:
				return TEXT("Asset is read-only and cannot be modified");

			// Validation
			case EAutonomixErrorCode::MissingJsonField:
				return TEXT("Required JSON field is missing");
			case EAutonomixErrorCode::WrongJsonFieldType:
				return TEXT("JSON field has wrong type");
			case EAutonomixErrorCode::InvalidFilePath:
				return TEXT("File path is invalid");

			// File operations
			case EAutonomixErrorCode::FileNotFound:
				return TEXT("File does not exist");
			case EAutonomixErrorCode::FileReadFailed:
				return TEXT("Failed to read file (check permissions)");
			case EAutonomixErrorCode::FileWriteFailed:
				return TEXT("Failed to write file (check disk space and permissions)");
			case EAutonomixErrorCode::InsufficientDiskSpace:
				return TEXT("Insufficient disk space for operation");

			// Blueprint
			case EAutonomixErrorCode::BlueprintParentNotFound:
				return TEXT("Blueprint parent class not found (verify parent class path)");
			case EAutonomixErrorCode::BlueprintCompilationFailed:
				return TEXT("Blueprint failed to compile (check logs for details)");
			case EAutonomixErrorCode::PinConnectionFailed:
				return TEXT("Failed to connect Blueprint pins (verify pin types match)");
			case EAutonomixErrorCode::T3DParsingFailed:
				return TEXT("T3D format parsing failed (Blueprint text format corrupted)");

			// Safety
			case EAutonomixErrorCode::SafetyGateRejected:
				return TEXT("Operation blocked by safety gate (check permissions)");
			case EAutonomixErrorCode::ProtectedFileModification:
				return TEXT("Cannot modify protected file (system files are read-only)");

			// LLM
			case EAutonomixErrorCode::MissingAPIKey:
				return TEXT("API key is missing (configure provider settings)");
			case EAutonomixErrorCode::APIRateLimitExceeded:
				return TEXT("API rate limit exceeded (wait before retrying)");
			case EAutonomixErrorCode::APITimeout:
				return TEXT("API request timed out (network issue?)");

			// Context
			case EAutonomixErrorCode::ContextWindowExceeded:
				return TEXT("Context window exceeded (try condensing conversation)");
			case EAutonomixErrorCode::TokenLimitExceeded:
				return TEXT("Token limit exceeded for this provider");

			default:
				return FString::Printf(TEXT("Error code %d"), (int32)ErrorCode);
		}
	}

	/**
	 * Get recovery hint for an error code.
	 * @param ErrorCode The error code
	 * @return Actionable recovery suggestion
	 */
	FORCEINLINE FString GetRecoveryHint(EAutonomixErrorCode ErrorCode)
	{
		switch (ErrorCode)
		{
			case EAutonomixErrorCode::AssetNotFound:
				return TEXT("Check the asset path is correct. Use Content Browser to verify the asset exists.");
			
			case EAutonomixErrorCode::BlueprintCompilationFailed:
				return TEXT("Check the Blueprint compiler output for syntax errors. Verify all node pins are connected correctly.");
			
			case EAutonomixErrorCode::FileWriteFailed:
				return TEXT("Verify you have write permissions to the directory and sufficient disk space.");
			
			case EAutonomixErrorCode::SafetyGateRejected:
				return TEXT("This file is protected. Switch to 'Developer' security mode if you need to modify it.");
			
			case EAutonomixErrorCode::MissingAPIKey:
				return TEXT("Go to Project Settings > Autonomix and configure your API key for the selected provider.");
			
			case EAutonomixErrorCode::ContextWindowExceeded:
				return TEXT("Your conversation is too long. Use the 'Condense' button to summarize old messages.");
			
			case EAutonomixErrorCode::APIRateLimitExceeded:
				return TEXT("You've hit the API rate limit. Wait a few minutes before trying again, or reduce your request frequency.");
			
			default:
				return TEXT("Check the logs for more details. If the problem persists, contact support.");
		}
	}

	/**
	 * Get severity level for error code (0=info, 1=warning, 2=error, 3=critical)
	 */
	FORCEINLINE int32 GetSeverity(EAutonomixErrorCode ErrorCode)
	{
		if (ErrorCode == EAutonomixErrorCode::Success)
			return 0;
		
		if (ErrorCode >= 9000 && ErrorCode < 10000)  // Safety errors
			return 3;
		
		if (ErrorCode >= 10000)  // API/LLM errors
			return 2;
		
		return 2;  // Most others are errors
	}
}

/**
 * Helper to create structured error results with error codes.
 */
struct FAutonomixErrorInfo
{
	EAutonomixErrorCode Code;
	FString Message;
	FString Details;
	FString RecoveryHint;
	FString StackTrace;

	FAutonomixErrorInfo(EAutonomixErrorCode InCode = EAutonomixErrorCode::UnknownError)
		: Code(InCode)
		, Message(AutonomixErrorMessages::GetErrorMessage(InCode))
		, RecoveryHint(AutonomixErrorMessages::GetRecoveryHint(InCode))
	{
	}

	explicit operator bool() const { return Code != EAutonomixErrorCode::Success; }
};
