/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_to_console.cpp
 * Author: root
 * 
 * Created on 2017年9月2日, 下午12:05
 */

#include "boLogToConsole.h"

using namespace bingo::log ;

boLogToDebug::boLogToDebug( ) :
     boLogHandler( ) ,
     boLogInfoType<boLogSimpleThreadInfo>( ) ,
     boLogOutputType<boLogConsoleOutput>( ) {
}

boLogToDebug::~boLogToDebug( ) {
}

void boLogToDebug::handle( int level , const char* tag , std::string& info ) {
        string msg = make_log_info( level , tag , info ) ;
        transfer( msg ) ;
}

