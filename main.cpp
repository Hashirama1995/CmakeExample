#include "lib.h"

#include <spdlog/spdlog.h>

int main()
{
    // auto logger = spdlog::stdout_logger_mt("console");
    // logger->info("version {} was started", version());

    spdlog::info("Welcome to spdlog!");

    return 0;
}

