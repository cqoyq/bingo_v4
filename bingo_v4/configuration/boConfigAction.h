/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boConfigAction.h
 * Author: root
 *
 * Created on 2017年9月19日, 下午3:44
 */

#ifndef BINGO_CONFIG_ACTION_H_
#define BINGO_CONFIG_ACTION_H_

#include "bingo_v4/boForeach.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/configuration/boNode.h"
using bingo::configuration::boNode;
using namespace bingo;

namespace bingo {
        namespace configuration {
                class boConfigAction {
                public:
                        boConfigAction(bool is_pretty = false);
                        virtual ~boConfigAction();

                        // Read data from file, fail return false, then check err().
                        virtual bool read(const char* filepath) = 0;

                        // Read data from steam, fail return false, then check err().
                        virtual bool read(stringstream& stream) = 0;

                        // Write to stream.
                        virtual void write(stringstream& stream, boNode* n) = 0;

                        // Write to file.
                        virtual void write(const char* file, boNode* n) = 0;



                        // Get value, fail return false, then check err().
                        virtual bool get_value(const char* pt_path, string& out_value) = 0;

                        // Get node, fail return 0, then check err().
                        virtual boNode* get_node(const char* pt_path) = 0;

                        // Return error.
                        boErrorWhat& err();

                protected:
                        boNode* node_;

                        bool is_pretty_;
                        boErrorWhat e_what_;

                };

        }
}

#endif /* BOCONFIGACTION_H */

