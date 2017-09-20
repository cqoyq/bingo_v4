/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boCRC32.h
 * Author: root
 *
 * Created on 2017年9月5日, 上午10:43
 */

#ifndef BINGO_ALGORITHM_CRC32_HEADER_H_
#define BINGO_ALGORITHM_CRC32_HEADER_H_

#include "bingo_v4/boType.h"

namespace bingo {
        namespace algorithm {
                class boCRC32 {
                public:
                        boCRC32();
                        virtual ~boCRC32();

                        // Handle crc32, input is stream, output is 4 bytes stream.
                        void crc32_to_stream(string& in, string& out);
                        u32_t crc32_to_int(string& in_stream);
                };
        }
}

#endif /* BINGO_ALGORITHM_CRC32_HEADER_H_ */

