//
// Created by TSU on 2023-05-02.
//

#include <std.hpp>

int main(int argc, char* argv[])
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    (void)argc;
    (void)argv;
    plog::init(plog::verbose, &consoleAppender);

    return 0;
}