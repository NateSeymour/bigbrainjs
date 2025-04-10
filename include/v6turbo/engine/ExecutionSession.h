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

    struct Module
    {
        std::shared_future<std::string> source;
        std::shared_future<parser::NodeRef> ast;
    };

    /**
     * Loads a Module's source text.
     */
    struct ReadSourceJob
    {
        Module module;
        std::filesystem::path path;
        std::promise<std::string> source;
    };

    /**
     * Parses the source of a module into its AST.
     */
    struct ParseJob
    {
        Module module;
        std::promise<parser::NodeRef> ast;
    };

    struct RenderJob
    {
    };

    struct ExecuteJob
    {
    };

    /**
     * Controls the loading, parsing and `main` execution of Module.
     */
    struct LoadModuleJob
    {
        bool preload = false;
        std::filesystem::path path;
    };

    using Job = std::variant<ReadSourceJob, ParseJob, RenderJob, ExecuteJob, LoadModuleJob>;

    struct JobSummaryFunctor
    {
        std::string operator()(ReadSourceJob &job) const
        {
            return "ReadSourceJob";
        }

        std::string operator()(ParseJob &job) const
        {
            return "ParseJob";
        }

        std::string operator()(RenderJob &job) const
        {
            return "RenderJob";
        }

        std::string operator()(ExecuteJob &job) const
        {
            return "ExecuteJob";
        }

        std::string operator()(LoadModuleJob &job) const
        {
            return "LoadModuleJob";
        }
    };

    enum class JobExecutionStatus
    {
        Failure,
        Success,
        Deferred,
    };

    struct JobExecutionFunctor
    {
        ExecutionSession &es;
        unsigned worker_id;

        JobExecutionStatus operator()(ReadSourceJob &job) const;
        JobExecutionStatus operator()(ParseJob &job) const;
        JobExecutionStatus operator()(RenderJob &job) const;
        JobExecutionStatus operator()(ExecuteJob &job) const;
        JobExecutionStatus operator()(LoadModuleJob &job) const;
    };

    class ExecutionSession
    {
        std::promise<int> result_;

        std::filesystem::path pwd_;

        std::atomic<bool> shutdown_requested_ = false;

        std::mutex m_break_room_;
        std::condition_variable cv_break_room_;

        std::mutex m_jobs_;
        std::deque<Job> jobs_;

        std::atomic<int> active_workers_ = 0;
        std::vector<std::thread> company_;

        std::mutex m_modules_;
        std::map<std::string, Module> modules_;

        unsigned patience_ = 1;

        void Work(unsigned id);

    protected:
        friend struct JobExecutionFunctor;

        [[nodiscard]] Module GetOrEmplaceModule(std::string const &name)
        {
            std::lock_guard lk(this->m_modules_);

            return this->modules_[name];
        }

        void Exit(int result);

    public:
        void Submit(Job &&job)
        {
            std::lock_guard lk(this->m_jobs_);

            this->jobs_.push_back(std::move(job));

            this->cv_break_room_.notify_one();
        }

        template<typename JobType, typename... Args>
        void Submit(Args... args)
        {
            this->Submit(JobType{std::forward<Args>(args)...});
        }

        [[nodiscard]] std::future<int> GetResult();

        void Shutdown();

        ExecutionSession(std::filesystem::path pwd, unsigned patience = 1);
        ~ExecutionSession();

        ExecutionSession(ExecutionSession &&) = delete;
        ExecutionSession(ExecutionSession const &) = delete;
    };

    using ExecutionSessionHandle = std::unique_ptr<ExecutionSession>;
} // namespace v6

#endif // EXECUTIONSESSION_H
