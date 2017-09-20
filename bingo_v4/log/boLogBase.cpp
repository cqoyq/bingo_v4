/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_base.cpp
 * Author: root
 * 
 * Created on 2017年9月1日, 下午5:20
 */

#include "boLogBase.h"

using namespace bingo::log ;

boLogBase::boLogBase( ) {
}

boLogBase::~boLogBase( ) {
}

void boLogBase::set_log_environment( const char* tag , boLogHandler*& logger ) {
        log_tag_ = tag ;
        logger_ = logger ;
}

void boLogBase::to_debug( string& info ) {
        if ( logger_ )
                logger_->handle( LOG_LEVEL_DEBUG , log_tag_.c_str( ) , info ) ;
}

void boLogBase::to_error( string& info ) {
        if ( logger_ )
                logger_->handle( LOG_LEVEL_ERROR , log_tag_.c_str( ) , info ) ;
}

void boLogBase::to_info( string& info ) {
        if ( logger_ )
                logger_->handle( LOG_LEVEL_INFO , log_tag_.c_str( ) , info ) ;
}
