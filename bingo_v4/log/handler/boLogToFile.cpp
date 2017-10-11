/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_file.cpp
 * Author: root
 * 
 * Created on 2017年9月2日, 上午8:34
 */

#include "boLogToFile.h"

using namespace bingo::log ;

boLogToFile::boLogToFile( ) :
     boLogHandler( ) ,
     boLogInfoType<>( ) ,
     boLogOutputType<>( ) {
}

boLogToFile::~boLogToFile( ) {
}

void boLogToFile::handle( int level , const char* tag , std::string& info ) {
        if ( level <= this->level_ ) {
                string msg = make_log_info( level , tag , info ) ;
                transfer( msg ) ;
        }
}

void boLogToFile::set_path( string& path ) {
        this->get_output_type( ).set_path( path ) ;
}

