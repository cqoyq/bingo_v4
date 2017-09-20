/*
 * dbresult.cpp
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#include <boost/algorithm/string.hpp>
#include "bingo_v4/boForeach.h"

#include "boDbResult.h"

using namespace bingo::database ;

boDbResult::boDbResult( ) {
        // TODO Auto-generated constructor stub
        row_amount = 0 ;
        column_amount = 0 ;
}

boDbResult::~boDbResult( ) {
        // TODO Auto-generated destructor stub
}

void boDbResult::add_header( boDbFieldHeader* header ) {
        header_.push_back( header ) ;
}

void boDbResult::add_row( boDbRow* row ) {
        rows_.push_back( row ) ;
}

boDbRow* boDbResult::get_row( int index ) {
        return rows_[index] ;
}

boDbFieldHeader* boDbResult::get_header( int index ) {
        return header_[index] ;
}

int boDbResult::get_header_index( const char* header_name ) {
        int result = -1 , index = 0 ;

        foreach_( boDbFieldHeader& n , header_.collection( ) ) {

                // @s_name1 and @s_name2 change to lower both.
                string s_name1 = boost::to_lower_copy( n.name ) ;
                string s_name2 = header_name ;
                boost::to_lower( s_name2 ) ;

                if ( s_name1.compare( s_name2.c_str( ) ) == 0 ) {
                        result = index ;
                        break ;
                }
                index++ ;
        }

        return result ;
}

