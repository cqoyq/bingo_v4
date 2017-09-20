/*
 * node.h
 *
 *  Created on: 2016-2-19
 *      Author: root
 */

#ifndef BINGO_CONFIG_NODE_H_
#define BINGO_CONFIG_NODE_H_

#include <exception>
#include <string>
using namespace std;


#include "bingo_v4/boCollection.h"
#include "bingo_v4/boForeach.h"

namespace bingo {
        namespace configuration {

                // Node information class.
                class boNode {
                public:
                        // Node collection class
                        class NodeSet : public boCollection<boNode> {
                        public:
                                NodeSet();
                                virtual ~NodeSet();

                                // Get one node by name, fail return 0.
                                boNode* operator[](const char* name);
                        };

                        // Node attribute class
                        struct NodeAttr {
                                string name;
                                string value;
                                NodeAttr(const char* name, const char* value) :
                                     name(name), value(value) {
                                }
                        };

                        // Node attribute collection class
                        class NodeAttrSet : public boCollection<NodeAttr> {
                        public:
                                NodeAttrSet();
                                virtual ~NodeAttrSet();

                                // Get attribute by name, fail return 0.
                                NodeAttr* operator[](const char* attr_name);
                        };

                public:
                        string name;
                        string value;
                        NodeAttrSet attrs;
                        NodeSet child;
                        boNode() {
                        }
                        boNode(const char* name, const char* value = "") :
                             name(name), value(value) {
                        };
                        virtual ~boNode() {
                        }
                        template<typename HANDLER>
                        void write(stringstream& stream) {
                                HANDLER hdr;
                                hdr.write(stream, this);
                        }
                        template<typename HANDLER>
                        void write(const char* file) {
                                HANDLER hdr;
                                hdr.write(file, this);
                        }

                private:

                };

                // Configuration error type.
                enum {
                        CONFIG_ERROR_TYPE_READ_FILE_FAIL = 0x01,
                        CONFIG_ERROR_TYPE_READ_STREAM_FAIL = 0x02,
                        CONFIG_ERROR_TYPE_GET_VALUE_FAIL = 0x03,
                        CONFIG_ERROR_TYPE_GET_ATTRIBUTE_FAIL = 0x04,
                        CONFIG_ERROR_TYPE_GET_NODE_FAIL = 0x05,
                };
        }
}


#endif /* BINGO_CONFIG__NODE_H_ */
