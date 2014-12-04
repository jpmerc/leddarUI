// *****************************************************************************
// Module..: SDK -- Software development kit for Leddar products.
//
/// \file    LeddarC.h
///
/// \brief   Declarations for the C interface of the Leddar SDK.
///
//  \author  Louis Perreault
// 
//  \since   July 2013
//
// Platform: Win32, Linux
//
// Copyright (c) 2013-2014 LeddarTech Inc. All rights reserved.
// Information contained herein is or may be confidential and proprietary to
// LeddarTech inc. Prior to using any part of the software development kit
// accompanying this notice, you must accept and agree to be bound to the
// terms of the LeddarTech Inc. license agreement accompanying this file.
// *****************************************************************************

#pragma once

#ifdef WIN32
  #ifdef LEDDARC_EXPORTS
    #define LEDDARC_DLL __declspec( dllexport )
  #else
    #define LEDDARC_DLL __declspec( dllimport )
  #endif
  typedef wchar_t LtChar;
  #define LeddarPrintf wprintf
  #define LeddarScanf  wscanf
  #define LTS( S )     L##S
#else
  #define LEDDARC_DLL __attribute__ ((visibility ("default")))
  typedef char LtChar;
  #define LeddarPrintf printf
  #define LeddarScanf  scanf
  #define LTS( S )     S
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// Result codes that can be returned from LeddarC functions.
// 0 is success, negative is a failure, positive is success
// with warning.
#define LD_START_OF_FILE      (-8)
#define LD_END_OF_FILE        (-7)
#define LD_NO_RECORD          (-6)
#define LD_ALREADY_STARTED    (-5)
#define LD_NO_DATA_TRANSFER   (-4)
#define LD_NOT_CONNECTED      (-3)
#define LD_INVALID_ARGUMENT   (-2)
#define LD_ERROR              (-1)
#define LD_SUCCESS            0
#define LD_NOT_ENOUGH_SPACE   1

typedef void *LeddarHandle;
typedef unsigned char  LeddarBool;
typedef unsigned short LeddarU16;
typedef unsigned int   LeddarU32;

typedef LeddarBool (*LdCallback)( void *, LeddarU32 );

enum LdDataLevels
{
    LDDL_NONE = 0,
    LDDL_STATE = 1,
    LDDL_DETECTIONS = 2
};

typedef struct _LdDetection
{
    float     mDistance;
    float     mAmplitude;
    LeddarU16 mSegment;
    LeddarU16 mFlags;
} LdDetection;

LEDDARC_DLL void
LeddarGetVersion( char *aBuffer, size_t aLength );

LEDDARC_DLL LeddarHandle
LeddarCreate( void );

LEDDARC_DLL void
LeddarDestroy( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarGetConnected( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarGetConfigurationModified( LeddarHandle aHandle );

LEDDARC_DLL size_t
LeddarGetRecordSize( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarGetRecordLoading( LeddarHandle aHandle );

LEDDARC_DLL size_t
LeddarGetCurrentRecordIndex( LeddarHandle aHandle );

LEDDARC_DLL unsigned int
LeddarGetDetectionCount( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarGetRecording( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarGetRecordingDirectory( LtChar *aValue, LeddarU32 aLength );

LEDDARC_DLL LeddarU32
LeddarGetMaxRecordFileSize( void );

LEDDARC_DLL LeddarU32
LeddarGetRecordingLevels( void );

LEDDARC_DLL int
LeddarGetProperty( LeddarHandle aHandle, unsigned int aId, 
                   unsigned int aIndex, double *aValue );

LEDDARC_DLL int
LeddarGetTextProperty( LeddarHandle aHandle, unsigned int aId,
                       unsigned int aIndex, char *aValue,
                       size_t aValueLen );

LEDDARC_DLL int
LeddarSetProperty( LeddarHandle aHandle, unsigned int aId,
                   unsigned int aIndex, double aValue );

LEDDARC_DLL int
LeddarSetTextProperty( LeddarHandle aHandle, unsigned aId,
                       unsigned int aIndex, const char *aValue );

LEDDARC_DLL int
LeddarGetDetections( LeddarHandle aHandle, LdDetection *aDetections,
                     unsigned int aLength );

LEDDARC_DLL int
LeddarGetResult( LeddarHandle aHandle, unsigned int aId,
                 unsigned int aIndex, double *aValue );

LEDDARC_DLL int
LeddarExecuteCommand( LeddarHandle aHandle, unsigned int aId, ... );

LEDDARC_DLL int
LeddarListSensors( char *aAddresses, unsigned int *aSize,
                   unsigned int aTimeout );

LEDDARC_DLL int
LeddarConnect( LeddarHandle aHandle, const char *aAddress );

LEDDARC_DLL void
LeddarDisconnect( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarLoadRecord( LeddarHandle aHandle, const LtChar *aFileName );

LEDDARC_DLL int
LeddarPing( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarStartDataTransfer( LeddarHandle aHandle, LeddarU32 aLevels );

LEDDARC_DLL void
LeddarStopDataTransfer( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarAddCallback( LeddarHandle aHandle, LdCallback aCallback,
                   void *aUserData );

LEDDARC_DLL int
LeddarRemoveCallback( LeddarHandle aHandle, LdCallback aCallback,
                      void *aUserData );

LEDDARC_DLL int
LeddarWriteConfiguration( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarRestoreConfiguration( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarConfigureRecording( const LtChar *aDirectory,
                          unsigned int aMaxFileSize,
                          LeddarU32 aLevels );

LEDDARC_DLL int
LeddarStartRecording( LeddarHandle aHandle );

LEDDARC_DLL void
LeddarStopRecording( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarStepForward( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarStepBackward( LeddarHandle aHandle );

LEDDARC_DLL int
LeddarMoveRecordTo( LeddarHandle aHandle, unsigned int aIndex );

LEDDARC_DLL int
LeddarGetErrorMessage( int aCode, LtChar *aBuffer, size_t aLength );

// Some OS abstraction functions.

LEDDARC_DLL LeddarBool
LeddarKeyPressed( void );

LEDDARC_DLL int
LeddarGetKey( void );

LEDDARC_DLL void
LeddarSleep( double aSeconds );

#ifdef __cplusplus
}
#endif

// End of file LeddarC.h
