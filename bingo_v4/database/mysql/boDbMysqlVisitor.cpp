/*
 * mysqlvisitor.cpp
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#include <mysql.h>
#include "boDbMysqlVisitor.h"

using namespace bingo::database ;
using namespace bingo::database::mysql ;

boDbMysqlVisitor::boDbMysqlVisitor( boDbConnectDetail& conn ) : boDbVisitor( ) {
        // TODO Auto-generated constructor stub
        conn.clone( this->conn_ ) ;
}

boDbMysqlVisitor::~boDbMysqlVisitor( ) {
        // TODO Auto-generated destructor stub
}

void boDbMysqlVisitor::free_mysql_database( ) {
        mysql_library_end( ) ;
}

bool boDbMysqlVisitor::query_result( const char* sql , boDbResult*& result ) {
        // Sql string valid.
        MYSQL *conn = mysql_init( NULL ) ;

        my_bool reconnect = 0 ;
        mysql_options( conn , MYSQL_SECURE_AUTH , &reconnect ) ;

        // Connect mysql's server.
        if ( !mysql_real_connect( conn , conn_.ip( ) , conn_.user( ) , conn_.pwd( ) , conn_.dbname( ) , conn_.port( ) , NULL , 0 ) ) {

                e_what_.err_no( DATABASE_ERROR_TYPE_CONNECT_FAIL ) ;
                e_what_.err_message( "database connect fail!" ) ;

                // free Mysql.
                mysql_close( conn ) ;

                mysql_thread_end( ) ;

                return false ;
        }

        mysql_set_character_set( conn , "utf8" ) ;

        // Execute sql statement.
        mysql_query( conn , sql ) ;
        const char* er = mysql_error( conn ) ;
        if ( ( *er ) != 0x00 ) {

                string err_info = "database query fail!" ;
                err_info.append( " error:" ) ;
                err_info.append( er ) ;
                e_what_.err_no( DATABASE_ERROR_TYPE_QUERY_FAIL ) ;
                e_what_.err_message( err_info.c_str( ) ) ;

                mysql_close( conn ) ;

                mysql_thread_end( ) ;

                return false ;
        }


        // Starting to handle return of result.
        MYSQL_RES *res ;
        MYSQL_ROW row ;

        // Get MYSQL_RES.
        res = mysql_store_result( conn ) ;

        // Get row number and column number in MYSQL_RES.
        result->row_amount = mysql_num_rows( res ) ;
        result->column_amount = mysql_num_fields( res ) ;


        // Get column data in MYSQL_RES.
        MYSQL_FIELD *columns ;
        columns = mysql_fetch_fields( res ) ;
        for ( int i = 0 ; i < result->column_amount ; i++ ) {
                //	   printf("Field %u is %s\n", i, fields[i].name);
                //		result->fields_[i] = columns[i].name;
                result->add_header( new boDbFieldHeader( columns[i].name ) ) ;
        }

        // Get row data in MYSQL_RES.
        while ( ( row = mysql_fetch_row( res ) ) != NULL ) {
                //		result->result[index] = row;

                u64_t *FieldLength = mysql_fetch_lengths( res ) ;
                //		for(int i = 0;i<column_num;i++ )
                //		{
                //			int nDataLen = (int)FieldLength[i];
                //			printf("index=%d column index:%d ,length:%d\n", index, i, nDataLen);
                //		}

                boDbRow* d_row = new boDbRow( result ) ;

                // Save field data.
                for ( int i = 0 ; i < result->column_amount ; i++ ) {
                        // Get field length.
                        u64_t field_size = ( u64_t ) FieldLength[i] ;

                        // Get field value.
                        char* field_value = row[i] ;

                        d_row->add_field( new boDbField( field_value , field_size ) ) ;
                }

                // Save row data.
                result->add_row( d_row ) ;
        }

        mysql_free_result( res ) ;


        mysql_close( conn ) ;

        mysql_thread_end( ) ;

        return true ;
}

bool boDbMysqlVisitor::query_result( std::vector<string>& sqls , boost::ptr_vector<boDbResult>& results ) {
        // Sql string valid.
        MYSQL *conn = mysql_init( NULL ) ;

        my_bool reconnect = 0 ;
        mysql_options( conn , MYSQL_SECURE_AUTH , &reconnect ) ;

        // Connect mysql's server.
        if ( !mysql_real_connect( conn , conn_.ip( ) , conn_.user( ) , conn_.pwd( ) , conn_.dbname( ) , conn_.port( ) , NULL , 0 ) ) {

                e_what_.err_no( DATABASE_ERROR_TYPE_CONNECT_FAIL ) ;
                e_what_.err_message( "database connect fail!" ) ;

                // free Mysql.
                mysql_close( conn ) ;

                mysql_thread_end( ) ;

                return false ;
        }

        mysql_set_character_set( conn , "utf8" ) ;

        for ( size_t n = 0 ; n < sqls.size( ) ; n++ ) {

                if ( strcmp( sqls[n].c_str( ) , "" ) == 0 ) continue ;

                // Execute sql statement.
                mysql_query( conn , sqls[n].c_str( ) ) ;
                const char* er = mysql_error( conn ) ;
                if ( ( *er ) != 0x00 ) {

                        string err_info = "database query fail!" ;
                        err_info.append( " error:" ) ;
                        err_info.append( er ) ;

                        e_what_.err_no( DATABASE_ERROR_TYPE_QUERY_FAIL ) ;
                        e_what_.err_message( err_info.c_str( ) ) ;

                        mysql_close( conn ) ;

                        mysql_thread_end( ) ;

                        return false ;
                }


                // Starting to handle return of result.
                MYSQL_RES *res ;
                MYSQL_ROW row ;

                boDbResult* result = new boDbResult( ) ;

                // Add result to vector.
                results.push_back( result ) ;


                // Get MYSQL_RES.
                res = mysql_store_result( conn ) ;

                if ( res ) {

                        // Get row number and column number in MYSQL_RES.
                        result->row_amount = mysql_num_rows( res ) ;
                        result->column_amount = mysql_num_fields( res ) ;


                        // Get column data in MYSQL_RES.
                        MYSQL_FIELD *columns ;
                        columns = mysql_fetch_fields( res ) ;
                        for ( int i = 0 ; i < result->column_amount ; i++ ) {
                                //	   printf("Field %u is %s\n", i, fields[i].name);
                                //		result->fields_[i] = columns[i].name;
                                result->add_header( new boDbFieldHeader( columns[i].name ) ) ;
                        }

                        // Get row data in MYSQL_RES.
                        while ( ( row = mysql_fetch_row( res ) ) != NULL ) {

                                u64_t *FieldLength = mysql_fetch_lengths( res ) ;

                                boDbRow* d_row = new boDbRow( result ) ;

                                // Save field data.
                                for ( int i = 0 ; i < result->column_amount ; i++ ) {
                                        // Get field length.
                                        u64_t field_size = ( u64_t ) FieldLength[i] ;

                                        // Get field value.
                                        char* field_value = row[i] ;

                                        d_row->add_field( new boDbField( field_value , field_size ) ) ;
                                }

                                // Save row data.
                                result->add_row( d_row ) ;
                        }

                }

                mysql_free_result( res ) ;

        }

        mysql_close( conn ) ;

        mysql_thread_end( ) ;

        return true ;
}

bool boDbMysqlVisitor::query_result( std::vector<string>& sqls ) {
        // Sql string valid.
        MYSQL *conn = mysql_init( NULL ) ;

        my_bool reconnect = 0 ;
        mysql_options( conn , MYSQL_SECURE_AUTH , &reconnect ) ;

        if ( !mysql_real_connect( conn , conn_.ip( ) , conn_.user( ) , conn_.pwd( ) , conn_.dbname( ) , conn_.port( ) , NULL , 0 ) ) {

                e_what_.err_no( DATABASE_ERROR_TYPE_CONNECT_FAIL ) ;
                e_what_.err_message( "database connect fail!" ) ;

                // free Mysql.
                mysql_close( conn ) ;

                mysql_thread_end( ) ;

                return false ;
        }

        mysql_set_character_set( conn , "utf8" ) ;

        // Start trasaction.
        mysql_query( conn , "start transaction;" ) ;

        for ( size_t i = 0 ; i < sqls.size( ) ; i++ ) {

                if ( sqls[i] != "" ) {

                        mysql_query( conn , sqls[i].c_str( ) ) ;
                        const char* er = mysql_error( conn ) ;
                        if ( ( *er ) != 0x00 ) {

                                string err_info = "database query fail!" ;
                                err_info.append( " error:" ) ;
                                err_info.append( er ) ;

                                e_what_.err_no( DATABASE_ERROR_TYPE_QUERY_FAIL ) ;
                                e_what_.err_message( err_info.c_str( ) ) ;

                                // Roll back transaction.
                                mysql_query( conn , "rollback;" ) ;

                                mysql_close( conn ) ;

                                mysql_thread_end( ) ;

                                return false ;

                        }
                }
        }

        // Commit transaction.
        mysql_query( conn , "commit;" ) ;

        mysql_close( conn ) ;

        mysql_thread_end( ) ;

        return true ;
}

