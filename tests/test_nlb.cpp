#include "sender_recipient.h"
#include "../src/network_load_balancer.h"
#include "../src/parser.h"

#include <gtest/gtest.h>
#include <thread>

TEST (NLB, evarege_load) {
    using namespace My_NLB;
    try {
        Parser prs;
        NLB nlb(std::move(prs.getConns()), prs.getNomps());
        uint32_t count_recipient_1{0};
        uint32_t count_recipient_2{0};
        uint32_t count_recipient_3{0};
        std::thread(recipient, 12346, std::ref(count_recipient_1)).detach();
        std::thread(recipient, 12347, std::ref(count_recipient_2)).detach();
        std::thread(recipient, 12348, std::ref(count_recipient_3)).detach();
        std::thread(sender, 12345).join();

        ASSERT_TRUE((count_recipient_1 + count_recipient_2 + count_recipient_3) / 3 <= prs.getNomps);
    } catch (const exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception!" << std::endl;
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}