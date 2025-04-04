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
#include <memory>
#include <mutex>
#include <thread>
#include <variant>
#include <vector>

namespace v6
{
    class ExecutionSession;

    struct ParseJob
    {
        std::filesystem::path path;
    };

    using Job = std::variant<ParseJob>;

    struct JobSummaryFunctor
    {
        std::string operator()(ParseJob &job) const
        {
            return std::format("ParseJob ({})", job.path.string());
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

        JobExecutionStatus operator()(ParseJob &job) const;
    };

    class ExecutionSession
    {
        std::atomic<bool> shutdown_requested_ = false;

        std::mutex m_break_room_;
        std::condition_variable cv_break_room_;

        std::mutex m_jobs_;
        std::deque<Job> jobs_;

        std::atomic<int> time_card_;
        std::vector<std::thread> company_;

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
