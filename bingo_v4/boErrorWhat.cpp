/*
 * error_what.cpp
 *
 *  Created on: 2016-9-28
 *      Author: root
 */

#include "boErrorWhat.h"

using namespace bingo ;

boErrorWhat::boErrorWhat( ) {
        err_no_ = 0 ;
}

boErrorWhat::~boErrorWhat( ) {

}

string& boErrorWhat::err_message( ) {
        return err_ ;
}

void boErrorWhat::err_message( const char* err ) {
        err_ = err ;
}

int boErrorWhat::err_no( ) {
        return err_no_ ;
}

void boErrorWhat::err_no( int n ) {
        err_no_ = n ;
}

void boErrorWhat::clone( boErrorWhat& obj ) {
        obj.err_no( err_no_ ) ;
        obj.err_message( err_.c_str( ) ) ;
}


