// *****************************************************************************
// Module..: Leddar -- Common classes that can be used by many applications
//           and product variants that use the LeddarTM technology.
//
/// \file    LeddarProperties.h
///
/// \brief   Definition of property ids.
//
//  \author  Louis Perreault
//
//  \since   June 2013
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

enum LdProperties
{
    PID_LED_INTENSITY = 1, // RW
    PID_OVERSAMPLING_EXPONENT, // RW
    PID_OVERSAMPLING, // R
    PID_ACCUMULATION_EXPONENT, // RW
    PID_ACCUMULATION, // R
    PID_BASE_POINT_COUNT, // RW
    PID_THRESHOLD_OFFSET, // RW
    PID_SEGMENT_LEFT, // R
    PID_SEGMENT_RIGHT, // R
    PID_SEGMENT_TOP, // R
    PID_SEGMENT_BOTTOM, // R
    PID_ZONE_MIN_DISTANCE, // R
    PID_ZONE_MAX_DISTANCE, // R
    PID_ZONE_LEFT_LIMIT, // R
    PID_ZONE_RIGHT_LIMIT, // R
    PID_ZONE_X_LEFT, // R
    PID_ZONE_X_RIGHT, // R
    PID_ZONE_ACTIVE, // R
    PID_SENSOR_HEIGHT, // R
    PID_REFERENCE_LINE_DISTANCE, // R
    PID_GLOBAL_TRANSFORM, // R
    PID_INVERSE_TRANSFORM, // R
    PID_RECEIVER_BOARD_VERSION, // R
    PID_ZONE_NEAR_LIMIT, // RW
    PID_ZONE_FAR_LIMIT, // RW
    PID_ZONE_ENABLED, // RW
    PID_ZONE_SEGMENT_ENABLED, // RW
    PID_ZONE_RISING_DEBOUNCE, // RW
    PID_ZONE_FALLING_DEBOUNCE, // RW
    PID_OUTPUT_PNP, // RW
    PID_OUTPUT_INVERTED, // RW
    PID_TEACH_MARGIN, // RW
    PID_TEACH_STATE, // R
    PID_AUTOMATIC_LED_INTENSITY, // RW
    PID_CHANGE_DELAY, // RW
    PID_OBJECT_DEMERGING, // RW
    PID_MEASUREMENT_RATE, // RW
    PID_NAME // RW
};

// Standard measurement rates to use with PID_MEASUREMENT_RATE. 
#define LD_MEASUREMENT_RATE_1_5625 1.5625
#define LD_MEASUREMENT_RATE_3_125  3.125
#define LD_MEASUREMENT_RATE_6_25   6.25
#define LD_MEASUREMENT_RATE_12_5   12.5
#define LD_MEASUREMENT_RATE_25     25
#define LD_MEASUREMENT_RATE_50     50

// End of file LeddarProperties.h