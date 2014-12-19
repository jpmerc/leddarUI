//CODE BY JEAN-PHILIPPE MERCIER, UNIVERSITE LAVAL
//INSPIRED BY LEDDARTECH SDK EXAMPLE (http://www.leddartech.com/support/downloads/9-leddar-evaluation-kit)

#include <stdio.h>
#include <ctype.h>
#include <ctime>
#include <math.h>
#include <iostream>
#include <string.h>
#include "LeddarC.h"
#include "LeddarProperties.h"
#include "LeddarResults.h"

#include "UI/mainwindow.h"
#include <QtGui/QApplication>
#include <unistd.h>

#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDir>

#include "leddarThread.h"

#define ARRAY_LEN( a )  (sizeof(a)/sizeof(a[0]))

struct CONFIG_PARAMS {
    double OVERSAMPLING_EXPONENT;
    double ACCUMULATION_EXPONENT;
    double BASE_POINT_COUNT;
    double AUTOMATIC_LED_INTENSITY;
    double LED_INTENSITY;
    double CHANGE_DELAY;
    double THRESHOLD_OFFSET;
    double LOOP_TIME;
};


// Function declarations
static unsigned char DataCallback( void *aHandle, unsigned int aLevels );
static void CheckError( int aCode );
static void startLeddarData();
static void stopLeddarData();
static void setupLeddar();
static QVector<CONFIG_PARAMS> readConfigurationFile(QFile *file);
static bool checkConfigVectorValidity(CONFIG_PARAMS param_set);
static void setLeddarParameters(CONFIG_PARAMS param_set);
static QString getLogFileName();
static void printConfigParam(CONFIG_PARAMS config);


// Global variable to avoid passing to each function.
static LeddarHandle gHandle=NULL;
static MainWindow* w_;
static QVector<CONFIG_PARAMS> g_param_vector;
static int g_param_vector_index;
static QVector<double> x_data;
static leddarTimer *round_robin_timer;
static leddarTimer *log_file_timer;
static QFile *LogFile = 0;
static QMutex LogFileMutex;
static int data_callback_called = 0;
static QMutex data_callback_called_mutex;

leddarThread::leddarThread(MainWindow *in_w, QApplication *in_app, QObject *parent) :
    QThread(parent)
{
    w = in_w;
    app = in_app;
}

void leddarThread::run(){
    // LOOP PINGING THE SENSOR (ENDS THE PROGRAM IF NOT PINGABLE FOR A CERTAIN TIME)
    int ping_fail = 0;
    while(true){
        bool connected = (LeddarPing( gHandle ) == LD_SUCCESS);
        if(!connected) ping_fail++;
        if(ping_fail >= 5) break;
        LeddarSleep( 0.5 );
    }

    std::cout << "The leddar has been disconnected. Program will shutdown!" << std::endl;

    stopLeddarData();
    this->exit();
    app->exit();

}

leddarTimer::leddarTimer(int timerNumber, QObject *parent) : QTimer(parent) {
    if(timerNumber == 0){
        connect(this, SIGNAL(timeout()), this, SLOT(RoundRobinSlot()));
    }

    else if(timerNumber == 1){
        connect(this, SIGNAL(timeout()), this, SLOT(LogFileSlot()));
    }

    else{
        connect(this, SIGNAL(timeout()), this, SLOT(pingLeddar()));
    }
}

void leddarTimer::RoundRobinSlot() {
    qDebug() << "Current date and time:" << QDateTime::currentDateTime().toString();
    round_robin_timer->stop();

    // increment g_param_vector_index or restart at the beggining if all parameters have been tested
    int g_param_vector_size = g_param_vector.size();
    g_param_vector_index++;
    if(g_param_vector_index >= g_param_vector_size && g_param_vector_size > 0){
        g_param_vector_index = 0;
    }

    CONFIG_PARAMS parameters = g_param_vector.at(g_param_vector_index);
    setLeddarParameters(parameters);

}

void leddarTimer::LogFileSlot() {
    qDebug() << "Current date and time:" << QDateTime::currentDateTime().toString();

    LogFileMutex.lock();
    LogFile->close();
    LogFile = new QFile(getLogFileName());
    LogFile->open(QIODevice::WriteOnly | QIODevice::Text);
    LogFileMutex.unlock();

}

void leddarTimer::pingLeddar() {
    //qDebug() << "Current date and time:" << QDateTime::currentDateTime().toString();
    bool connected = (LeddarPing( gHandle ) == LD_SUCCESS);
    std::cout << "Leddar Ping Status = " << connected << std::endl;

    bool connected2 = LeddarGetConnected(gHandle);
    std::cout << "Leddar Connected Status = " << connected2 << std::endl;

    data_callback_called_mutex.lock();
    if(data_callback_called == 0){
        std::cout << "The callback is not responding." << std::endl;
        std::cout << "Restarting the leddar..." << std::endl;
        stopLeddarData();
        LeddarDestroy( gHandle );
        setupLeddar();
        startLeddarData();
        data_callback_called = 0;
    }
    data_callback_called_mutex.unlock()


}

static void printConfigParam(CONFIG_PARAMS config){

    std::cout << std::endl;
    std::cout << " OVERSAMPLING_EXPONENT : "    << config.OVERSAMPLING_EXPONENT     << std::endl;
    std::cout << " ACCUMULATION_EXPONENT : "    << config.ACCUMULATION_EXPONENT     << std::endl;
    std::cout << " BASE_POINT_COUNT : "         << config.BASE_POINT_COUNT          << std::endl;
    std::cout << " AUTOMATIC_LED_INTENSITY : "  << config.AUTOMATIC_LED_INTENSITY   << std::endl;
    std::cout << " LED_INTENSITY : "            << config.LED_INTENSITY             << std::endl;
    std::cout << " CHANGE_DELAY : "             << config.CHANGE_DELAY              << std::endl;
    std::cout << " THRESHOLD_OFFSET : "         << config.THRESHOLD_OFFSET          << std::endl;
    std::cout << " EXECUTION TIME (s) : "       << config.LOOP_TIME                 << std::endl;
    std::cout << std::endl;

}


static void startLeddarData(){
    CheckError( LeddarStartDataTransfer( gHandle, LDDL_DETECTIONS ) );
    CheckError( LeddarAddCallback( gHandle, DataCallback, gHandle ) );
}

static void stopLeddarData(){
    LeddarStopDataTransfer( gHandle );
    LeddarRemoveCallback( gHandle, DataCallback, gHandle );
}


static void setupLeddar(){
    gHandle = LeddarCreate();

    char lAddress[24];
    lAddress[0] = 0;

    if ( LeddarConnect( gHandle, lAddress ) == LD_SUCCESS ){
        if( LeddarGetConnected( gHandle ) ){
            puts( "\nConnection established!" );
        }
    }

    else{
        puts( "\nConnection failed!" );
    }
}


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
    data_callback_called_mutex.lock();
    data_callback_called++;
    data_callback_called_mutex.unlock();

    LdDetection lDetections[50];
    unsigned int i, j, lCount = LeddarGetDetectionCount( aHandle );
    //std::cout << std::endl << " lCount = " << lCount << std::endl;

    // Loop to get wich segment has returned a distance
    QVector<double> index_data;
    double new_index[16] = {0};
    double old_index[16] = {-1};
    for(int i = 0; i < 16; i++){
        LeddarGetResult(aHandle, 3, i, new_index);
        if(new_index[0] != old_index[0]){
            index_data.push_back(new_index[0]);
            old_index[0] = new_index[0];
        }
        else{
            break;
        }
    }

    if ( lCount > ARRAY_LEN( lDetections ) )
    {
        lCount = ARRAY_LEN( lDetections );
    }

    LeddarGetDetections( aHandle, lDetections, ARRAY_LEN( lDetections ) );


    // Place the distance and amplitude at the good segment index
    QVector<double> y_distance(QVector<double>(16));
    QVector<double> y_amplitude(QVector<double>(16));
    for( i=0, j=0; (i<lCount) && (j<16); ++i )
    {
        y_distance.replace(index_data.at(i),lDetections[i].mDistance);
        y_amplitude.replace(index_data.at(i),lDetections[i].mAmplitude);
        ++j;
    }

    //    for(int i = 0; i < 16; i++){
    //        printf( "%5.2f ", y_distance.at(i) );
    //    }
    //    std::cout << std::endl;

    //    for(int i = 0; i < 16; i++){
    //        printf( "%5.2f ", y_amplitude.at(i) );
    //    }
    //    std::cout << std::endl;


    // Send data to GUI
    w_->setData(&x_data, &y_distance);


    // Get the temperature of the sensor (celsius) and the LED intensity (useful if leddar is in automatic mode)
    double temp_celsius[1] = {0};
    double led_intensity[1] = {0};
    LeddarGetResult(aHandle, 5, 0, temp_celsius);
    LeddarGetResult(aHandle, 6, 0, led_intensity);

    //    std::cout << " Temperature = " << temp_celsius[0] << std::endl;
    //    std::cout << " LED intensity = " << led_intensity[0] << std::endl;


    // ***************************************LOG DATA************************************************
    LogFileMutex.lock();


    if(LogFile && y_distance.size() > 0){

        QTextStream out(LogFile);

        // Write the time the data was logged
        QDateTime date_time;
        QString date_time_string = date_time.currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
        out << date_time_string << ",";

        std::cout << "leddartech.cpp : " ;

        //Write the distance
        for(int i = 0; i < 16; i++){
            out << y_distance.at(i) << ",";
            std::cout << y_distance.at(i) << ",";
        }

        std::cout << std::endl;

        //Write the amplitude
        for(int i = 0; i < 16; i++){
            out << y_amplitude.at(i) << ",";
        }

        // Write the parameters
        CONFIG_PARAMS p = g_param_vector.at(g_param_vector_index);
        out << p.OVERSAMPLING_EXPONENT << ",";
        out << p.ACCUMULATION_EXPONENT << ",";
        out << p.BASE_POINT_COUNT << ",";
        out << p.AUTOMATIC_LED_INTENSITY << ",";
        if(p.AUTOMATIC_LED_INTENSITY == 1){out << p.CHANGE_DELAY << ",";}
        else{out << p.LED_INTENSITY << ",";}
        out << p.THRESHOLD_OFFSET << ",";
        out << p.LOOP_TIME << ",";

        // Write Temperature and LED Intensity
        out << temp_celsius[0] << ",";
        out << led_intensity[0] << ",";
        out << "\n";
    }

    LogFileMutex.unlock();

    return 1;
}


static QVector<CONFIG_PARAMS> readConfigurationFile(QFile *file){

    QVector<CONFIG_PARAMS> param_vector;

    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        line.remove(QString(" "));
        line.remove(QString("\t"));
        if(line.size() > 0 && line.at(0)!='#'){
            QStringList list = line.split(",", QString::SkipEmptyParts);
            int list_size = list.size();
            if(list_size == 7){
                CONFIG_PARAMS param_set;
                param_set.OVERSAMPLING_EXPONENT     = trunc(list.at(0).toDouble());
                param_set.ACCUMULATION_EXPONENT     = trunc(list.at(1).toDouble());
                param_set.BASE_POINT_COUNT          = trunc(list.at(2).toDouble());
                param_set.AUTOMATIC_LED_INTENSITY   = trunc(list.at(3).toDouble());
                param_set.LED_INTENSITY             = trunc(list.at(4).toDouble()); //only one of them is used
                param_set.CHANGE_DELAY              = trunc(list.at(4).toDouble()); //only one of them is used
                param_set.THRESHOLD_OFFSET          = list.at(5).toDouble();
                param_set.LOOP_TIME                 = list.at(6).toDouble();

                bool valid_params = checkConfigVectorValidity(param_set);
                if(valid_params){
                    param_vector.push_back(param_set);
                }

                else{
                    std::cout << line.toAscii().data() <<"  :  Parameters are invalid! Please refer to config/config.txt for more information." << std::endl;
                }
            }
        }
    }

    return param_vector;
}

static bool checkConfigVectorValidity(CONFIG_PARAMS param_set){

    //printConfigParam(param_set);

    if(param_set.OVERSAMPLING_EXPONENT < 0 || param_set.OVERSAMPLING_EXPONENT > 3){
        return false;
    }

    if(param_set.ACCUMULATION_EXPONENT < 0 || param_set.ACCUMULATION_EXPONENT > 10){
        return false;
    }

    if(!(param_set.AUTOMATIC_LED_INTENSITY == 0 || param_set.AUTOMATIC_LED_INTENSITY == 1)){
        return false;
    }

    if(param_set.AUTOMATIC_LED_INTENSITY == 0){
        if(param_set.LED_INTENSITY < 0 || param_set.LED_INTENSITY > 16){
            return false;
        }
    }

    if(param_set.LOOP_TIME < 0){
        return false;
    }


    return true;
}

static void setLeddarParameters(CONFIG_PARAMS param_set){

    printConfigParam(param_set);

    CheckError( LeddarSetProperty( gHandle, PID_OVERSAMPLING_EXPONENT, 0, param_set.OVERSAMPLING_EXPONENT ) );
    CheckError( LeddarSetProperty( gHandle, PID_ACCUMULATION_EXPONENT, 0, param_set.ACCUMULATION_EXPONENT ) );
    CheckError( LeddarSetProperty( gHandle, PID_BASE_POINT_COUNT, 0, param_set.BASE_POINT_COUNT ) );
    CheckError( LeddarSetProperty( gHandle, PID_AUTOMATIC_LED_INTENSITY, 0, param_set.AUTOMATIC_LED_INTENSITY ) );


    // For updating the legend in the GUI
    QVector<double> parameter_legend;
    parameter_legend.push_back(param_set.OVERSAMPLING_EXPONENT);
    parameter_legend.push_back(param_set.ACCUMULATION_EXPONENT);
    parameter_legend.push_back(param_set.BASE_POINT_COUNT);
    parameter_legend.push_back(param_set.AUTOMATIC_LED_INTENSITY);

    if(param_set.AUTOMATIC_LED_INTENSITY == 1){
        CheckError( LeddarSetProperty( gHandle, PID_CHANGE_DELAY, 0, param_set.CHANGE_DELAY ) );
        parameter_legend.push_back(param_set.CHANGE_DELAY);
    }
    else{
        CheckError( LeddarSetProperty( gHandle, PID_LED_INTENSITY, 0, param_set.LED_INTENSITY ) );
        parameter_legend.push_back(param_set.LED_INTENSITY);
    }

    CheckError( LeddarSetProperty( gHandle, PID_THRESHOLD_OFFSET, 0, param_set.THRESHOLD_OFFSET ) );
    parameter_legend.push_back(param_set.THRESHOLD_OFFSET);

    // Save Modifications
    CheckError( LeddarWriteConfiguration( gHandle ) );

    parameter_legend.push_back(param_set.LOOP_TIME);
    w_->updateParametersLegend(parameter_legend);



    // Configure timer
    round_robin_timer->start(1000*param_set.LOOP_TIME);

}

static QString getLogFileName(){
    // file name will be of the form : ../log/yyyy-mm-dd-hh-mm.txt
    if(!QDir("../log/").exists()){
        QDir().mkdir("../log/");
    }
    QDateTime date_time;
    QString date_time_string = date_time.currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    date_time_string.prepend("../log/");
    date_time_string.append(".txt");
    std::cout << "Writing to file : " << date_time_string.toAscii().data() << std::endl;
    return date_time_string;
}

// *****************************************************************************
// Function: main
//
/// \brief   Standard C entry point!
// *****************************************************************************

int main(int argc, char** argv){

    QApplication a(argc, argv);
    //MainWindow w;
    w_ = new MainWindow();

    w_->show();

    // Initialize vector
    for(int i = 0; i < 16; i++){
        x_data.push_back(i);
    }

    // READ CONFIG FILE
    QFile configFile("../config/config.txt");
    g_param_vector = readConfigurationFile(&configFile);
    g_param_vector_index = 0;

    if(g_param_vector.size() <= 0){
        return 0;
    }

    // CONNECT LEDDAR AND REGISTER CALLBACK FUNCTION
    setupLeddar();
    startLeddarData();

    // SET TIMERS AND PARAMETERS
    round_robin_timer = new leddarTimer(0);
    CONFIG_PARAMS initial_parameters = g_param_vector.at(g_param_vector_index);
    setLeddarParameters(initial_parameters);
    //config_round_robin_timer = new QTimer(&a);
    //config_round_robin_timer->start(100);

    // SET LOG FILE AND TIMER TO CHANGE LOG FILE EVERY 20 MINUTES
    log_file_timer = new leddarTimer(1);
    log_file_timer->start(1000*60*20);
    LogFile = new QFile(getLogFileName());
    LogFile->open(QIODevice::WriteOnly | QIODevice::Text);


    leddarTimer *ping_timer = new leddarTimer(2);
    ping_timer->start(1000);

    a.exec();

    stopLeddarData();
    LeddarDestroy( gHandle );

    return 0;
}


