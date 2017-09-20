/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boSha1.cpp
 * Author: root
 * 
 * Created on 2017年9月5日, 上午10:35
 */

#include "boSha1.h"

using namespace bingo::algorithm ;

#include <boost/uuid/sha1.hpp>
using namespace boost::uuids::detail ;

boSha1::boSha1( ) {
}

boSha1::~boSha1( ) {
}

void boSha1::sha1_to_stream( string& in , string& out ) {
        sha1 sha ;
        sha.process_bytes( in.c_str( ) , in.length( ) ) ;

        u32_t digest[5] ;
        sha.get_digest( digest ) ;

        char d[20] = {0x00} ;
        for ( int i = 0 ; i < 5 ; i++ ) {
                u32_t elem = digest[i] ;
                memcpy( d + i * 4 , &elem , 4 ) ;
        }

        out.append( d , 20 ) ;
}

