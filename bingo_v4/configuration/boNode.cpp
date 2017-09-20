/*
 * node.cpp
 *
 *  Created on: 2016-10-11
 *      Author: root
 */

#include "boNode.h"

using namespace bingo::configuration ;

// ------------------------------  node_attr_set ----------------------------------- //

boNode::NodeAttrSet::NodeAttrSet( ) {

}

boNode::NodeAttrSet::~NodeAttrSet( ) {

}

boNode::NodeAttr* boNode::NodeAttrSet::operator []( const char* attr_name ) {
        boNode::NodeAttr* p = 0 ;

        foreach_( boNode::NodeAttr&n , collection_ ) {
                if ( n.name.compare( attr_name ) == 0 ) {
                        p = &n ;
                        break ;
                }
        }
        return p ;
}


// ------------------------------  node_set ----------------------------------- //

boNode::NodeSet::NodeSet( ) {

}

boNode::NodeSet::~NodeSet( ) {

}

boNode* boNode::NodeSet::operator []( const char* name ) {

        foreach_( boNode&n , collection_ ) {
                if ( n.name.compare( name ) == 0 )
                        return &n ;
        }
        return 0 ;
}