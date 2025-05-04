//
// Created by Nathan Seymour on 4/4/2025.
//

#ifndef EXECUTIONSESSION_H
#define EXECUTIONSESSION_H

#include <atomic>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <format>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <variant>
#include <vector>
#include "Package.h"
#include "v6turbo/parser/Parser.h"

namespace v6
{
    class ExecutionSession;

    class ContextualModuleThread
    {
        ExecutionSession &es_;

        std::filesystem::path path_;
        std::string source_;
        // parser::NodeRef ast_;

        std::thread thread_;

        void Log(char const *log);

        /**
         * Called before making any external calls.
         */
        void Fence();

        /// 1. load
        /// 2. parse
        /// 3. compile
        /// 4. run
        /// 5. standby for requests
        void Worker();

    protected:
        friend class ExecutionSession;

        void Join();

    public:
        ContextualModuleThread(ExecutionSession &es, std::filesystem::path relative_path);
    };

    class ExecutionSession
    {
        std::mutex m_modules_;
        std::map<std::string, std::unique_ptr<ContextualModuleThread>> modules_;

    protected:
        friend class ContextualModuleThread;

        Package package;

        void Exit(int result);

        [[nodiscard]] ContextualModuleThread &InsertModule(std::filesystem::path relative_path);

    public:
        void Shutdown();

        void RunMain();

        ExecutionSession(Package package);
        ~ExecutionSession();

        ExecutionSession(ExecutionSession &&) = delete;
        ExecutionSession(ExecutionSession const &) = delete;
    };

    using ExecutionSessionHandle = std::unique_ptr<ExecutionSession>;
} // namespace v6

#endif // EXECUTIONSESSION_H
