/*
 * ServerCompletionQueue. A wrapper over grpc::ServerCompletionQueue.
 * Copyright (C) 2017  Kelvin Ng
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <grpc++/grpc++.h>
#include <glog/logging.h>

namespace tcsmr {

class ServerCompletionQueue {
   private:
    class RequestHandlerBase {
     public:
        RequestHandlerBase(grpc::ServerCompletionQueue* cq)
            : cq_(cq) {}

        virtual ~RequestHandlerBase() {}

        virtual void start() = 0;
        virtual void process() = 0;

        void proceed() {
            if (status_ == PROCESS) {
                process();
            } else {
                start();
            }
        }

     protected:
        grpc::ServerContext ctx_;
        grpc::ServerCompletionQueue* cq_;
        enum Status {PROCESS, END};
        Status status_;
    };

    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    std::vector<std::unique_ptr<RequestHandlerBase>> handlers_;

   public:
    ServerCompletionQueue(std::unique_ptr<grpc::ServerCompletionQueue>&& cq) : cq_(std::move(cq)) {}

    template <typename RequestT, typename ServiceT, typename CallbackT, typename RequestStartT>
    auto register_events(CallbackT&& callback, ServiceT* service, RequestStartT&& request_start, unsigned num = 1) {
        using ReplyT = std::result_of_t<CallbackT(const RequestT&)>;
        using ResponserT = grpc::ServerAsyncResponseWriter<ReplyT>;

        class RequestHandler : public RequestHandlerBase {
           public:
            RequestHandler(CallbackT callback, ServiceT* service, RequestStartT request_start, grpc::ServerCompletionQueue* cq)
                : RequestHandlerBase(cq), responser_(&ctx_), callback_(callback), service_(service), request_start_(request_start) {}

            virtual void start() override final {
                ctx_.~ServerContext();
                new (&ctx_)(grpc::ServerContext);

                status_ = RequestHandler::PROCESS;
                (service_->*request_start_)(&ctx_, &request_, &responser_, cq_, cq_, this);
            }

            virtual void process() override final {
                ReplyT reply = callback_(request_);

                status_ = RequestHandler::END;

                responser_.Finish(reply, grpc::Status::OK, this);
            }

           protected:
            CallbackT callback_;
            ServiceT* service_;
            RequestStartT request_start_;

            RequestT request_;
            ResponserT responser_;
        };

        struct RequestHandlerType {
            using type = RequestHandler;
        };

        handlers_.reserve(handlers_.size() + num);

        for (unsigned i = 0; i < num; ++i) {
            handlers_.emplace_back(std::make_unique<RequestHandler>(callback, service, request_start, cq_.get()));
            handlers_.back()->start();
        }

        return RequestHandlerType();
    }

    // If the user is sure that there is only a certain type of request,
    // he / she can set the type to eliminate the cost of virtual function calls.
    template <typename RequestHandlerT = RequestHandlerBase>
    void event_loop() {
        void* tag;
        bool ok;

        while (true) {
            // TODO: there seems to be some bugs: https://github.com/grpc/grpc/issues/12364
            LOG_ASSERT(cq_->Next(&tag, &ok));
            LOG_ASSERT(ok);

            RequestHandlerBase* handler = reinterpret_cast<RequestHandlerBase*>(tag);
            handler->proceed();
        }
    }
};

}  // namespace tcsmr

