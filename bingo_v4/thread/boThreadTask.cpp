/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_thread_task.cpp
 * Author: root
 * 
 * Created on 2017年9月2日, 上午11:03
 */

#include "boThreadTask.h"

using namespace bingo::thread ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

// ---------------------- boThreadTask ----------------------------------- //

boThreadTask::boThreadTask( boThreadTaskData* exit_thr_flag ) :
     is_thread_exit_( false ) ,
     thr_( bind( &boThreadTask::svc , this ) ) {

        // Save exit_thread_flag.
        exit_thread_flag_ = exit_thr_flag ;
}

boThreadTask::~boThreadTask( ) {
}

int boThreadTask::put( boThreadTaskData* data , boErrorWhat& e_what ) {
        {
                // lock part field.
                BO_SCOPE_LOCK_TYPE lock( mu_ ) ;

                // Check is_thread_exit, the value is true that thread exit.
                if ( is_thread_exit_ ) {
                        e_what.err_no( THREAD_ERROR_TYPE_THREAD_HAS_EXITED ) ;
                        e_what.err_message( "thread has exit!" ) ;

                        return -1 ;
                }

                // Condition is satisfy, then stop to wait.
                queue_.push( data ) ;
        }

        // notify to read data from queue.
        cond_get_.notify_one( ) ;

        return 0 ;
}

void boThreadTask::svc( ) {
        while ( true ) {

                // lock part field.
                BO_SCOPE_LOCK_TYPE lock( mu_ ) ;

                // Check buffer whether empty.
                bool is_empty = ( queue_.empty( ) ) ? true : false ;
                while ( is_empty ) {
                        if ( is_thread_exit_ ) {
#ifdef BINGO_DEBUG_THREAD_TASK
                                bo_debug_out( "is_thread_exit_ is true, thread will exist" ) ;
#endif
                                return ;
                        }

#ifdef BINGO_DEBUG_THREAD_TASK
                        bo_debug_out( "start cond_get_.wait(mu_)!" ) ;
#endif
                        // wait for notify.
                        cond_get_.wait( mu_ ) ;

                        is_empty = ( queue_.empty( ) ) ? true : false ;
                }

                // Condition is satisfy, then stop to wait.
                // TASK_DATA top from queue.
                boThreadTaskData* data = queue_.front( ) ;
                queue_.pop( ) ;


#ifdef BINGO_DEBUG_THREAD_TASK
                boStringAppend ap ;
                ap.add( "queue pop succes! data:" )->add( data->to_string( ) ) ;
                bo_debug_out( ap.to_string( ) ) ;
#endif 

                if ( is_exit_data( data ) ) {
                        is_thread_exit_ = true ;
                } else {
                        top( data ) ;
                }

                free_data( data ) ;
        }
}

void boThreadTask::free_data( boThreadTaskData*& data ) {
        delete data ;
        data = 0 ;
}

void boThreadTask::exit_task( ) {
        // Send message to exit thread.
        boErrorWhat e_what ;
        int suc = -1 ;

        do {
                suc = put( exit_thread_flag_ , e_what ) ;
                if ( suc == -1 && e_what.err_no( ) == THREAD_ERROR_TYPE_THREAD_HAS_EXITED )
                        break ;
        } while ( suc == -1 ) ;

        // Block until the thread exit.
        if ( suc == 0 ) {
#ifdef BINGO_DEBUG_THREAD_TASK
                bo_debug_out( "wait for thread exit,  ~bo_thread_task()" ) ;
#endif
                thr_.join( ) ;
#ifdef BINGO_DEBUG_THREAD_TASK
                bo_debug_out( "thread exit,  ~bo_thread_task()" ) ;
#endif
        } else {
                free_data( exit_thread_flag_ ) ;
        }
}