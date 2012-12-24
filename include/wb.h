///////////////////////////////////////////////////////////////////////////////
//
// wb.h: Header file for Heterogeneous Parallel Programming course (Coursera)
//
// Copyright (c) 2012 Ashwin Nanjappa
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WB
#define WB
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include "cudatimer.h"

////
// Logging
////

enum wbLogLevel
{
    OFF,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
    wbLogLevelNum, //*** Keep this at the end
};

const char* _wbLogLevelStr[] = {
    "Off",
    "Fatal",
    "Error",
    "Warn",
    "Info",
    "Debug",
    "Trace",
    "***InvalidLogLevel***", //*** Keep this at the end
};

const char* _wbLogLevelToStr( wbLogLevel level )
{
    assert( level >= OFF && level <= TRACE );

    return _wbLogLevelStr[ level ];
}

inline void _wbLog( wbLogLevel level, const char* logStr )
{
    std::cout << _wbLogLevelToStr( level ) << " ";
    std::cout << logStr;

    return;
}

inline void wbLog( wbLogLevel level, const char* logStr )
{
    _wbLog( level, logStr );
    std::cout << std::endl;

    return;
}

template < typename T >
inline void wbLog( wbLogLevel level, const char* logStr, T val )
{
    _wbLog( level, logStr );
    std::cout << val << std::endl;

    return;
}

template < typename T1, typename T2 >
inline void wbLog( wbLogLevel level, const char* logStr, T1 v1 , T2 v2 )
{
    _wbLog( level, logStr );
    std::cout << v1 << v2 << std::endl;

    return;
}

template < typename T1, typename T2, typename T3 >
inline void wbLog( wbLogLevel level, const char* logStr, T1 v1, T2 v2, T3 v3 )
{
    _wbLog( level, logStr );
    std::cout << v1 << v2 << v3 << std::endl;

    return;
}

template < typename T1, typename T2, typename T3 , typename T4>
inline void wbLog( wbLogLevel level, const char* logStr, T1 v1, T2 v2, T3 v3, T4 v4 )
{
    _wbLog( level, logStr );
    std::cout << v1 << v2 << v3 << v4 << std::endl;

    return;
}

template < typename T1, typename T2, typename T3 , typename T4,typename T5>
inline void wbLog( wbLogLevel level, const char* logStr, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5 )
{
    _wbLog( level, logStr );
    std::cout << v1 << v2 << v3 << v4 << v5 << std::endl;

    return;
}

////
// Input arguments
////

struct wbArg_t
{
    int    argc;
    char** argv;
};

wbArg_t wbArg_read( int argc, char** argv )
{
    wbArg_t argInfo = { argc, argv };
    return argInfo;
}

char* wbArg_getInputFile( wbArg_t argInfo, int argNum )
{
    assert( argNum >= 0 && argNum < ( argInfo.argc - 1 ) );

    return argInfo.argv[ argNum + 1 ];
}

// mp1 vector read
float* wbImport( char* fname, int* itemNum )
{
    // Open file

    std::ifstream inFile( fname );

    if ( !inFile )
    {
        std::cout << "Error opening input file: " << fname << " !\n";
        exit( 1 );
    }

    // Read file to vector

    std::string sval;
    float fval;
    std::vector< float > fVec;

    while ( inFile >> sval )
    {
        std::istringstream iss( sval );

        iss >> fval;

        fVec.push_back( fval );
    }

    // Vector to malloc memory

    *itemNum = fVec.size();

    float* fBuf = ( float* ) malloc( *itemNum * sizeof( float ) );

    for ( int i = 0; i < *itemNum; ++i )
        fBuf[i] = fVec[i];

    return fBuf;
}

// mp2 matrix 
float* wbImport( char* fname, int* numRows, int* numColumns) {
    // Open file

    std::ifstream inFile( fname );

    if ( !inFile )
    {
        std::cout << "Error opening input file: " << fname << " !\n";
        exit( 1 );
    }

    // Read file to vector

    std::string sval;
    float fval;
    std::vector< float > fVec;

    std::string line;
    int numcolumns = -1;
    int c = 0;
    int numrows = 0;
    while (std::getline(inFile, line)) {
        std::istringstream linestream(line);
        numrows++;
       
        c = 0;
        while(linestream >> fval) {
            fVec.push_back( fval );
            ++c;
        }
        if (numcolumns < 0)
            numcolumns = c;
        else
            assert(numcolumns == c);  // Matrix has all rows the same
    }

    // Vector to malloc memory
    *numRows = numrows; 
    *numColumns = numcolumns;

    float* fBuf = ( float* ) malloc( numrows * numcolumns * sizeof( float ) );

    for ( int i = 0; i < numrows * numcolumns; ++i ) {
        fBuf[i] = fVec[i];
    }

    return fBuf;
}

enum wbTimeType
{
    Generic,
    GPU,
    Compute,
    Copy,
    wbTimeTypeNum, // Keep this at the end
};

const char* wbTimeTypeStr[] = {
    "Generic",
    "GPU    ",
    "Compute",
    "Copy   ",
    "***Invalid***",
};

const char* wbTimeTypeToStr( wbTimeType t )
{
    assert( t >= Generic && t < wbTimeTypeNum );

    return wbTimeTypeStr[t];
}

struct wbTimerInfo
{
    wbTimeType             type;
    std::string            name;
    CudaTimerNS::CudaTimer timer;

    bool operator == ( const wbTimerInfo& t2 ) const
    {
        return ( type == t2.type && ( 0 == name.compare( t2.name ) ) );
    }
};

typedef std::list< wbTimerInfo> wbTimerInfoList;

wbTimerInfoList gTimerInfoList;

void wbTime_start( wbTimeType timeType, const std::string timeStar )
{
    CudaTimerNS::CudaTimer timer;
    timer.start();

    wbTimerInfo tInfo = { timeType, timeStar, timer };

    gTimerInfoList.push_front( tInfo );

    return;
}

void wbTime_stop( wbTimeType timeType, const std::string timeStar )
{
    // Find timer

    const wbTimerInfo searchInfo         = { timeType, timeStar };
    const wbTimerInfoList::iterator iter = std::find( gTimerInfoList.begin(), gTimerInfoList.end(), searchInfo );

    // Stop timer and print time

    wbTimerInfo& timerInfo = *iter;

    timerInfo.timer.stop();
    
    std::cout << "[" << wbTimeTypeToStr( timerInfo.type ) << "] ";
    std::cout << timerInfo.timer.value() << " ";
    std::cout << timerInfo.name << std::endl;

    // Delete timer from list

    gTimerInfoList.erase( iter );

    return;
}

////
// Solution
////

template < typename T, typename S >
void wbSolution( wbArg_t args, const T& t, const S& s )
{
    return;
}

// used for mp1 printing out hostArray 
template < typename T>
void wbSolution( wbArg_t args, const T& t, const int c )
{
    for (int i = 0; i < c; ++i)
        std::cout << t[i] << ' ';
    std::cout << std::endl;

    return;
}

// used for mp2 printing out hostMatrix 
template < typename T>
void wbSolution( wbArg_t args, const T& t, const int numRows, const int numColumns )
{
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numColumns; ++j)
            std::cout << t[i*numColumns + j] << ' ';
        std::cout << std::endl;
    }

    return;
}
#endif  // ifndef WB
///////////////////////////////////////////////////////////////////////////////
