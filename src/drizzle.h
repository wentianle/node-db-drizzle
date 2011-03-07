#ifndef _NODE_DRIZZLE__DRIZZLE_H
#define _NODE_DRIZZLE__DRIZZLE_H

#include <vector>
#include <node.h>
#include <node_events.h>
#include "drizzle/connection.h"
#include "drizzle/result.h"
#include "drizzle_bindings.h"

using namespace v8;

namespace node_drizzle {
class Drizzle : public node::EventEmitter {
    public:
        static void Init(Handle<Object> target);

    protected:
        struct connect_request_t {
            Drizzle* drizzle;
            const char* error;
            Persistent<Function> cbSuccess;
            Persistent<Function> cbError;
        };
        struct query_request_t {
            bool buffer;
            bool runEach;
            Drizzle* drizzle;
            std::string query;
            drizzle::Result *result;
            const char* error;
            std::vector<std::string**>* rows;
            Persistent<Function> cbSuccess;
            Persistent<Function> cbError;
            Persistent<Function> cbEach;
        };
        drizzle::Connection *connection;

        Drizzle();
        ~Drizzle();
        static Handle<Value> New(const Arguments& args);
        static Handle<Value> Connect(const Arguments& args);
        static Handle<Value> Disconnect(const Arguments& args);
        static Handle<Value> Query(const Arguments& args);
        static int eioConnect(eio_req* req);
        static void connect(connect_request_t* request);
        static void connectFinished(connect_request_t* request);
        static int eioConnectFinished(eio_req* eioRequest);
        static int eioQuery(eio_req* eioRequest);
        static int eioQueryFinished(eio_req* eioRequest);
        static int eioQueryEach(eio_req* eioRequest);
        static int eioQueryEachFinished(eio_req* eioRequest);
        static void eioQueryCleanup(query_request_t* request);
        Local<Object> row(drizzle::Result* result, std::string** currentRow);
};
}

#endif
