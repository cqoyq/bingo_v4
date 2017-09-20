/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boUUID.cpp
 * Author: root
 * 
 * Created on 2017年9月5日, 上午10:19
 */

#include "boUUID.h"

using namespace bingo::algorithm ;

boUUID::boUUID( ) {
}

boUUID::~boUUID( ) {
}

string boUUID::make_random_uuid( ) {
        random_generator rgen ; //随机生成器
        uuid u = rgen( ) ;

        return uuid_to_string( u ) ;
}

string boUUID::uuid_to_string( boost::uuids::uuid& in ) {
        std::stringstream ss ;
        ss << in ;
        string out ;
        ss >> out ;

        return out ;
}

void boUUID::string_to_uuid( string& in , boost::uuids::uuid& out ) {
        string_generator sgen ;
        uuid u1 = sgen( in ) ;
        memcpy( out.data , u1.data , 16 ) ;
}

