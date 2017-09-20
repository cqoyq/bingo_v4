/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boSha1.h
 * Author: root
 *
 * Created on 2017年9月5日, 上午10:35
 */

#ifndef BINGO_ALGORITHM_SHA1_HEADER_H_
#define BINGO_ALGORITHM_SHA1_HEADER_H_

#include "bingo_v4/boType.h"

namespace bingo {
        namespace algorithm {
                class boSha1 {
                public:
                        boSha1();
                        virtual ~boSha1();

                        // Handle sha1, input is stream, output is 20 bytes stream.
                        void sha1_to_stream(string& in, string& out);
                };

        }
}

#endif /* BINGO_ALGORITHM_SHA1_HEADER_H_ */

