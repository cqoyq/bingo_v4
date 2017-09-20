/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boDesCbc.h
 * Author: root
 *
 * Created on 2017年9月11日, 上午10:47
 */

#ifndef BINGO_ALGORITHM_DESCBC_HEADER_H_
#define BINGO_ALGORITHM_DESCBC_HEADER_H_

#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/boType.h"

namespace bingo {
        namespace algorithm {
                class boDesCbc {
                public:
                        boDesCbc();
                        virtual ~boDesCbc();

                        // Set des-cbc key.
                        void set_key(char* key);
                        void set_iv(char* iv);

                        // Encrypt data by des-cbc.
                        // return -1 fail, then to check error, return 0 success.
                        int encrypt(string& in, string& out);
                        int encrypt(const char* in, size_t in_size, string& out);

                        // Decrypt data by des-cbc.
                        // return -1 fail, then to check error, return 0 success.
                        int decrypt(string& in, string& out);
                        int decrypt(const char* in, size_t in_size, string& out);
                        enum {
                                MAX_CACHE_BUFFER_SIZE = 1024,
                                MAX_KEY_SIZE = 8,
                        };

                        // Retrival error information.
                        boErrorWhat& err() {
                                return err_;
                        }

                protected:
                        boErrorWhat err_;

                        u8_t* key_;
                        size_t key_size_;

                        u8_t* iv_;
                        size_t iv_size_;
                };
        }
}

#endif /* BINGO_ALGORITHM_DESCBC_HEADER_H_ */

