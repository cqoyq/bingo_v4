/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boAlgorithmFactory.cpp
 * Author: root
 * 
 * Created on 2017年9月5日, 上午10:17
 */

#include "boAlgorithmFactory.h"

using namespace bingo::algorithm ;

boUUID* boAlgorithmFactory::uuid = new boUUID ;
boSha1* boAlgorithmFactory::sha1 = new boSha1 ;
boCRC32* boAlgorithmFactory::crc32 = new boCRC32 ;

boAlgorithmFactory::boAlgorithmFactory( ) {
}

boAlgorithmFactory::~boAlgorithmFactory( ) {
}

boDesCbc* boAlgorithmFactory::make_des_cbc( ) {
        return new boDesCbc( ) ;
}
