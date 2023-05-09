//
// Created by TSU on 2023-05-04.
//

#include "std.hpp"
#include "net/login_server.hpp"

int main(int argc, char* argv[])
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    (void)argc;
    (void)argv;
    plog::init(plog::verbose, &consoleAppender);

    hl::singleton<hl::socket_thread_pool>::get().begin();
    hl::singleton<hl::login::login_server>::get().begin("0.0.0.0", 7675);

    LOGI << "Stopped event loop..";
    return 0;
}