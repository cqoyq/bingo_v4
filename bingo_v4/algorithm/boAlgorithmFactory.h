/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boAlgorithmFactory.h
 * Author: root
 *
 * Created on 2017年9月5日, 上午10:17
 */

#ifndef BINGO_ALGORITHM_ALGORITHMFACTORY_HEADER_H_
#define BINGO_ALGORITHM_ALGORITHMFACTORY_HEADER_H_

#include "boUUID.h"
#include "boSha1.h"
#include "boCRC32.h"
#include "boDesCbc.h"

namespace bingo {
        namespace algorithm {
                class boAlgorithmFactory {
                public:
                        boAlgorithmFactory();
                        virtual ~boAlgorithmFactory();

                        static boUUID* uuid;
                        static boSha1* sha1;
                        static boCRC32* crc32;

                        static boDesCbc* make_des_cbc();
                };
        }
}



#endif /* BINGO_ALGORITHM_ALGORITHMFACTORY_HEADER_H_ */

