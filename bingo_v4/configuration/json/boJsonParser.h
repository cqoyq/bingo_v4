/*
 * parse_handler.h
 *
 *  Created on: 2016-2-19
 *      Author: root
 */

#ifndef BINGO_CONFIG_JSON_PARSER_HEADER_H_
#define BINGO_CONFIG_JSON_PARSER_HEADER_H_

#include "bingo_v4/configuration/boConfigAction.h"

#include <boost/exception/all.hpp>
#include <boost/current_function.hpp>

#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;
using namespace boost;

namespace bingo {
        namespace configuration {
                namespace json {

                        // Json parse without attribute
                        class boJsonParser : public boConfigAction {
                        public:
                                boJsonParser(bool is_pretty = false);
                                virtual ~boJsonParser();

                                // Read data from file, fail return false, then check err().
                                bool read(const char* filepath)override;

                                // Read data from steam, fail return false, then check err().
                                bool read(stringstream& stream)override;

                                // Get value, fail return false, then check err().
                                bool get_value(const char* pt_path, string& out_value) override;

                                // Get node, fail return 0, then check err().
                                boNode* get_node(const char* pt_path)override;

                                // Write to stream.
                                void write(stringstream& stream, boNode* n)override;

                                // Write to file.
                                void write(const char* file, boNode* n)override;

                        protected:
                                void parse_ptree(boNode* n, pt::ptree& tree);

                                void make_ptree(boNode* n, pt::ptree& tree);

                        protected:
                                pt::ptree pt_;
                        };

                }
        }
}


#endif /* BINGO_CONFIG_JSON_PARSER_HEADER_H_ */
