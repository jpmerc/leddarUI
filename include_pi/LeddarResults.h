// *****************************************************************************
// Module..: Leddar -- Common classes that can be used by many applications
//           and product variants that use the LeddarTM technology.
//
/// \file    LeddarResults.h
///
/// \brief   Definition of results ids.
///
/// \author  Louis Perreault
///
/// \since   June 2013
//
// Platform: Win32
//
// Copyright (c) 2013-2014 LeddarTech Inc. All rights reserved.
// *****************************************************************************

#pragma once

enum LdResults
{
    RID_OVERSAMPLING = 1,
    RID_ACCUMULATION,
    RID_DETECTION_SEGMENT,
    RID_DETECTION_DISTANCE,
    RID_TEMPERATURE,
    RID_LED_INTENSITY,
    RID_GAIN_ENABLED,
    RID_OUTPUT_STATE
};

// End of file LeddarResults.h