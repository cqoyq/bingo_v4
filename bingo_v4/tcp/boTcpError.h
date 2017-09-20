/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boTcpError.h
 * Author: root
 *
 * Created on 2017年9月5日, 下午1:09
 */

#ifndef BINGO_TCP_ERROR_HEADER_H_
#define BINGO_TCP_ERROR_HEADER_H_

namespace bingo {
        namespace TCP {
                enum {
                        TCP_ERROR_TYPE_SERVER_UNKNOWN = 0x00,
                        TCP_ERROR_TYPE_SERVER_ACTIVE_CLOSE_SOCKET = 0x01,
                        TCP_ERROR_TYPE_SERVER_PASSIVE_CLOSE_SOCKET = 0x02,
                        TCP_ERROR_TYPE_SERVER_CLOSE_SOCKET_BECAUSE_HEARTJUMP = 0x03,
                        TCP_ERROR_TYPE_SERVER_CLOSE_SOCKET_BECAUSE_AUTHENTICATE = 0x04,
                        TCP_ERROR_TYPE_SERVER_MESSAGE_FORMAT_HAS_ERROR = 0x05,
                        TCP_ERROR_TYPE_SERVER_MAX_OF_SOCKET_LIMIT = 0x06,
                        TCP_ERROR_TYPE_SERVER_HEARTJUMP_FORMAT_HAS_ERROR = 0x07,
                        TCP_ERROR_TYPE_SERVER_AUTHENTICATE_FORMAT_HAS_ERROR = 0x08,
                        TCP_ERROR_TYPE_CLIENT_ACTIVE_CLOSE_SOCKET = 0x11,
                        TCP_ERROR_TYPE_CLIENT_PASSIVE_CLOSE_SOCKET = 0x12,
                        TCP_ERROR_TYPE_CLIENT_CLOSE_SOCKET_BECAUSE_HEARTJUMP = 0x13,
                        TCP_ERROR_TYPE_CLIENT_MESSAGE_FORMAT_HAS_ERROR = 0x14,
                };
        }
}

#endif /* BINGO_TCP_ERROR_HEADER_H_ */

