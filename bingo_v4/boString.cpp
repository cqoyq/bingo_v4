/*
 * string.cpp
 *
 *  Created on: 2016-6-29
 *      Author: root
 */

#include "boString.h"
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost ;

boString::boString( ) {

}

boString::~boString( ) {

}

int boString::char_to_int( string chr ) {

        // Change to lower.
        to_lower( chr ) ;

        if ( chr.compare( "1" ) == 0 ) {
                return 1 ;
        } else if ( chr.compare( "2" ) == 0 ) {
                return 2 ;
        } else if ( chr.compare( "3" ) == 0 ) {
                return 3 ;
        } else if ( chr.compare( "4" ) == 0 ) {
                return 4 ;
        } else if ( chr.compare( "5" ) == 0 ) {
                return 5 ;
        } else if ( chr.compare( "6" ) == 0 ) {
                return 6 ;
        } else if ( chr.compare( "7" ) == 0 ) {
                return 7 ;
        } else if ( chr.compare( "8" ) == 0 ) {
                return 8 ;
        } else if ( chr.compare( "9" ) == 0 ) {
                return 9 ;
        } else if ( chr.compare( "a" ) == 0 ) {
                return 10 ;
        } else if ( chr.compare( "b" ) == 0 ) {
                return 11 ;
        } else if ( chr.compare( "c" ) == 0 ) {
                return 12 ;
        } else if ( chr.compare( "d" ) == 0 ) {
                return 13 ;
        } else if ( chr.compare( "e" ) == 0 ) {
                return 14 ;
        } else if ( chr.compare( "f" ) == 0 ) {
                return 15 ;
        } else {
                return 0 ;
        }
}

string boString::stream_to_string( const char* p , size_t p_size ) {

        string str ;

        char tmp[4] = {0x00} ;
        for ( size_t i = 0 ; i < p_size ; ++i ) {
                u8_t ch = ( u8_t ) ( *( p + i ) ) ;
                memset( tmp , 0x00 , 4 ) ;
                sprintf( tmp , "%02x " , ch ) ;

                str.append( &tmp[0] ) ;
        }

        trim( str ) ;

        return str ;
}

string boString::string_to_stream( string& in ) {
        vector<string> v ;
        split( v , in , is_any_of( " " ) , token_compress_on ) ;

        size_t max = v.size( ) ;
        char* data = new char[max] ;
        memset( data , 0x00 , max ) ;

        for ( size_t i = 0 ; i < max ; ++i ) {
                string vt = v[i] ;

                string height_bit = vt.substr( 0 , 1 ) ;
                string low_bit = vt.substr( 1 , 1 ) ;
                int hbit = char_to_int( height_bit ) ;
                int lbit = char_to_int( low_bit ) ;

                int value = lbit + hbit * 16 ;
                char a = 0x00 ;
                memcpy( &a , &value , 1 ) ;

                data[i] = a ;
        }

        string out ;
        out.append( data , max ) ;

        delete[] data ;

        return out ;
}

string boString::short_to_stream( u16_t value ) {

        char data[2] = {0x00 , 0x00} ;

        data[0] = value % 256 ;
        data[1] = value / 256 ;

        string out ;
        out.append( data , 2 ) ;

        return out ;
}

u16_t boString::stream_to_short( const char* p ) {
        u16_t v ;
        memcpy( &v , p , 2 ) ;

        return v ;
}

u32_t boString::ip_to_int( string& s ) {
        vector<string> v ;
        split( v , s , is_any_of( "." ) , token_compress_on ) ;

        if ( v.size( ) != 4 )
                return 0 ;
        else {

                u8_t data[4] = {0x00 , 0x00 , 0x00 , 0x00} ;
                data[0] = lexical_cast<u32_t>( v[0] ) ;
                data[1] = lexical_cast<u32_t>( v[1] ) ;
                data[2] = lexical_cast<u32_t>( v[2] ) ;
                data[3] = lexical_cast<u32_t>( v[3] ) ;

                return data[0] + data[1]*256 + data[2]*256 * 256 + data[3]*256 * 256 * 256 ;
        }
}

string boString::int_to_ip( u32_t n ) {

        u32_t n_part1 = n % 256 ;
        u32_t n_part2 = n / 256 % 256 ;
        u32_t n_part3 = n / 256 / 256 % 256 ;
        u32_t n_part4 = n / 256 / 256 / 256 ;

        char tmp[16] = {0x00} ;
        memset( tmp , 0x00 , 16 ) ;
        sprintf( &tmp[0] , "%d.%d.%d.%d" , n_part1 , n_part2 , n_part3 , n_part4 ) ;

        string out ;
        out.append( &tmp[0] ) ;

        return out ;
}

string boString::convert( const char*& t ) {
        string out ;
        out.append( t ) ;
        return out ;
}

string boString::convert( const char& t ) {
        stringstream oss ;
        string out ;
        u8_t c = ( u8_t ) t ;
        oss << ( u16_t ) c ;
        oss >> out ;
        return out.c_str( ) ;
}

string boString::convert( const u8_t& t ) {
        stringstream oss ;
        string out ;
        oss << ( u16_t ) t ;
        oss >> out ;
        return out.c_str( ) ;
}
