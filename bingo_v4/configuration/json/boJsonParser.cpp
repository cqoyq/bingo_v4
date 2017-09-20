/*
 * parse_handler.cpp
 *
 *  Created on: 2016-2-20
 *      Author: root
 */

#include "boJsonParser.h"

using namespace bingo::configuration::json ;

boJsonParser::boJsonParser( bool is_pretty ) : boConfigAction( is_pretty ) {
}

boJsonParser::~boJsonParser( ) {
}

bool boJsonParser::read( const char* filepath ) {
        try {
                pt::read_json( filepath , pt_ ) ;
                return true ;
        } catch ( boost::exception& e ) {
                e_what_.err_no( CONFIG_ERROR_TYPE_READ_FILE_FAIL ) ;
                e_what_.err_message( current_exception_cast<std::exception>( )->what( ) ) ;

                return false ;
        }
}

bool boJsonParser::read( stringstream& stream ) {
        try {
                pt::read_json( stream , pt_ ) ;
                return true ;
        } catch ( boost::exception& e ) {
                e_what_.err_no( CONFIG_ERROR_TYPE_READ_STREAM_FAIL ) ;
                e_what_.err_message( current_exception_cast<std::exception>( )->what( ) ) ;
                return false ;
        }
}

void boJsonParser::write( stringstream& stream , boNode* n ) {

        pt::ptree wtree ;
        make_ptree( n , wtree ) ;

        pt::write_json( stream , wtree , is_pretty_ ) ;
}

void boJsonParser::write( const char* file , boNode* n ) {

        pt::ptree wtree ;
        make_ptree( n , wtree ) ;

        pt::write_json( file , wtree , std::locale( ) , is_pretty_ ) ;
}

void boJsonParser::parse_ptree( boNode* n , pt::ptree& tree ) {
        n->value.append( tree.get_value<string>( ).c_str( ) ) ;

        foreach_( pt::ptree::value_type &v1 , tree ) {
                if ( v1.first == "<xmlattr>" ) {

                } else {
                        // First node do it.
                        string name( v1.first ) ;
                        boNode* child = new boNode( name.c_str( ) ) ;

                        parse_ptree( child , v1.second ) ;

                        n->child.push_back( child ) ;
                }

        }
}

void boJsonParser::make_ptree( boNode* n , pt::ptree& tree ) {

        pt::ptree& addnode = tree.add( n->name , n->value ) ;

        if ( n->child.size( ) > 0 ) {
                // Have child, then node value is ''.
                addnode.put_value( "" ) ;

                foreach_( boNode& m , n->child.collection( ) ) {
                        make_ptree( &m , addnode ) ;
                }
        }
}

bool boJsonParser::get_value( const char* pt_path , string& out_value ) {
        try {
                out_value = pt_.get<string>( pt_path ) ;
                return true ;
        } catch ( boost::exception& e ) {
                e_what_.err_no( CONFIG_ERROR_TYPE_GET_VALUE_FAIL ) ;
                e_what_.err_message( current_exception_cast<std::exception>( )->what( ) ) ;

                return false ;
        }
}

boNode* boJsonParser::get_node( const char* pt_path ) {

        if ( node_ != 0 ) {
                delete node_ ;
                node_ = 0 ;
        }

        try {
                pt::ptree pt = pt_.get_child( pt_path ) ;

                node_ = new boNode( ) ;
                parse_ptree( node_ , pt ) ;

                return node_ ;
        } catch ( boost::exception& e ) {
                e_what_.err_no( CONFIG_ERROR_TYPE_GET_NODE_FAIL ) ;
                e_what_.err_message( current_exception_cast<std::exception>( )->what( ) ) ;

                return 0 ;
        }
}
