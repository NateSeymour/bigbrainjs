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

    template<typename T>
    class SharedAttribute
    {
        std::mutex m_value_;
        std::condition_variable cv_value_;

        std::optional<T> value_;

    protected:
        T const &Set(T &&value)
        {
            {
                std::lock_guard lk(this->m_value_);

                this->value_ = std::move(value);
            }

            this->cv_value_.notify_all();

            return this->value_;
        }

    public:
        [[nodiscard]] T const &Get()
        {
            if (!this->value_.has_value())
            {
                std::unique_lock lk(this->m_value_);
                this->cv_value_.wait(lk, [this] { return this->value_.has_value(); });
            }

            return this->value_;
        }

        SharedAttribute<T> &operator=(T &&value)
        {
            this->Set(std::move(value));

            return *this;
        }
    };

    struct Module
    {
        SharedAttribute<std::filesystem::path> path;
        SharedAttribute<std::string> source;
        SharedAttribute<parser::NodeRef> ast;
    };

    /**
     * Loads a Module's source text.
     */
    struct ReadSourceJob
    {
        Module &module;
    };

    /**
     * Parses the source of a module into its AST.
     */
    struct ParseJob
    {
        Module &module;
    };

    struct RenderJob
    {
        Module &module;
    };

    struct ExecuteJob
    {
        Module &module;
    };

    /**
     * Controls the loading, parsing and `main` execution of Module.
     */
    struct LoadModuleJob
    {
        std::filesystem::path path;
    };

    struct RunMainModuleJob
    {
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

        [[nodiscard]] Module &GetOrEmplaceModule(std::string const &name)
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
