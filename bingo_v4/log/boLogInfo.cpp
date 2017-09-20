/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_info.cpp
 * Author: root
 * 
 * Created on 2017年9月2日, 上午8:50
 */

#include "boLogInfo.h"

using namespace bingo::log ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

#include "boLogLevel.h"
#include "bingo_v4/thread/boThreadAction.h"

string boLogInfo::get_now( ) {
        ptime p1 = boost::posix_time::microsec_clock::local_time( ) ;
        int year = p1.date( ).year( ) ;
        int month = p1.date( ).month( ) ;
        int day = p1.date( ).day( ) ;

        int hour = p1.time_of_day( ).hours( ) ;
        int minute = p1.time_of_day( ).minutes( ) ;
        int second = p1.time_of_day( ).seconds( ) ;

        int millsecond = p1.time_of_day( ).total_milliseconds( ) - p1.time_of_day( ).total_seconds( )*1000 ;

        char ctime[25] ;
        memset( &ctime[0] , 0x00 , 25 ) ;

        sprintf( &ctime[0] , "%4d-%02d-%02d %02d:%02d:%02d.%03d" , year , month , day , hour , minute , second , millsecond ) ;
        string now ;
        now.append( ctime ) ;

        return now ;
}

// ---------------------------------- bo_log_normal_info ------------------------------ //

string boLogNormalInfo::make_log_info( int level , const char*& tag , string& info ) {

        string now = get_now( ) ;
        string s_level = loglevel_to_string( level ) ;

        string msg ;
        msg.append( now ) ;
        msg.append( "@" ) ;
        msg.append( tag ) ;
        msg.append( "@" ) ;
        msg.append( s_level ) ;
        msg.append( "@" ) ;
        msg.append( info.c_str( ) ) ;

        return msg ;
}

// ---------------------------------- bo_log_normal_thr_info ------------------------------ //

string boLogNormalThreadInfo::make_log_info( int level , const char*& tag , string& info ) {
        string now = get_now( ) ;
        string s_level = loglevel_to_string( level ) ;
        string s_thread_id = ::bo_thread_id( ) ;

        string msg ;
        msg.append( now ) ;
        msg.append( "@" ) ;
        msg.append( s_thread_id ) ;
        msg.append( "@" ) ;
        msg.append( tag ) ;
        msg.append( "@" ) ;
        msg.append( s_level ) ;
        msg.append( "@" ) ;
        msg.append( info.c_str( ) ) ;

        return msg ;
}

// ---------------------------------- bo_log_simple_info ------------------------------ //

string boLogSimpleInfo::make_log_info( int level , const char*& tag , string& info ) {
        string now = get_now( ) ;

        string msg ;
        msg.append( now ) ;
        msg.append( "@" ) ;
        msg.append( info.c_str( ) ) ;

        return msg ;
}

// ---------------------------------- bo_log_simple_thr_info ------------------------------ //

string boLogSimpleThreadInfo::make_log_info( int level , const char*& tag , string& info ) {
        string now = get_now( ) ;
        string s_thread_id = ::bo_thread_id( ) ;

        string msg ;
        msg.append( now ) ;
        msg.append( "@" ) ;
        msg.append( s_thread_id ) ;
        msg.append( "@" ) ;
        msg.append( info.c_str( ) ) ;

        return msg ;
}