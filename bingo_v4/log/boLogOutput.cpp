/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_output.cpp
 * Author: root
 * 
 * Created on 2017年9月2日, 上午10:05
 */

#include "boLogOutput.h"

using namespace bingo::log ;

#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
using namespace std ;

#include <boost/filesystem.hpp>
using namespace boost::filesystem ;

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;
using namespace boost::gregorian ;


// -------------------------------- bo_log_file_output --------------------------- //

void boLogFileOutput::set_path( string& path ) {
        this->path_ = path ;
}

void boLogFileOutput::transfer( string& info ) {
        // If path_ is "", then return.
        if ( path_.length( ) == 0 )
                return ;

        // Whether LOG_DIRECTORY is exist.
        if ( access( path_.c_str( ) , 0 ) == -1 ) {
                // the directory isn't exist.
                // make new directory.
                mkdir( path_.c_str( ) , S_IRWXU + S_IRGRP + S_IXGRP + S_IXOTH ) ;
        }

        // Get filename.
        string filename( path_.c_str( ) ) ;
        filename.append( "/log." ) ;

        // Get file's postfix.
        {
                date now = day_clock::local_day( ) ;
                int year = now.year( ) ;
                int month = now.month( ) ;
                int day = now.day( ) ;
                char cnow[9] ;
                memset( &cnow[0] , 0x00 , 9 ) ;
                sprintf( &cnow[0] , "%4d%02d%02d" , year , month , day ) ;
                filename.append( cnow ) ;
        }

        {
                // Whether file is exist.
                if ( access( filename.c_str( ) , 0 ) == -1 ) {
                        // the file isn't exist.
                        // make new file.

                        ofstream fs( filename.c_str( ) ) ;
                        fs << info.c_str( ) << endl ;
                        fs.close( ) ;

                } else {

                        ofstream fs( filename.c_str( ) , ios_base::app ) ;
                        fs << info.c_str( ) << endl ;
                        fs.close( ) ;
                }
        }
}

// -------------------------------- bo_log_console_output --------------------------- //

void boLogConsoleOutput::transfer( string& info ) {
        cout << info << endl ;
}