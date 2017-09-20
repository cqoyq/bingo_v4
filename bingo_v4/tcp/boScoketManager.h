/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boScoketManager.h
 * Author: root
 *
 * Created on 2017年9月5日, 上午10:51
 */

#ifndef BINGO_TCP_SOCKETMANAGER_HEADER_H_
#define BINGO_TCP_SOCKETMANAGER_HEADER_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boCollection.h"
#include "bingo_v4/boErrorWhat.h"
#include "boTcpError.h"

namespace bingo {
        namespace TCP {
                template<class TCP_HANDLER>
                class boScoketManager {
                public:
                        struct SocketDetail {
                                string uuid;
                                void* hdr;
                                SocketDetail() {
                                        hdr = 0;
                                }
                        };
                        class SocketDetailSet : public boCollection<SocketDetail> {
                        public:
                                // Algorithm that find first item with same uuid.
                                struct find_first_item_same_uuid {
                                        find_first_item_same_uuid(string& uuid) : uuid_(uuid) {
                                        }
                                        bool operator()(SocketDetail& n) {
                                                return (n.uuid.compare(uuid_.c_str()) == 0) ? true : false;
                                        }
                                private:
                                        string uuid_;
                                };
                                // Algorithm that find all item with heartjump timeout.
                                template<class HANDLER>
                                struct find_all_item_with_heartjump_timeout {
                                        bool operator()(SocketDetail& n) {

                                                HANDLER* p = static_cast<HANDLER*> (n.hdr);
                                                if (p) {
                                                        if (p->check_heartjump_timeout()) {
                                                                return true;
                                                        }
                                                }
                                                return false;
                                        }
                                };
                                // Algorithm that find all item with authenticate isn't pass.
                                template<class HANDLER>
                                struct find_all_item_with_authenticate_no_pass {
                                        bool operator()(SocketDetail& n) {

                                                HANDLER* p = static_cast<HANDLER*> (n.hdr);
                                                if (p) {
                                                        if (!p->check_authentication_pass()) {
                                                                return true;
                                                        }
                                                }
                                                return false;
                                        }
                                };

                        public:
                                SocketDetailSet() : boCollection<SocketDetail>() {
                                };
                                virtual ~SocketDetailSet() {
                                };

                                // Handle function.
                                void erase(string& uuid) {
                                        typedef typename boost::ptr_vector<SocketDetail>::iterator ITERATOR_TYPE;
                                        // Find the first element which member handler_pointer is same with p.
                                        ITERATOR_TYPE iter = boost::find_if(this->collection(), SocketDetailSet::find_first_item_same_uuid(uuid));

                                        if (iter != this->collection().end()) {
                                                this->collection().erase(iter);
                                        }
                                }
                                void* get_hdr(string& uuid) {
                                        typedef typename boost::ptr_vector<SocketDetail>::iterator ITERATOR_TYPE;
                                        // Find the first element which member handler_pointer is same with p.
                                        ITERATOR_TYPE iter = boost::find_if(this->collection(), SocketDetailSet::find_first_item_same_uuid(uuid));

                                        if (iter != this->collection().end()) {
                                                return (*iter).hdr;
                                        } else {
                                                return 0;
                                        }
                                }
                                void get_hdrs_with_heartjump_timeout(std::vector<void*>& hdrs) {
                                        foreach_(SocketDetail& n, this->collection() | adaptors::filtered(find_all_item_with_heartjump_timeout<TCP_HANDLER>())) {
                                                hdrs.push_back(n.hdr);
                                        }
                                }
                                void get_hdrs_with_authenticate_no_pass(std::vector<void*>& hdrs) {
                                        foreach_(SocketDetail& n, this->collection() | adaptors::filtered(find_all_item_with_authenticate_no_pass<TCP_HANDLER>())) {
                                                hdrs.push_back(n.hdr);
                                        }
                                }
                        };
                public:
                        boScoketManager() {

                        }
                        virtual ~boScoketManager() {
                        }

                        // Save and remove socket.
                        void add(string& uuid, void*& hdr) {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);

                                boScoketManager::SocketDetail* detail = new SocketDetail;
                                detail->hdr = hdr;
                                detail->uuid = uuid;

                                this->socks_.push_back(detail);
                        }
                        void erase(string& uuid) {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);

                                this->socks_.erase(uuid);
                        }

                        // Check all tcp_handler is heatjump pass, no then close socket.
                        void check_heartjump() {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);
                                std::vector<void*> hdrs;
                                this->socks_.get_hdrs_with_heartjump_timeout(hdrs);
                                foreach_(void*& n, hdrs) {
                                        TCP_HANDLER* p = static_cast<TCP_HANDLER*> (n);
                                        if (p) {
                                                boErrorWhat e_what;
                                                e_what.err_no(TCP_ERROR_TYPE_SERVER_CLOSE_SOCKET_BECAUSE_HEARTJUMP);
                                                e_what.err_message("server socket is been closed because heartbeat!");

                                                p->send_close_in_thread(e_what);
                                        }
                                }
                        }

                        // Check all tcp_handler is authentication pass, no then close socket.
                        void check_authentication_pass() {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);
                                std::vector<void*> hdrs;
                                this->socks_.get_hdrs_with_authenticate_no_pass(hdrs);
                                foreach_(void*& n, hdrs) {
                                        TCP_HANDLER* p = static_cast<TCP_HANDLER*> (n);
                                        if (p) {
                                                boErrorWhat e_what;
                                                e_what.err_no(TCP_ERROR_TYPE_SERVER_CLOSE_SOCKET_BECAUSE_AUTHENTICATE);
                                                e_what.err_message("server socket is been closed because authenticate!");

                                                p->send_close_in_thread(e_what);
                                        }
                                }
                        }

                        // Send data to socket.
                        void send_data_in_thread(string& uuid, const char* data, size_t data_size) {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);
                                void* p0 = this->socks_.get_hdr(uuid);
                                if (p0) {
                                        TCP_HANDLER* p1 = static_cast<TCP_HANDLER*> (p0);
                                        p1->send_data_in_thread(data, data_size);
                                }
                        }

                        // Send close to socket.
                        void send_close_in_thread(string & uuid) {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);
                                void* p0 = this->socks_.get_hdr(uuid);
                                if (p0) {
                                        boErrorWhat e_what;
                                        e_what.err_no(TCP_ERROR_TYPE_SERVER_ACTIVE_CLOSE_SOCKET);
                                        e_what.err_message("server active close socket!");

                                        TCP_HANDLER* p1 = static_cast<TCP_HANDLER*> (p0);
                                        p1->send_close_in_thread(e_what);
                                }
                        }

                        // Get socket-collection.
                        SocketDetailSet& collection() {
                                return socks_;
                        }

                        // Get socket-size.
                        size_t size() {
                                BO_SCOPE_LOCK_TYPE lock(mtx_);
                                return socks_.size();
                        }

                protected:
                        BO_MUTEX_TYPE mtx_;
                        SocketDetailSet socks_;
                };
        }
}

#endif /* BINGO_TCP_SOCKETMANAGER_HEADER_H_ */

