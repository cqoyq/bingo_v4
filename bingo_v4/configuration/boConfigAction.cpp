/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "boConfigAction.h"

using namespace bingo::configuration ;

boConfigAction::boConfigAction( bool is_pretty ) : is_pretty_( is_pretty ) {
        node_ = 0 ;
}

boConfigAction::~boConfigAction( ) {
        if ( node_ != 0 ) delete node_ ;
}

boErrorWhat& boConfigAction::err( ) {
        return e_what_ ;
}

