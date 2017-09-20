/*
 * asiohandler.h
 *
 *  Created on: 2016-9-17
 *      Author: root
 */

#ifndef BINGO_RABBITMQ_ASIO_ASIOHANDLER1_H_
#define BINGO_RABBITMQ_ASIO_ASIOHANDLER1_H_

#ifndef WIN32

#include <deque>
#include <vector>
#include <memory>

#include <amqpcpp.h>
using namespace AMQP;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

#include "bingo_v4/tcp/boTcpClient.h"
#include "bingo_v4/tcp/boTcpCetHandler.h"
#include "bingo_v4/tcp/boTcpError.h"
using namespace bingo::TCP;
#include "bingo_v4/log/boLogHandler.h"
#include "bingo_v4/boType.h"
#include "bingo_v4/boString.h"
using namespace bingo;
using namespace bingo::log;

#define RABBITMQ_ASIOHANDLERLOG_TAG "rabbitmq_asiohandler"
#define RABBITMQ_MAX_WAIT_FOR_HEARTBEAT_SECONDS  90

// Rabbitmq connect parser
struct boRbAsioParser {
        static int retry_delay_seconds; // when client connect fail, reconnect wait for max seconds, if the value is 0, then reconnect don't call.
        static int max_retry_delay_seconds;

        static int max_interval_seconds_send_heartjump; // If value is 0, then don't send heartjump.
        static int max_interval_seconds_check_heartjump; // If value is 0, then don't check heartjump.
};

// Rabbitmq handler class.
class boRbAsioHandler : public AMQP::ConnectionHandler, public boTcpCetHandler {
public:
        boRbAsioHandler(BO_ASYNC_SERVICE& io_service,
                     int max_interval_seconds_check_heartjump,
                     reconnect_func f1,
                     authenticate_pass_func f2);
        virtual ~boRbAsioHandler();
protected:
        void onData(Connection* connection, const char* buffer, size_t size) override;
        void onConnected(Connection* connection) override;
        void onError(Connection* connection, const char* message) override;
        void onClosed(Connection* connection) override;
        void onHeartbeat(Connection* connection) override;


protected:
        void catch_error_func(pointer, boErrorWhat&) override;
        void close_complete_func(pointer, int&) override;

        void message_split_action(const char* in, size_t in_size, std::vector<string>& message_set) override;
        int make_first_sended_package_func(string&, boErrorWhat&) override;


        int read_pk_full_complete_func(pointer, const char*, size_t, boErrorWhat&) override;
        void write_pk_full_complete_func(pointer, const char*, size_t, int) override;

        void parse_data(const char* in, size_t in_size);
        string remain_data_;

protected:
        AMQP::Connection* _connection;
        bool is_first_;
};

typedef boTcpClient<boRbAsioHandler, boRbAsioParser> RABBITMQ_CET_BASE_TYPE;
class boRbAsioClient : public RABBITMQ_CET_BASE_TYPE {
public:
        boRbAsioClient(BO_ASYNC_SERVICE& io_service, string& ipv4, u16_t& port, make_heartjump_message_func f1);
        virtual ~boRbAsioClient();

        // Make heartjump package.
        static void make_heartjump(string& out);

protected:
        void connet_fail_func(pointer ptr, int& retry_delay_seconds) override;
        int connet_success_func(pointer ptr, boErrorWhat& e)override;

};

#endif

#endif /* BINGO_RABBITMQ_ASIO_ASIOHANDLER_H_ */
