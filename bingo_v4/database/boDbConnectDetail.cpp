/*
 * dbconnector.cpp
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#include "boDbConnectDetail.h"

using namespace bingo ;
using namespace bingo::database ;

boDbConnectDetail::boDbConnectDetail( ) {
        // TODO Auto-generated constructor stub
        port_ = 0 ;
}

boDbConnectDetail::boDbConnectDetail( const char* ip , const char* db , const char* user , const char* pwd , u32_t port ) :
     ip_( ip ) ,
     dbname_( db ) ,
     user_( user ) ,
     pwd_( pwd ) ,
     port_( port ) {

}

boDbConnectDetail::~boDbConnectDetail( ) {
        // TODO Auto-generated destructor stub
}

const char* boDbConnectDetail::ip( ) {
        return ip_.c_str( ) ;
}

void boDbConnectDetail::ip( const char* s ) {
        ip_ = s ;
}

const char* boDbConnectDetail::user( ) {
        return user_.c_str( ) ;
}

void boDbConnectDetail::user( const char* s ) {
        user_ = s ;
}

const char* boDbConnectDetail::pwd( ) {
        return pwd_.c_str( ) ;
}

void boDbConnectDetail::pwd( const char* s ) {
        pwd_ = s ;
}

const char* boDbConnectDetail::dbname( ) {
        return dbname_.c_str( ) ;
}

void boDbConnectDetail::dbname( const char* s ) {
        dbname_ = s ;
}

u32_t boDbConnectDetail::port( ) {
        return port_ ;
}

void boDbConnectDetail::port( u32_t v ) {
        port_ = v ;
}

void boDbConnectDetail::clone( boDbConnectDetail& new_obj ) {
        new_obj.dbname( this->dbname_.c_str( ) ) ;
        new_obj.ip( this->ip_.c_str( ) ) ;
        new_obj.port( this->port_ ) ;
        new_obj.pwd( this->pwd_.c_str( ) ) ;
        new_obj.user( this->user_.c_str( ) ) ;
}

