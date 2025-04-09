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
#include "v6turbo/parser/Parser.h"

namespace v6
{
    class ExecutionSession;

    struct Module
    {
        std::filesystem::path path;

        std::shared_future<std::string> source;
        std::shared_future<parser::NodeRef> ast;
    };

    /**
     * Loads a Module's source text.
     */
    struct ReadSourceJob
    {
        std::promise<std::string> source;
        Module &module;
    };

    /**
     * Parses the source of a module into its AST.
     */
    struct ParseJob
    {
        std::promise<parser::NodeRef> ast;
        Module &module;
    };

    struct RenderJob
    {
    };

    /**
     * Controls the loading, parsing and `main` execution of Module.
     */
    struct LoadModuleJob
    {
    };

    using Job = std::variant<ReadSourceJob, ParseJob, RenderJob>;

    struct JobSummaryFunctor
    {
        std::string operator()(ReadSourceJob &job) const
        {
            return std::format("ReadSourceJob ({})", job.module.path.string());
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
    };

    class ExecutionSession
    {
        std::atomic<bool> shutdown_requested_ = false;

        std::mutex m_break_room_;
        std::condition_variable cv_break_room_;

        std::mutex m_jobs_;
        std::deque<Job> jobs_;

        std::atomic<int> active_workers_ = 0;
        std::vector<std::thread> company_;

        std::map<std::string, Module> modules_;

        int patience_ = 1;

        void Work(unsigned id);

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

        ExecutionSession();
        ~ExecutionSession();

        ExecutionSession(ExecutionSession &&) = delete;
        ExecutionSession(ExecutionSession const &) = delete;
    };

    using ExecutionSessionHandle = std::unique_ptr<ExecutionSession>;
} // namespace v6

#endif // EXECUTIONSESSION_H
