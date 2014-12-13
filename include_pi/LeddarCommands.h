// *****************************************************************************
// Module..: Leddar -- Common classes that can be used by many applications
//           and product variants that use the LeddarTM technology.
//
/// \file    LeddarCommands.h
///
/// \brief   Definition of command ids.
//
//  \author  Louis Perreault
//
//  \since   October 2013
//
// Platform: Win32
//
// Copyright (c) 2013 LeddarTech Inc. All rights reserved.
// Information contained herein is or may be confidential and proprietary to
// LeddarTech inc. Prior to using any part of the software development kit
// accompanying this notice, you must accept and agree to be bound to the
// terms of the LeddarTech Inc. license agreement accompanying this file.
// *****************************************************************************

#pragma once

enum LdCommands
{
    CID_START_TEACH = 1,
    CID_SET_QUICK_LIMIT = 2
};

// Constants for the second parameter of CID_SET_QUICK_LIMIT.
#define SQL_NEAR_PERPENDICULAR 0
#define SQL_NEAR_RADIAL        1
#define SQL_FAR_PERPENDICULAR  2
#define SQL_FAR_RADIAL         3

// End of file LeddarCommands.h
