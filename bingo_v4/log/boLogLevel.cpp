/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "boLogLevel.h"

using namespace bingo::log ;

string bingo::log::loglevel_to_string( int level ) {
        string out ;
        switch ( level ) {
                case LOG_LEVEL_OFF:
                        out.append( "off" ) ;
                        break ;
                case LOG_LEVEL_FATAL:
                        out.append( "fatal" ) ;
                        break ;
                case LOG_LEVEL_ERROR:
                        out.append( "error" ) ;
                        break ;
                case LOG_LEVEL_INFO:
                        out.append( "info" ) ;
                        break ;
                case LOG_LEVEL_DEBUG:
                        out.append( "debug" ) ;
                        break ;
                case LOG_LEVEL_ALL:
                        out.append( "all" ) ;
                        break ;
                default:
                        out.append( "unknow" ) ;
        }

        return out ;
}

int bingo::log::string_to_loglevel( string& s ) {
        if ( s.compare( "off" ) == 0 ) {
                return LOG_LEVEL_OFF ;

        } else if ( s.compare( "fatal" ) == 0 ) {
                return LOG_LEVEL_FATAL ;

        } else if ( s.compare( "error" ) == 0 ) {
                return LOG_LEVEL_ERROR ;

        } else if ( s.compare( "info" ) == 0 ) {
                return LOG_LEVEL_INFO ;

        } else if ( s.compare( "debug" ) == 0 ) {
                return LOG_LEVEL_DEBUG ;

        } else if ( s.compare( "all" ) == 0 ) {
                return LOG_LEVEL_ALL ;

        } else
                return LOG_LEVEL_UNKNOWN ;
}