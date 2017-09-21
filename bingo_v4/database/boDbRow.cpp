/*
 * dbrow.cpp
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#include "boDbRow.h"

#include "boDbResult.h"

using namespace bingo ;
using namespace bingo::database ;

// ----------------------------------  db_row ------------------------------------  //

boDbRow::boDbRow( void* res ) {
        res_ = res ;
}

boDbRow::~boDbRow( ) {

}

void boDbRow::add_field( boDbField* field ) {
        fields_.push_back( field ) ;
}

boDbField* boDbRow::get_field( int index ) {
        return fields_.at( index ) ;
}

boDbField* boDbRow::get_field( const char* field_name ) {
        boDbResult* p = static_cast < boDbResult* > ( res_ ) ;

        int index = p->get_header_index( field_name ) ;
        if ( index == -1 )
                return 0 ;
        else {
                return fields_.at( index ) ;
        }
}

int boDbRow::size( ) {
        return fields_.size( ) ;
}



