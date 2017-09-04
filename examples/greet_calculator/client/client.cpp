#include <greet_service.pb.h>
#include <greet_service.grpc.pb.h>
#include <calculator_service.pb.h>
#include <calculator_service.grpc.pb.h>

#include <grpc++/grpc++.h>
#include <glog/logging.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./client <hostname>:<port>\n");
        return 0;
    }

    auto channel = grpc::CreateChannel(argv[1], grpc::InsecureChannelCredentials());
    GreetService::Stub greet_stub(channel);
    CalculatorService::Stub calculator_stub(channel);

    GreetRequest request_english;
    request_english.set_name("Kelvin");
    GreetRequest request_chinese;
    request_chinese.set_name("啟榮");
    CalculatorBinaryRequest request_add;
    request_add.set_x(3);
    request_add.set_y(4);
    CalculatorUnaryRequest request_square;
    request_square.set_x(5);
    
    grpc::ClientContext context_english;
    grpc::ClientContext context_chinese;
    grpc::ClientContext context_add;
    grpc::ClientContext context_square;
    GreetReply reply_english;
    GreetReply reply_chinese;
    CalculatorReply reply_add;
    CalculatorReply reply_square;

    grpc::CompletionQueue cq;

    std::unique_ptr<grpc::ClientAsyncResponseReader<GreetReply>> rpc_chinese(greet_stub.Asyncchinese(&context_chinese, request_chinese, &cq));
    std::unique_ptr<grpc::ClientAsyncResponseReader<CalculatorReply>> rpc_add(calculator_stub.Asyncadd(&context_add, request_add, &cq));
    std::unique_ptr<grpc::ClientAsyncResponseReader<CalculatorReply>> rpc_square(calculator_stub.Asyncsquare(&context_square, request_square, &cq));
    std::unique_ptr<grpc::ClientAsyncResponseReader<GreetReply>> rpc_english(greet_stub.Asyncenglish(&context_english, request_english, &cq));

    grpc::Status status_english, status_chinese, status_add, status_square;
    rpc_chinese->Finish(&reply_chinese, &status_chinese, (void*)2);
    rpc_add->Finish(&reply_add, &status_add, (void*)3);
    rpc_square->Finish(&reply_square, &status_square, (void*)4);
    rpc_english->Finish(&reply_english, &status_english, (void*)1);

    void* got_tag;
    bool ok = false;

    bool done_english = false;
    bool done_chinese = false;
    bool done_add = false;
    bool done_square = false;

    for (int i = 0; i < 4; ++i) {
        LOG_ASSERT(cq.Next(&got_tag, &ok));
        LOG_ASSERT(ok);

        if (got_tag == (void*)1) {
            if (status_english.ok()) {
                done_english = true;
                LOG(INFO) << "GreetService::english replies: " << reply_english.greet();
            } else {
                LOG(ERROR) << status_english.error_code() << ": " << status_english.error_message();
            }
        } else if (got_tag == (void*)2) {
            if (status_chinese.ok()) {
                done_chinese = true;
                LOG(INFO) << "GreetService::chinese replies: " << reply_chinese.greet();
            } else {
                LOG(ERROR) << status_chinese.error_code() << ": " << status_chinese.error_message();
            }
        } else if (got_tag == (void*)3) {
            if (status_add.ok()) {
                done_add = true;
                LOG(INFO) << "CalculatorService::add replies: " << reply_add.z();
            } else {
                LOG(ERROR) << status_add.error_code() << ": " << status_add.error_message();
            }
        } else if (got_tag == (void*)4) {
            if (status_square.ok()) {
                done_square = true;
                LOG(INFO) << "CalculatorService::square replies: " << reply_square.z();
            } else {
                LOG(ERROR) << status_square.error_code() << ": " << status_square.error_message();
            }
        } else {
            LOG(ERROR) << "Unknown tag: " << got_tag;
        }
    }

    return 0;
}

