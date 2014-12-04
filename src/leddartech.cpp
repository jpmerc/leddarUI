// *****************************************************************************
// Module..: SDK -- Software development kit for Leddar products.
//
/// \file    Main.c
///
/// \brief   Simple console program demonstrating the use of LeddarC functions.
///
// Platform: Win32, Linux
//
// Copyright (c) 2013-2014 LeddarTech Inc. All rights reserved.
// Information contained herein is or may be confidential and proprietary to
// LeddarTech inc. Prior to using any part of the software development kit
// accompanying this notice, you must accept and agree to be bound to the
// terms of the LeddarTech Inc. license agreement accompanying this file.
// *****************************************************************************

// *****************************************************************************
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! WARNING !
// ! To keep the example simple, scanf is used to input values and printf to
// ! output. If you use non-ASCII characters for your paths, this may not work
// ! correctly on all operating systems (for example Windows prefers to use
// ! wchar_t for this). So in a real application you may have to use other
// ! functions for I/O and conversion function to/from UTF-8.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// *****************************************************************************


//#include <ros/ros.h>
//#include <sensor_msgs/LaserScan.h>
//#include <angles/angles.h>

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "LeddarC.h"
#include "LeddarProperties.h"


#define ARRAY_LEN( a )  (sizeof(a)/sizeof(a[0]))

// Global variable to avoid passing to each function.
static LeddarHandle gHandle=NULL;


//sensor_msgs::LaserScan constructLeddarMessage(std::vector<double> data);
//ros::Publisher leddar_publisher;
//int leddar_sequence_number = 0;


// *****************************************************************************
// Function: CheckError
//
/// \brief   Check a return code and if it is not success, display an error
///          message corresponding to the code.
///
/// \param   aCode  The result code to verify.
// *****************************************************************************

static void
CheckError( int aCode )
{
    if ( aCode != LD_SUCCESS )
    {
        LtChar lMessage[200];

        LeddarGetErrorMessage( aCode, lMessage, ARRAY_LEN( lMessage ) );
        LeddarPrintf( LTS( "LeddarC error (%d): %s\n" ), aCode, lMessage );
    }
}

// *****************************************************************************
// Function: WaitKey
//
/// \brief   Wait for a key to be pressed on the keyboard, pinging the sensor
///          to keep the connection alive while waiting.
///
/// \return  The character corresponding to the key pressed (converted to
///          uppercase for letters).
// *****************************************************************************

static char
WaitKey( void )
{
    // LeddarGetKey is blocking so we need to wait for a key to be pressed
    // before calling it.
    while( !LeddarKeyPressed() )
    {
        // If a live connection is active we need to ping it periodically.
        if ( LeddarGetConnected( gHandle ) )
        {
            if ( LeddarPing( gHandle ) != LD_SUCCESS )
            {
                return 0;
            }
        }

        LeddarSleep( 0.5 );
    }

    return toupper( LeddarGetKey() );
}

// *****************************************************************************
// Function: DataCallback
//
/// \brief   This is the function that is called when a new set of data is
///          available. Here we simply display the first 12 detections.
///
/// \param   aHandle  This is the user data parameter that was passed to
///                   LeddarAddCallback. Here by design we know its the handle.
/// \param   aLevels  A bitmask of the data levels received in that frame.
///
/// \return  Non zero to be called again (responding 0 would remove this
///          function from the callback list).
// *****************************************************************************

static unsigned char
DataCallback( void *aHandle, unsigned int aLevels )
{
    LdDetection lDetections[50];
    unsigned int i, j, lCount = LeddarGetDetectionCount( aHandle );

    if ( lCount > ARRAY_LEN( lDetections ) )
    {
        lCount = ARRAY_LEN( lDetections );
    }

    LeddarGetDetections( aHandle, lDetections, ARRAY_LEN( lDetections ) );

    // When replaying a record, display the current index
    if ( LeddarGetRecordSize( gHandle ) != 0 )
    {
        printf( "%6d ", LeddarGetCurrentRecordIndex( gHandle ) );
    }

//    std::vector<double> leddar_data;
    for( i=0, j=0; (i<lCount) && (j<16); ++i )
    {
        printf( "%5.2f ", lDetections[i].mDistance );
//        leddar_data.push_back(lDetections[i].mDistance);
        ++j;
    }
    puts( "" );


//    sensor_msgs::LaserScan message = constructLeddarMessage(leddar_data);
//    leddar_publisher.publish(message);


    return 1;
}

// *****************************************************************************
// Function: ReadLiveData
//
/// \brief   Start data transfer until a key is pressed and stop it (data is
///          displayed by the callback).
// *****************************************************************************

static void
ReadLiveData( void )
{
    puts( "\nPress a key to start reading data and press a key again to stop." );
    WaitKey();

    CheckError( LeddarStartDataTransfer( gHandle, LDDL_DETECTIONS ) );
    CheckError( LeddarAddCallback( gHandle, DataCallback, gHandle ) );

    WaitKey();

    LeddarStopDataTransfer( gHandle );
    LeddarRemoveCallback( gHandle, DataCallback, gHandle );
}

// *****************************************************************************
// Function: ReplayData
//
/// \brief   Navigation through a record file to display the data (data is
///          displayed by the callback).
// *****************************************************************************

static void
ReplayData( void )
{
    puts( "\nP to go forward, O to go backward, H to return to beginning, Q to quit" );

    CheckError( LeddarStartDataTransfer( gHandle, LDDL_DETECTIONS ) );
    CheckError( LeddarAddCallback( gHandle, DataCallback, gHandle ) );

    for(;;)
    {
        char lChoice = WaitKey();

        switch( lChoice )
        {
            case 'H':
                LeddarMoveRecordTo( gHandle, 0 );
                break;
            case 'O':
                CheckError( LeddarStepBackward( gHandle ) );
                break;
            case 'P':
                CheckError( LeddarStepForward( gHandle ) );
                break;
            case 'Q':
            case  27: // Escape
                LeddarStopDataTransfer( gHandle );
                LeddarRemoveCallback( gHandle, DataCallback, gHandle );
                return;
        }
    }
}

// *****************************************************************************
// Function: ReadConfiguration
//
/// \brief   Display some parameters of the current configuration.
// *****************************************************************************

static void
ReadConfiguration( void )
{
    double lValue;
    char   lValueStr[64];

    puts( "\nCurrent Configuration:\n" );
    CheckError( LeddarGetTextProperty( gHandle, PID_NAME, 0, lValueStr, sizeof(lValueStr) ) );
    printf( "  Device Name     : %s\n", lValueStr );
    CheckError( LeddarGetProperty( gHandle, PID_OVERSAMPLING, 0, &lValue ) );
    printf( "  Oversampling    : %.0f\n", lValue );
    CheckError( LeddarGetProperty( gHandle, PID_ACCUMULATION, 0, &lValue ) );
    printf( "  Accumulations   : %.0f\n", lValue );
    CheckError( LeddarGetProperty( gHandle, PID_BASE_POINT_COUNT, 0, &lValue ) );
    printf( "  Base Point Count: %.0f\n", lValue );
    CheckError( LeddarGetProperty( gHandle, PID_LED_INTENSITY, 0, &lValue ) );
    printf( "  Led Intensity   : %.0f\n", lValue );
    CheckError( LeddarGetProperty( gHandle, PID_THRESHOLD_OFFSET, 0, &lValue ) );
    printf( "  Threshold offset: %.2f\n", lValue );

    puts( "\nPress a key to continue." );
    WaitKey();
}

// *****************************************************************************
// Function: ConfigurationMenu
//
/// \brief   Menu allowing the change of configuration parameters.
// *****************************************************************************

static void
ConfigurationMenu( void )
{
    while( LeddarGetConnected( gHandle ) )
    {
        char         lChoice;
        unsigned int lId = 0;
        unsigned int lType = 1;

        puts( "\nConfiguration Change Menu" );
        puts( "  1. Change Oversampling Exponent" );
        puts( "  2. Change Accumulation Exponent" );
        puts( "  3. Change Base Point Count" );
        puts( "  4. Change Led Intensity" );
        puts( "  5. Change Threshold Offset" );
        puts( "  6. Change Name" );
        puts( "  7. Write" );
        puts( "  8. Restore" );
        puts( "  9. Quit" );

        lChoice = WaitKey();

        switch( lChoice )
        {
            case '1':
                lId = PID_OVERSAMPLING_EXPONENT;
                break;
            case '2':
                lId = PID_ACCUMULATION_EXPONENT;
                break;
            case '3':
                lId = PID_BASE_POINT_COUNT;
                break;
            case '4':
                lId = PID_LED_INTENSITY;
                break;
            case '5':
                lId = PID_THRESHOLD_OFFSET;
                break;
            case '6':
                lId = PID_NAME;
                lType = 2;
                break;
            case '7':
                CheckError( LeddarWriteConfiguration( gHandle ) );
                break;
            case '8':
                CheckError( LeddarRestoreConfiguration( gHandle ) );
                break;
            case '9':
            case  27: // Escape
                if ( !LeddarGetConfigurationModified( gHandle ) )
                {
                    return;
                }

                puts( "\n** Configuration modified, please Write or Restore before quitting **" );
                break;
        }

        if ( lId != 0 )
        {
            printf( "\nEnter new value: " );

            switch( lType )
            {
                case 1:
                {
                    double lValue;

                    scanf( "%lf", &lValue );
                    CheckError( LeddarSetProperty( gHandle, lId, 0, lValue ) );
                }
                    break;
                case 2:
                {
                    char lValue[64];

                    scanf( "%63s", lValue );
                    CheckError( LeddarSetTextProperty( gHandle, lId, 0, lValue ) );
                }
                    break;
            }
        }
    }
}

// *****************************************************************************
// Function: ConnectMenu
//
/// \brief   Main menu when a live connection is made.
///
/// \param   aTrySingleUsb  If true we will try to connect to a single USB
///                         sensor by sending an empty string as the address.
///                         This works only if there is 1 and only 1 USB sensor
///                         plugged to the PC.
// *****************************************************************************

static void
ConnectMenu( int aTrySingleUsb )
{
    char lAddress[24];

    if ( aTrySingleUsb )
    {
        lAddress[0] = 0;
    }
    else
    {
        // Ask for address and try to connect before displaying menu.
        printf( "\nEnter address: " );
        scanf( "%24s", lAddress );
    }

    if ( LeddarConnect( gHandle, lAddress ) == LD_SUCCESS )
    {
        while( LeddarGetConnected( gHandle ) )
        {
            char lChoice;

            puts( "\n\nConnected Menu" );
            puts( "  1. Read Data" );
            puts( "  2. Read Configuration" );
            puts( "  3. Change Configuration" );
            if ( LeddarGetRecording( gHandle ) )
            {
                puts( "  4. Stop Recording" );
            }
            else
            {
                puts( "  4. Start Recording" );
            }
            puts( "  5. Disconnect" );

            lChoice = WaitKey();

            switch( lChoice )
            {
                case '1':
                    ReadLiveData();
                    break;
                case '2':
                    ReadConfiguration();
                    break;
                case '3':
                    ConfigurationMenu();
                    break;
                case '4':
                    if ( LeddarGetRecording( gHandle ) )
                    {
                        LeddarStopRecording( gHandle );
                    }
                    else
                    {
                        CheckError( LeddarStartRecording( gHandle ) );
                    }
                    break;
                case '5':
                case  27:
                    LeddarDisconnect( gHandle );
                    return;
            }
        }
    }
    else
    {
        puts( "\nConnection failed!" );
    }
}

// *****************************************************************************
// Function: ReplayMenu
//
/// \brief   Main menu when a replay a record file.
// *****************************************************************************

static void
ReplayMenu( void )
{
    LtChar lName[256];

    // Ask for file name and try to load record before display menu.
    printf( "\nEnter file name: " );
    LeddarScanf( LTS( "%255s" ), lName );

    if ( LeddarLoadRecord( gHandle, lName ) == LD_SUCCESS )
    {
        puts( "\nPlease wait while the record is loading..." );

        // For a big file, especially if it is on a network drive, it may
        // take a while before the replay is 100% ready. Note that you
        // can still use the replay but it will not report the complete
        // size until it is finished loading.
        while( LeddarGetRecordLoading( gHandle ) )
        {
            LeddarSleep( 0.5 );
        }

        printf( "Finished loading record of %d frames.\n",
                LeddarGetRecordSize( gHandle ) );

        for(;;)
        {
            char lChoice;

            puts( "\nReplay Menu" );
            puts( "  1. Read Data" );
            puts( "  2. Read Configuration" );
            puts( "  3. Close" );

            lChoice = WaitKey();

            switch( lChoice )
            {
                case '1':
                    ReplayData();
                    break;
                case '2':
                    ReadConfiguration();
                    break;
                case '3':
                case  27:
                    LeddarDisconnect( gHandle );
                    return;
            }
        }
    }
    else
    {
        puts( "\nFailed to load file!" );
    }
}

// *****************************************************************************
// Function: ConfigureRecordingMenu
//
/// \brief   Menu allowing for configuration of data recording.
// *****************************************************************************

static void
ConfigureRecordingMenu( void )
{
    for(;;)
    {
        int    lChoice;
        LtChar lPath[256];

        puts( "\nConfigure Recording Menu" );
        LeddarGetRecordingDirectory( lPath, ARRAY_LEN(lPath) );
        LeddarPrintf( LTS( "  1. Change directory (%s)\n" ), lPath );
        printf( "  2. Change max file size (%dMB)\n", LeddarGetMaxRecordFileSize() );
        puts( "  3. Quit" );

        lChoice = toupper( LeddarGetKey() );

        switch( lChoice )
        {
            case '1':
                printf( "\nEnter recording directory: " );
                LeddarScanf( LTS( "%255s" ), lPath );
                LeddarConfigureRecording( lPath, 0, 0 );
                break;
            case '2':
                printf( "\nEnter max file size in MB: " );
                scanf( "%d", &lChoice );
                LeddarConfigureRecording( NULL, lChoice, 0 );
                break;
            case '3':
            case  27:
                return;
        }
    }
}

// *****************************************************************************
// Function: ListSensors
//
/// \brief   List the address of all sensors available.
// *****************************************************************************

static void
ListSensors( void )
{
    char         lAddresses[256];
    unsigned int lCount = sizeof(lAddresses);
    unsigned int lIndex = 0;

    puts( "\nScanning for available sensors, please wait..." );

    CheckError( LeddarListSensors( lAddresses, &lCount, 2000 ) );

    printf( "Found %d sensors\n", lCount );

    while( strlen( lAddresses+lIndex ) > 0 )
    {
        printf( "%s\n", lAddresses + lIndex );
        lIndex += strlen( lAddresses+lIndex ) + 1;
    }
}

// *****************************************************************************
// Function: MainMenu
//
/// \brief   Display and responds to the main menu.
// *****************************************************************************

static void
MainMenu( void )
{
    for(;;)
    {
        int lChoice;

        puts( "\n\nMain Menu" );
        puts( "  1. Connect" );
        puts( "  2. Connect to single USB sensor" );
        puts( "  3. List Sensors" );
        puts( "  4. Replay Record" );
        puts( "  5. Configure Recording" );
        puts( "  6. Quit" );

        lChoice = toupper( LeddarGetKey() );

        switch( lChoice )
        {
            case '1':
                ConnectMenu( 0 );
                break;
            case '2':
                ConnectMenu( 1 );
                break;
            case '3':
                ListSensors();
                break;
            case '4':
                ReplayMenu();
                break;
            case '5':
                ConfigureRecordingMenu();
                break;
            case '6':
            case 'Q':
            case  27:
                puts( "\n*** Goodbye! ***" );
                return;
        }
    }
}


//sensor_msgs::LaserScan constructLeddarMessage(std::vector<double> data){

//    sensor_msgs::LaserScan scan_message;
//    scan_message.header.frame_id    = "leddar_base_link";
//    scan_message.header.stamp       = ros::Time::now();
//    scan_message.header.seq         = leddar_sequence_number;
//    scan_message.angle_min          = angles::from_degrees(-22.5);
//    scan_message.angle_max          = angles::from_degrees(22.5);
//    scan_message.angle_increment    = angles::from_degrees(45/16);
//    scan_message.range_min          = 0;
//    scan_message.range_max          = 50;

//    std::cout << "Data size = " << data.size() << std::endl;

//    for(int i=0; i < data.size() ; i++){
//        scan_message.ranges.push_back(data.at(i));
//    }

//    std::cout << scan_message << std::endl;

//    leddar_sequence_number++;

//    return scan_message;
//}

// *****************************************************************************
// Function: main
//
/// \brief   Standard C entry point!
// *****************************************************************************

int main(int argc, char** argv){

//    ros::init (argc, argv, "leddartech_node");
//    ros::NodeHandle n;

//    leddar_publisher = n.advertise<sensor_msgs::LaserScan>(std::string("leddar_scan"), 1);


    puts( "*************************************************" );
    puts( "* Welcome to the LeddarC Demonstration Program! *" );
    puts( "*************************************************" );

    gHandle = LeddarCreate();

    MainMenu();

    LeddarDestroy( gHandle );

    return 0;
}

// End of file Main.c
