/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_timer.cpp
 * Author: root
 * 
 * Created on 2017年9月3日, 下午5:36
 */

#include "boTimer.h"

using namespace bingo ;

#ifdef WIN32
#else

boTimer::boTimer( BO_ASYNC_SERVICE& io_service ) : timer_( io_service ) {
}
#endif

boTimer::~boTimer( ) {
}

void boTimer::start( time_escape& time ) {

        interval_time_.type = time.type ;
        interval_time_.interval = time.interval ;


#ifdef WIN32
#else
        if ( interval_time_.type == TIMER_ESCAPE_TYPE_SECOND ) {
                boost::posix_time::seconds s( interval_time_.interval ) ;
                boost::posix_time::time_duration td = s ;
                timer_.expires_from_now( td ) ;
        } else if ( interval_time_.type == TIMER_ESCAPE_TYPE_MILLISECOND ) {
                boost::posix_time::milliseconds s( interval_time_.interval ) ;
                boost::posix_time::time_duration td = s ;
                timer_.expires_from_now( td ) ;
        }

        timer_.async_wait( bind( &boTimer::on_timeout ,
                                 this ,
                                 boost::asio::placeholders::error ) ) ;
#endif
}

#ifdef WIN32
#else

void boTimer::on_timeout( const system::error_code& ec ) {
        if ( !ec ) {
                this->timeout_handler( interval_time_ ) ;
                start( interval_time_ ) ;
        }
}
#endif

