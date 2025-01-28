//
// Created by davyxu on 2025/1/28.
//

#include "Application.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "spdlog/spdlog-inl.h"

#include "GarbageCollector.h"
#include "MemoryPool.h"

class Application : public IApplication {
public:

    void Init() override {
        InitLogger();
        spdlog::info("Application Init");
    }

    void Exit() override {

        GetGarbageCollector()->Collect();

        GetMemoryPool()->ReportLeak();

        spdlog::info("Application Exit");
    }

private:
    void InitLogger() {
        auto console = spdlog::stdout_color_mt("console");
        spdlog::set_default_logger(console);
        spdlog::set_level(spdlog::level::debug);

        auto formatter = std::make_unique<spdlog::pattern_formatter>();
        formatter->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        spdlog::set_formatter(std::move(formatter));
    }
};

IApplication *GetApplication() {
    static Application app;
    return &app;
}