/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boCRC32.cpp
 * Author: root
 * 
 * Created on 2017年9月5日, 上午10:43
 */

#include "boCRC32.h"

using namespace bingo::algorithm ;


#include <boost/crc.hpp>

boCRC32::boCRC32( ) {
}

boCRC32::~boCRC32( ) {
}

u32_t boCRC32::crc32_to_int( string& in_stream ) {
        crc_32_type crc32 ;
        crc32.process_bytes( in_stream.c_str( ) , in_stream.length( ) ) ;

        return crc32.checksum( ) ;
}

void boCRC32::crc32_to_stream( string& in , string& out ) {
        u32_t re = crc32_to_int( in ) ;

        char data[4] = {0x00} ;
        memcpy( data , &re , 4 ) ;

        out.append( data , 4 ) ;
}

