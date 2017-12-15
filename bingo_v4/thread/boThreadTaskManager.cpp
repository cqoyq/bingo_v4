/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boThreadTaskManager.cpp
 * Author: root
 * 
 * Created on 2017年12月13日, 下午2:29
 */

#include "boThreadTaskManager.h"

using namespace bingo::thread ;

boThreadTaskManager::boThreadTaskManager( ) : boCollection<boThreadTaskSvrAction>( ) {
}

boThreadTaskManager::~boThreadTaskManager( ) {
}

boThreadTaskSvrAction* boThreadTaskManager::get( const char* key ) {
        // Iter type.
        boost::ptr_vector<boThreadTaskSvrAction>::iterator iter = boost::find_if( this->collection_ , is_equal_key( key ) ) ;
        
        if ( iter != this->collection_.end( ) ) {
                return &( *iter ) ;
        } else
                return nullptr ;
}

