/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boActionFactory.cpp
 * Author: root
 * 
 * Created on 2017年9月19日, 下午3:21
 */

#include "boActionFactory.h"

using namespace bingo ;

#include "bingo_v4/database/mysql/boDbMysqlVisitor.h"
using namespace bingo::database::mysql ;

#include "bingo_v4/log/handler/boLogToConsole.h"
#include "bingo_v4/log/handler/boLogToFile.h"

#include "bingo_v4/configuration/xml/boXmlParser.h"
#include "bingo_v4/configuration/json/boJsonParser.h"
using namespace bingo::configuration::xml ;
using namespace bingo::configuration::json ;

boActionFactory::boActionFactory( ) {
}

boActionFactory::~boActionFactory( ) {
}

boDbVisitor* boActionFactory::make_mysql_visitor( boDbConnectDetail& conn_detail ) {

        // Make mysql visitor.
        return new boDbMysqlVisitor( conn_detail ) ;
}

boLogHandler* boActionFactory::make_file_logger( string& path ) {
        boLogToFile* p = new boLogToFile( ) ;
        p->set_path( path ) ;

        return p ;
}

boLogHandler* boActionFactory::make_console_logger( ) {
        boLogToDebug* p = new boLogToDebug( ) ;

        return p ;
}

boConfigAction* boActionFactory::make_xml_parser( bool is_pretty ) {
        boXmlParser* p = new boXmlParser( is_pretty ) ; 
        return p ;
}

boConfigAction* boActionFactory::make_json_parser( bool is_pretty ) {
        boJsonParser* p = new boJsonParser( is_pretty ) ;
        return p ; 
}
