/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "boType.h"

#include "log/handler/boLogToConsole.h"
using namespace bingo::log ;

void bo_debug_out( const char* debug_info ) {
        boLogToDebug s ;
        string info = debug_info ;
        s.handle( 0 , "" , info ) ;
}

void bo_debug_out( string& debug_info ) {
        boLogToDebug s ;
        s.handle( 0 , "" , debug_info ) ;
}

void bo_debug_out( stringstream& debug_info ) {
        string out ;
        debug_info >> out ;

        boLogToDebug s ;
        s.handle( 0 , "" , out ) ;
}

string bo_thread_id( ) {
        string out ;
#ifdef WIN32
#else
        stringstream oss ;
        oss << this_thread::get_id( ) ;
        oss >> out ;
#endif
        return out ;
}