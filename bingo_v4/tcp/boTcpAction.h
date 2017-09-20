/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boTcpAction.h
 * Author: root
 *
 * Created on 2017年9月13日, 下午4:20
 */

#ifndef BINGO_TCP_ACTION_HEADER_H_
#define BINGO_TCP_ACTION_HEADER_H_

#include <vector>

#include "bingo_v4/boType.h"

namespace bingo {
        namespace TCP {
                // Handle heartjump message.
                class boTcpHeartJumpAction {
                public:
                        virtual ~boTcpHeartJumpAction() {
                        }
                        virtual bool message_heartjump_action(string& message) {
                                return false;
                        }
                };

                // Handle authenticate message.
                class boTcpAuthenticateAction {
                public:
                        virtual ~boTcpAuthenticateAction() {
                        }
                        virtual bool message_authenticate_action(string& message) {
                                return false;
                        }
                };

                // Handle message-block split.
                class boTcpMessageSplitAction {
                public:
                        virtual ~boTcpMessageSplitAction() {
                        }
                        virtual void message_split_action(const char* in, size_t in_size, std::vector<string>& message_set) {
                        }
                };

                // Handle message format.
                class boTcpMessageFormatAction {
                public:
                        virtual ~boTcpMessageFormatAction() {
                        }
                        virtual bool message_format_action(string& message) {
                                return true;
                        }
                };
        }
}

#endif /* BINGO_TCP_ACTION_HEADER_H_ */

