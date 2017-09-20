/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boDesCbc.cpp
 * Author: root
 * 
 * Created on 2017年9月11日, 上午10:47
 */

#include "boDesCbc.h"

using namespace bingo::algorithm ;

#include <openssl/evp.h>

boDesCbc::boDesCbc( ) {
        key_ = 0 ;
        key_size_ = MAX_KEY_SIZE ;

        key_ = new u8_t[MAX_KEY_SIZE] ;
        memset( key_ , 0x00 , MAX_KEY_SIZE ) ;

        iv_ = 0 ;
        iv_size_ = MAX_KEY_SIZE ;

        iv_ = new u8_t[MAX_KEY_SIZE] ;
        memset( iv_ , 0x00 , MAX_KEY_SIZE ) ;
}

boDesCbc::~boDesCbc( ) {
        if ( key_ != 0 ) delete[] key_ ;
        if ( iv_ != 0 ) delete[] iv_ ;
}

void boDesCbc::set_iv( char* iv ) {
        memcpy( iv_ , iv , MAX_KEY_SIZE ) ;
}

void boDesCbc::set_key( char* key ) {
        memcpy( key_ , key , MAX_KEY_SIZE ) ;
}

int boDesCbc::encrypt( string& in , string& out_stream ) {
        u8_t out1[MAX_CACHE_BUFFER_SIZE] ; //输出密文缓冲区

        int out1_size ;
        int out_size_temp ;
        int out_total_size ; //密文长度
        memset( out1 , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;


        // 加密
        {
                EVP_CIPHER_CTX* ctx = new EVP_CIPHER_CTX( ) ; //evp算法上下文
                EVP_CIPHER_CTX_init( ctx ) ;

                int result = EVP_EncryptInit_ex( ctx , EVP_des_cbc( ) , 0 , key_ , iv_ ) ;

                if ( result ) {

                } else {

                        err_.err_message( "encrypt data init fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }


                //			string data("super,1234");						// user is admin.
                size_t data_size = strlen( in.c_str( ) ) ;

                u8_t* s_data = new u8_t[data_size] ;
                for ( size_t i = 0 ; i < data_size ; ++i ) {

                        const char* p1 = in.c_str( ) ;

                        s_data[i] = ( u8_t ) ( *( p1 + i ) ) ;
                }

                result = EVP_EncryptUpdate( ctx , out1 , &out1_size , s_data , data_size ) ;

                delete[] s_data ;

                if ( result ) {

                } else {

                        err_.err_message( "encrypt data update fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                result = EVP_EncryptFinal_ex( ctx , out1 + out1_size , &out_size_temp ) ;

                if ( result ) {

                        out_total_size = out1_size + out_size_temp ;

                } else {

                        err_.err_message( "encrypt data final fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                EVP_CIPHER_CTX_cleanup( ctx ) ;

                delete ctx ;
        }

        char out2[MAX_CACHE_BUFFER_SIZE] ;
        memset( out2 , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;

        for ( int i = 0 ; i < out_total_size ; ++i ) {

                *( out2 + i ) = out1[i] ;
        }

        out_stream.append( out2 , out_total_size ) ;

        return 0 ;
}

int boDesCbc::encrypt( const char* in , size_t in_size , string& out_stream ) {
        u8_t out1[MAX_CACHE_BUFFER_SIZE] ; //输出密文缓冲区

        int out1_size ;
        int out_size_temp ;
        int out_total_size ; //密文长度
        memset( out1 , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;


        // 加密
        {
                EVP_CIPHER_CTX* ctx = new EVP_CIPHER_CTX( ) ; //evp算法上下文
                EVP_CIPHER_CTX_init( ctx ) ;

                int result = EVP_EncryptInit_ex( ctx , EVP_des_cbc( ) , 0 , key_ , iv_ ) ;

                if ( result ) {

                } else {

                        err_.err_message( "encrypt data init fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }


                //			string data("super,1234");						// user is admin.
                size_t data_size = in_size ;

                u8_t* s_data = new u8_t[data_size] ;
                for ( size_t i = 0 ; i < data_size ; ++i ) {

                        const char* p1 = in ;

                        s_data[i] = ( u8_t ) ( *( p1 + i ) ) ;
                }

                result = EVP_EncryptUpdate( ctx , out1 , &out1_size , s_data , data_size ) ;

                delete[] s_data ;

                if ( result ) {

                } else {

                        err_.err_message( "encrypt data update fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                result = EVP_EncryptFinal_ex( ctx , out1 + out1_size , &out_size_temp ) ;

                if ( result ) {

                        out_total_size = out1_size + out_size_temp ;

                } else {

                        err_.err_message( "encrypt data final fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                EVP_CIPHER_CTX_cleanup( ctx ) ;

                delete ctx ;
        }

        char out2[MAX_CACHE_BUFFER_SIZE] ;
        memset( out2 , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;

        for ( int i = 0 ; i < out_total_size ; ++i ) {

                *( out2 + i ) = out1[i] ;
        }

        out_stream.append( out2 , out_total_size ) ;

        return 0 ;
}

int boDesCbc::decrypt( string& in , string& out_stream ) {
        const char* in_p = in.c_str( ) ;
        size_t in_size = in.length( ) ;
        u8_t* s_input = new u8_t[in_size] ;
        for ( size_t i = 0 ; i < in_size ; ++i ) {
                s_input[i] = ( u8_t ) ( *( in_p + i ) ) ;
        }

        u8_t de[MAX_CACHE_BUFFER_SIZE] ; //解码缓冲区 
        int de_size ;
        int de_size_temp ;
        int de_total_size ; //解码总长度
        memset( de , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;

        // 解密
        {
                EVP_CIPHER_CTX* ctx = new EVP_CIPHER_CTX( ) ; //evp算法上下文
                EVP_CIPHER_CTX_init( ctx ) ;

                int result = EVP_DecryptInit_ex( ctx , EVP_des_cbc( ) , 0 , key_ , iv_ ) ;

                if ( result ) {
                } else {
                        err_.err_message( "decrypt data init fail." ) ;
                        EVP_CIPHER_CTX_cleanup( ctx ) ;
                        return -1 ;
                }


                result = EVP_DecryptUpdate( ctx , de , &de_size , s_input , in_size ) ;

                delete[] s_input ;

                if ( result ) {
                } else {

                        err_.err_message( "decrypt data update fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                result = EVP_DecryptFinal_ex( ctx , de + de_size , &de_size_temp ) ;

                if ( result ) {

                        // Get total size.
                        de_total_size = de_size + de_size_temp ;

                } else {

                        err_.err_message( "decrypt data final fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                EVP_CIPHER_CTX_cleanup( ctx ) ;

                delete ctx ;

        }

        char* s_de = new char[de_total_size + 1] ;
        memset( s_de , 0x00 , de_total_size + 1 ) ;

        for ( int i = 0 ; i < de_total_size ; ++i ) {
                s_de[i] = de[i] ;
        }

        out_stream.append( s_de ) ;

        delete[] s_de ;

        return 0 ;
}

int boDesCbc::decrypt( const char* in , size_t in_size , string& out_stream ) {
        u8_t* s_input = new u8_t[in_size] ;
        for ( size_t i = 0 ; i < in_size ; ++i ) {
                s_input[i] = ( u8_t ) ( *( in + i ) ) ;
        }

        u8_t de[MAX_CACHE_BUFFER_SIZE] ; //解码缓冲区 
        int de_size ;
        int de_size_temp ;
        int de_total_size ; //解码总长度
        memset( de , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;

        // 解密
        {
                EVP_CIPHER_CTX* ctx = new EVP_CIPHER_CTX( ) ; //evp算法上下文
                EVP_CIPHER_CTX_init( ctx ) ;

                int result = EVP_DecryptInit_ex( ctx , EVP_des_cbc( ) , 0 , key_ , iv_ ) ;

                if ( result ) {
                } else {
                        err_.err_message( "decrypt data init fail." ) ;
                        EVP_CIPHER_CTX_cleanup( ctx ) ;
                        return -1 ;
                }


                result = EVP_DecryptUpdate( ctx , de , &de_size , s_input , in_size ) ;

                delete[] s_input ;

                if ( result ) {
                } else {

                        err_.err_message( "decrypt data update fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                result = EVP_DecryptFinal_ex( ctx , de + de_size , &de_size_temp ) ;

                if ( result ) {

                        // Get total size.
                        de_total_size = de_size + de_size_temp ;

                } else {

                        err_.err_message( "decrypt data final fail." ) ;

                        EVP_CIPHER_CTX_cleanup( ctx ) ;

                        return -1 ;
                }

                EVP_CIPHER_CTX_cleanup( ctx ) ;

                delete ctx ;

        }

        char out[MAX_CACHE_BUFFER_SIZE] ;
        memset( out , 0x00 , MAX_CACHE_BUFFER_SIZE ) ;

        for ( int i = 0 ; i < de_total_size ; ++i ) {
                out[i] = de[i] ;
        }

        out_stream.append( out , de_total_size ) ;

        return 0 ;
}

