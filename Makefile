############# Include path ############# 
INCLUDE = -I$(BOOST_INCLUDE_ROOT) \
	-I$(OPENSSL_INCLUDE_ROOT) \
	-I$(MYSQL_INCLUDE_ROOT) \
	-I./


############# Library path and lib file ############# 
LIBS =  -lamqpcpp \
	-lpthread \
	-lmysqlclient_r \
	-lrt \
	-lcrypto \
	-lssl \
	-lboost_random \
	-lboost_regex \
	-lboost_chrono \
	-lboost_timer \
	-lboost_system \
	-lboost_thread \
	-lboost_context \
	-lboost_coroutine \
	-lboost_date_time \
	-lboost_unit_test_framework
	  
LIBPATH = -L$(BOOST_LIB_ROOT) \
	-L$(MYSQL_LIB_ROOT) \
	-L$(OPENSSL_LIB_ROOT)
 		   
		
############# Exe source file ############# 
OBJS =  main.o \
	bingo_v4/boString.o \
	bingo_v4/boErrorWhat.o \
	bingo_v4/boType.o \
	bingo_v4/boTimer.o \
	bingo_v4/boActionFactory.o \
	bingo_v4/configuration/boNode.o \
	bingo_v4/configuration/boConfigAction.o \
	bingo_v4/configuration/xml/boXmlParser.o \
	bingo_v4/configuration/json/boJsonParser.o \
	bingo_v4/log/boLogBase.o \
	bingo_v4/log/boLogInfo.o \
	bingo_v4/log/boLogLevel.o \
	bingo_v4/log/boLogOutput.o \
	bingo_v4/log/handler/boLogToConsole.o \
	bingo_v4/log/handler/boLogToFile.o \
	bingo_v4/log/handler/boLogToTask.o \
	bingo_v4/log/handler/boLogToRb.o \
	bingo_v4/thread/boThreadAction.o \
	bingo_v4/thread/boThreadTask.o \
	bingo_v4/thread/boThreadBigDataTask.o \
	bingo_v4/algorithm/boAlgorithmFactory.o \
	bingo_v4/algorithm/boUUID.o \
	bingo_v4/algorithm/boSha1.o \
	bingo_v4/algorithm/boCRC32.o \
	bingo_v4/algorithm/boDesCbc.o \
	bingo_v4/tcp/boTcpSvrHandler.o \
	bingo_v4/tcp/boTcpCetHandler.o \
	bingo_v4/tcp/boScoketManager.o \
	bingo_v4/database/boDbField.o \
	bingo_v4/database/boDbResult.o \
	bingo_v4/database/boDbRow.o \
	bingo_v4/database/boDbConnectDetail.o \
	bingo_v4/database/mysql/boDbMysqlVisitor.o \
	bingo_v4/rabbitmq/asio/asiohandler.o \
	bingo_v4/rabbitmq/imp/boRbSimpleSendor.o \
	bingo_v4/rabbitmq/imp/boRbSimpleReceiver.o \
	bingo_v4/rabbitmq/imp/boRbPublishSendor.o \
	bingo_v4/rabbitmq/imp/boRbPublishReceiver.o \
	bingo_v4/rabbitmq/imp/boRbRoutingSendor.o \
	bingo_v4/rabbitmq/imp/boRbRoutingReceiver.o \
	bingo_v4/rabbitmq/imp/boRbTopicSendor.o \
	bingo_v4/rabbitmq/imp/boRbTopicReceiver.o \
	bingo_v4/rabbitmq/imp/boRbWorkSendor.o \
	bingo_v4/rabbitmq/imp/boRbWorkReceiver.o \
	test/test_thread_task.o \
	test/tcp/test_server.o \
	test/tcp/test_client.o \
	test/rabbitmq/test_simple.o \
	test/rabbitmq/test_publish.o \
	test/rabbitmq/test_worker.o \
	test/rabbitmq/test_routing.o \
	test/rabbitmq/test_topic.o


############# SO source file ############# 		
CPPS = bingo_v4/boString.cpp \
	bingo_v4/boErrorWhat.cpp \
	bingo_v4/boType.cpp \
	bingo_v4/boTimer.cpp \
	bingo_v4/boActionFactory.cpp \
	bingo_v4/configuration/boNode.cpp \
	bingo_v4/configuration/boConfigAction.cpp \
	bingo_v4/configuration/xml/boXmlParser.cpp \
	bingo_v4/configuration/json/boJsonParser.cpp \
	bingo_v4/log/boLogBase.cpp \
	bingo_v4/log/boLogInfo.cpp \
	bingo_v4/log/boLogLevel.cpp \
	bingo_v4/log/boLogOutput.cpp \
	bingo_v4/log/handler/boLogToConsole.cpp \
	bingo_v4/log/handler/boLogToFile.cpp \
	bingo_v4/log/handler/boLogToTask.cpp \
	bingo_v4/log/handler/boLogToRb.cpp \
	bingo_v4/thread/boThreadAction.cpp \
	bingo_v4/thread/boThreadTask.cpp \
	bingo_v4/thread/boThreadBigDataTask.cpp \
	bingo_v4/algorithm/boAlgorithmFactory.cpp \
	bingo_v4/algorithm/boUUID.cpp \
	bingo_v4/algorithm/boSha1.cpp \
	bingo_v4/algorithm/boCRC32.cpp \
	bingo_v4/algorithm/boDesCbc.cpp \
	bingo_v4/tcp/boTcpSvrHandler.cpp \
	bingo_v4/tcp/boTcpCetHandler.cpp \
	bingo_v4/tcp/boScoketManager.cpp \
	bingo_v4/database/boDbField.cpp \
	bingo_v4/database/boDbResult.cpp \
	bingo_v4/database/boDbRow.cpp \
	bingo_v4/database/boDbConnectDetail.cpp \
	bingo_v4/database/mysql/boDbMysqlVisitor.cpp \
	bingo_v4/rabbitmq/asio/asiohandler.cpp \
	bingo_v4/rabbitmq/imp/boRbSimpleSendor.cpp \
	bingo_v4/rabbitmq/imp/boRbSimpleReceiver.cpp \
	bingo_v4/rabbitmq/imp/boRbPublishSendor.cpp \
	bingo_v4/rabbitmq/imp/boRbPublishReceiver.cpp \
	bingo_v4/rabbitmq/imp/boRbRoutingSendor.cpp \
	bingo_v4/rabbitmq/imp/boRbRoutingReceiver.cpp \
	bingo_v4/rabbitmq/imp/boRbTopicSendor.cpp \
	bingo_v4/rabbitmq/imp/boRbTopicReceiver.cpp \
	bingo_v4/rabbitmq/imp/boRbWorkSendor.cpp \
	bingo_v4/rabbitmq/imp/boRbWorkReceiver.cpp
		

############# Debug argument ############# 

# debug_tcp_server
ifeq ($(debug_tcp_server),y)
DEBUG_TCP_SERVER = -DBINGO_DEBUG_TCP_SERVER
else
DEBUG_TCP_SERVER =
endif

# debug_tcp_client
ifeq ($(debug_tcp_client),y)
DEBUG_TCP_CLIENT = -DBINGO_TCP_CLIENT_DEBUG
else
DEBUG_TCP_CLIENT =
endif

# debug_mysql
ifeq ($(debug_mysql),y)
DEBUG_MYSQL = -DBINGO_MYSQL_DEBUG
else
DEBUG_MYSQL =
endif

# debug_thread_task
ifeq ($(debug_thread_task),y)
DEBUG_THREAD_TASK = -DBINGO_DEBUG_THREAD_TASK
else
DEBUG_THREAD_TASK =
endif

# debug_process_task
ifeq ($(debug_process_task),y)
DEBUG_PROCESS_TASK = -DBINGO_PROCESS_SHARED_MEMORY_DEBUG
else
DEBUG_PROCESS_TASK =
endif

# debug_rabbitmq
ifeq ($(debug_rabbitmq),y)
DEBUG_MQ = -DBINGO_MQ_DEBUG
else
DEBUG_MQ =
endif

# define_sart_message_type
ifeq ($(define_sart_message_type),y)
DEFINE_SART_MESSAGE_TYPE = -DBINGO_SART_MESSAGE_TYPE
else
DEFINE_SART_MESSAGE_TYPE =
endif


############# Compile option #############
ifeq ($(findstring Test_Debug,$(ConfigName)),Test_Debug)
	CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=c++11 $(INCLUDE) $(DEBUG_TCP_SERVER) \
									$(DEBUG_TCP_CLIENT) \
									$(DEBUG_THREAD_TASK) \
									$(DEBUG_PROCESS_TASK) \
									$(DEBUG_MYSQL) \
									$(DEBUG_MQ) \
									$(DEFINE_SART_MESSAGE_TYPE)
	TARGET = mytest
else ifeq ($(findstring Lib_Debug,$(ConfigName)),Lib_Debug)
	CXXFLAGS =	-O0 -g -fPIC  -shared -std=c++11 $(INCLUDE) $(DEBUG_TCP_SERVER) \
									$(DEBUG_TCP_CLIENT) \
									$(DEBUG_THREAD_TASK) \
									$(DEBUG_PROCESS_TASK) \
									$(DEBUG_MYSQL) \
									$(DEBUG_MQ) \
									$(DEFINE_SART_MESSAGE_TYPE)
	TARGET = libbingo_v4_d.so
else ifeq ($(findstring Lib_Release,$(ConfigName)),Lib_Release)
	CXXFLAGS =	-O2 -fPIC  -shared -std=c++11 $(INCLUDE) $(DEFINE_SART_MESSAGE_TYPE)
	TARGET = libbingo_v4.so
endif

############# Run to compile  #############
ifeq ($(findstring Test_,$(ConfigName)),Test_)
$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(LIBPATH);

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
else ifeq ($(findstring Lib_,$(ConfigName)),Lib_)
all:
	$(CXX)  $(CXXFLAGS) -o $(TARGET) $(CPPS) $(LIBS) $(LIBPATH);
	cp $(TARGET) $(MY_LIB_ROOT_v3);
clean:
	rm -f $(TARGET)
endif


