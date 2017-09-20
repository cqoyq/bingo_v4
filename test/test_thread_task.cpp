/*
 * test_string_ex.cpp
 *
 *  Created on: 2016-9-13
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include "../bingo_v4/boSingleton.h"
#include "../bingo_v4/boString.h"
#include "../bingo_v4/boType.h"
#include "../bingo_v4/thread/boThreadBigDataTask.h"
#include "../bingo_v4/thread/boThreadTask.h"
using namespace bingo::thread ;
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

BOOST_AUTO_TEST_SUITE( test_thread_task )

// Test thread big-data task, two thread send and one thread receive, receive number is right.

u32_t in_err_num = 0 ;
u32_t in_max_num = 65535 ;
u32_t rev_max_num = 65535 * 2 ;
//u32_t in_max_num = 1 ;
//u32_t rev_max_num = 1 ;
string start_timestamp ;
string end_timestamp ;

string get_now( ) {
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

class thr_task_data : public boThreadTaskData {
public:

        string data ; // Save stream data.

        thr_task_data( string& v ) {
                data.append( v.c_str( ) , v.length( ) ) ;
        }

        virtual ~thr_task_data( ) {
        }

        string to_string( ) override {
                boString t ;
                string out = t.stream_to_string( data.c_str( ) , data.length( ) ) ;
                return out ;
        }

} ;

class thr_task_handler : public boThreadBigDataTask {
public:

        thr_task_handler( thr_task_data* exit_flag , BO_ASYNC_SERVICE& io_service ) :
             boThreadBigDataTask( exit_flag , io_service ) {

        } ;

        virtual ~thr_task_handler( ) {
                exit_task( ) ;
        } ;

        void top( boThreadTaskData*& data ) override {
                //          boStringAppend ap ;
                //          boString t ;
                //          ap.add( "top data_size:" )->add( data_size ) ;
                //
                //          ::g_debug_out( ap.to_string( ) ) ;

                thr_task_data* p = dynamic_cast < thr_task_data* > ( data ) ;
                if ( p ) {
                        size_t data_size = p->data.length( ) ;

                        int max = data_size / 5 ;
                        for ( int i = 0 ; i < max ; i++ ) {
                                //               const char* header = data + i * 5 ;
                                //
                                //               boStringAppend ap ;
                                //               boString t ;
                                //               ap.add( "top data:" )->add( t.stream_to_string( header , 5 ) ) ->add( ",data_size:5 ,nCount:" )->add( nCount ) ;
                                //
                                //               ::g_debug_out( ap.to_string( ) ) ;

                                // Save last rev time.

                                if ( nCount == ( rev_max_num - 1 ) ) {
                                        end_timestamp = get_now( ) ;
                                }

                                nCount++ ;
                        }
                } else {
                        boStringAppend ap ;
                        ap.add( "call top(), p is null" ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }

        }

        bool is_exit_data( boThreadTaskData*& data ) override {

                thr_task_data* p = dynamic_cast < thr_task_data* > ( data ) ;
                if ( p ) {
                        char flag[5] = {0x01 , 0x02 , 0x03 , 0x05 , 0xdd} ;
                        if ( p->data.length( ) == 5 && memcmp( flag , p->data.c_str( ) , 5 ) == 0 ) {

                                boStringAppend ap ;
                                ap.add( "call is_exit_data(), return true" ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;

                                return true ;
                        } else
                                return false ;
                } else {
                        boStringAppend ap ;
                        ap.add( "call is_exit_data(), p is null" ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;

                        return false ;
                }

        }

        static u32_t nCount ;
} ;
u32_t thr_task_handler::nCount = 0 ;
typedef boSingletonV2<thr_task_handler , thr_task_data* , BO_ASYNC_SERVICE&> MY_THREAD_TASK_HANDLER ;

void run_recv_thr( ) {
        char flag[5] = {0x01 , 0x02 , 0x03 , 0x05 , 0xdd} ;
        string exit_flag ;
        exit_flag.append( flag , 5 ) ;

        BO_ASYNC_SERVICE io ;
        MY_THREAD_TASK_HANDLER::construct( new thr_task_data( exit_flag ) , io ) ;
        io.run( ) ;
}

void run_send_thr( ) {
        this_thread::sleep( seconds( 3 ) ) ;

        // Save start time.
        start_timestamp = get_now( ) ;

        u32_t n = 0 ;
        while ( n < in_max_num ) {

                char flag[5] = {0x00 , 0x00 , 0x00 , 0x00 , 0xFE} ;
                memcpy( &flag[0] , &n , 4 ) ;

                string data ;
                data.append( flag , 5 ) ;


                //          if ( MY_THREAD_TASK_HANDLER::instance( )->put( data , e_what ) == -1 ) {
                //
                //               in_err_num++ ;
                //          }

                MY_THREAD_TASK_HANDLER::instance( )->to_block( new thr_task_data( data ) ) ;

                n++ ;

                //                    this_thread::sleep( milliseconds( 1 ) ) ;
        }


}

BOOST_AUTO_TEST_CASE( t1 ) {

        BO_THREAD thr1( run_recv_thr ) ;
        //     thr1.detach( ) ;

        BO_THREAD thr2( run_send_thr ) ;
        BO_THREAD thr3( run_send_thr ) ;
        thr2.join( ) ;
        thr3.join( ) ;

        this_thread::sleep( seconds( 2 ) ) ;
        MY_THREAD_TASK_HANDLER::release( ) ;

        {
                boStringAppend ap ;
                ap.add( "put number:" )->add( in_max_num )->add( ",fail:" )->add( in_err_num )->add( ",success:" )->add( in_max_num - in_err_num )
                        ->add( ",top num:" )->add( thr_task_handler::nCount )
                        ->add( ",start time:" )->add( start_timestamp ) ->add( ",end time:" )->add( end_timestamp ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

}

// Test thread common task, two thread send and one thread receive, receive number is right.

class thr_task_handler2 : public boThreadTask {
public:

        thr_task_handler2( thr_task_data* exit_flag ) :
             boThreadTask( exit_flag ) {

        } ;

        virtual ~thr_task_handler2( ) {
                exit_task( ) ;
        } ;

        void top( boThreadTaskData*& data ) override {
                //          boStringAppend ap ;
                //          boString t ;
                //          ap.add( "top data_size:" )->add( data_size ) ;
                //
                //          ::g_debug_out( ap.to_string( ) ) ;

                thr_task_data* p = dynamic_cast < thr_task_data* > ( data ) ;
                if ( p ) {
                        size_t data_size = p->data.length( ) ;

                        int max = data_size / 5 ;
                        for ( int i = 0 ; i < max ; i++ ) {

                                //                                const char* header = p->data.c_str( ) + i * 5 ;
                                //                                boStringAppend ap ;
                                //                                boString t ;
                                //                                ap.add( "top data:" )->add( t.stream_to_string( header , 5 ) ) ->add( ",data_size:5 ,nCount:" )->add( nCount ) ;
                                //                                ::g_debug_out( ap.to_string( ) ) ;

                                // Save last rev time.

                                //                                if ( nCount == ( rev_max_num - 1 ) ) {
                                //                                        end_timestamp = get_now( ) ;
                                //                                }
                                //
                                nCount++ ;

                                if ( nCount % in_max_num == 0 ) {
                                        boStringAppend ap ;
                                        ap.add( "nCount:" )->add( nCount ) ;
                                        ::bo_debug_out( ap.to_string( ) ) ;
                                }
                        }
                } else {
                        boStringAppend ap ;
                        ap.add( "call top(), p is null" ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }

        }

        bool is_exit_data( boThreadTaskData*& data ) override {

                thr_task_data* p = dynamic_cast < thr_task_data* > ( data ) ;
                if ( p ) {
                        char flag[5] = {0x01 , 0x02 , 0x03 , 0x05 , 0xdd} ;
                        if ( p->data.length( ) == 5 && memcmp( flag , p->data.c_str( ) , 5 ) == 0 ) {

                                boStringAppend ap ;
                                ap.add( "call is_exit_data(), return true" ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;

                                return true ;
                        } else
                                return false ;
                } else {
                        boStringAppend ap ;
                        ap.add( "call is_exit_data(), p is null" ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;

                        return false ;
                }

        }

        static u32_t nCount ;
} ;
u32_t thr_task_handler2::nCount = 0 ;
typedef boSingletonV1<thr_task_handler2 , thr_task_data* > MY_THREAD_TASK_HANDLER2 ;

void run_recv_thr2( ) {
        char flag[5] = {0x01 , 0x02 , 0x03 , 0x05 , 0xdd} ;
        string exit_flag ;
        exit_flag.append( flag , 5 ) ;

        MY_THREAD_TASK_HANDLER2::construct( new thr_task_data( exit_flag ) ) ;
}

void run_send_thr2( ) {
        this_thread::sleep( seconds( 3 ) ) ;

        // Save start time.
        start_timestamp = get_now( ) ;

        int loop = 0 ;
        while ( loop < 100 ) {
                u32_t n = 0 ;
                while ( n < in_max_num ) {

                        char flag[5] = {0x00 , 0x00 , 0x00 , 0x00 , 0xFE} ;
                        memcpy( &flag[0] , &n , 4 ) ;

                        string data ;
                        data.append( flag , 5 ) ;


                        //          if ( MY_THREAD_TASK_HANDLER::instance( )->put( data , e_what ) == -1 ) {
                        //
                        //               in_err_num++ ;
                        //          }

                        thr_task_data* exit_p = new thr_task_data( data ) ;
                        boErrorWhat e_what ;
                        MY_THREAD_TASK_HANDLER2::instance( )->put( exit_p , e_what ) ;

                        n++ ;
                }

                //                this_thread::sleep( seconds( 2 ) ) ;

                loop++ ;
        }




}

BOOST_AUTO_TEST_CASE( t2 ) {

        BO_THREAD thr1( run_recv_thr2 ) ;
        thr1.detach( ) ;

        BO_THREAD thr2( run_send_thr2 ) ;
        BO_THREAD thr3( run_send_thr2 ) ;
        thr2.join( ) ;
        thr3.join( ) ;

        this_thread::sleep( seconds( 2 ) ) ;
        MY_THREAD_TASK_HANDLER2::release( ) ;

        {
                boStringAppend ap ;
                ap.add( "put number:" )->add( in_max_num )->add( ",fail:" )->add( in_err_num )->add( ",success:" )->add( in_max_num - in_err_num )
                        ->add( ",top num:" )->add( thr_task_handler2::nCount )
                        ->add( ",start time:" )->add( start_timestamp ) ->add( ",end time:" )->add( end_timestamp ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }
}

BOOST_AUTO_TEST_SUITE_END( )
