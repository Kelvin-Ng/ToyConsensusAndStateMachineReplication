#include <greet_service.grpc.pb.h>
#include <greet_service.pb.h>
#include <calculator_service.pb.h>
#include <calculator_service.grpc.pb.h>

#include <tcsmr/base/utils/config.hpp>
#include <tcsmr/base/net/server.hpp>
#include <tcsmr/base/net/server_completion_queue.hpp>

#include <thread>

class GreetServiceImpl {
   public:
    GreetServiceImpl() : english_num(0), chinese_num(0) {}

    GreetReply english(const GreetRequest& request) {
        LOG(INFO) << "Received request with name = " << request.name();

        ++english_num;

        std::string suffix;
        if (english_num >= 10 && english_num <= 20) {
            suffix = "-th";
        } else {
            switch (english_num % 10) {
               case 1:
                suffix = "-st";
                break;

               case 2:
                suffix = "-nd";
                break;

               case 3:
                suffix = "-rd";
                break;

               default:
                suffix = "-th";
                break;
            }
        }

        GreetReply reply;
        reply.set_greet("Hello, " + request.name() + "! You are the " + std::to_string(english_num) + suffix + " English user!");
        return reply;
    }

    GreetReply chinese(const GreetRequest& request) {
        LOG(INFO) << "Received request with name = " << request.name();

        ++chinese_num;

        GreetReply reply;
        reply.set_greet("你好，" + request.name() + "！你是第 " + std::to_string(chinese_num) + " 個中文使用者!");
        return reply;
    }

   private:
    int english_num, chinese_num;
};

class CalculatorServiceImpl {
   public:
    static CalculatorReply add(const CalculatorBinaryRequest& request) {
        LOG(INFO) << "Received request: " << request.x() << " + " << request.y();

        CalculatorReply reply;
        reply.set_z(request.x() + request.y());
        return reply;
    }

    static CalculatorReply square(const CalculatorUnaryRequest& request) {
        LOG(INFO) << "Received request: sqr(" << request.x() << ")";

        CalculatorReply reply;
        reply.set_z(request.x() * request.x());
        return reply;
    }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./service <conf_path>\n");
        return 0;
    }

    tcsmr::Config config(argv[1]);
    tcsmr::Server server(config.get_listen_endpoints());

    GreetService::AsyncService greet_service;
    CalculatorService::AsyncService calculator_service;

    server.register_service(&greet_service);
    server.register_service(&calculator_service);

    auto greet_english_cq = server.add_completion_queue();
    auto greet_chinese_cq = server.add_completion_queue();
    auto calculator_cq = server.add_completion_queue();

    server.serve();

    int num_thr_calculator = config.get_raw_config().get<int>("num_thr_calculator");
    std::vector<std::thread> thrs;
    thrs.reserve(num_thr_calculator + 2);

    GreetServiceImpl greet_service_impl;

    LOG(INFO) << "Start registering events";

    greet_english_cq.register_events<GreetRequest>(
        std::bind(&GreetServiceImpl::english, &greet_service_impl, std::placeholders::_1),
        &greet_service,
        &GreetService::AsyncService::Requestenglish,
        100);

    greet_chinese_cq.register_events<GreetRequest>(
        std::bind(&GreetServiceImpl::chinese, &greet_service_impl, std::placeholders::_1),
        &greet_service,
        &GreetService::AsyncService::Requestchinese,
        100);

    calculator_cq.register_events<CalculatorBinaryRequest>(
        std::bind(&CalculatorServiceImpl::add, std::placeholders::_1),
        &calculator_service,
        &CalculatorService::AsyncService::Requestadd,
        100);

    calculator_cq.register_events<CalculatorUnaryRequest>(
        std::bind(&CalculatorServiceImpl::square, std::placeholders::_1),
        &calculator_service,
        &CalculatorService::AsyncService::Requestsquare,
        100);

    LOG(INFO) << "All events registered";

    thrs.emplace_back(std::bind(&tcsmr::ServerCompletionQueue::event_loop, &greet_english_cq));
    thrs.emplace_back(std::bind(&tcsmr::ServerCompletionQueue::event_loop, &greet_chinese_cq));

    for (int i = 0; i < num_thr_calculator; ++i) {
        thrs.emplace_back(std::bind(&tcsmr::ServerCompletionQueue::event_loop, &calculator_cq));
    }

    for (int i = 0; i < num_thr_calculator + 2; ++i) {
        thrs[i].join();
    }

    return 0;
}

