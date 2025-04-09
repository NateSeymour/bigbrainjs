#include "v6turbo/engine/ExecutionSession.h"
#include <chrono>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include "v6turbo/parser/Parser.h"

using namespace v6;

JobExecutionStatus JobExecutionFunctor::operator()(ReadSourceJob &job) const
{
    // Load program text
    std::ifstream stream(job.module.path.c_str());
    std::string source(std::istreambuf_iterator<char>{stream}, {});

    job.source.set_value(std::move(source));
    job.module.source = job.source.get_future().share();

    return JobExecutionStatus::Success;
}

void ExecutionSession::Work(unsigned id)
{
    JobSummaryFunctor summarizer{};
    JobExecutionFunctor executor{*this, id};

    while (true)
    {
        std::unique_lock lk_break_room(this->m_break_room_);
        auto status = this->cv_break_room_.wait_for(lk_break_room, std::chrono::seconds(1));

        // Shutdown if requested
        if (this->shutdown_requested_)
        {
            std::cout << std::format("[ExecutionSession.Worker({})] Received shutdown request.\n", id);
            return;
        }

        // Only check job status if woken up on purpose
        if (status == std::cv_status::timeout)
        {
            continue;
        }

        // Grab next job if available.
        std::unique_lock lk_jobs(this->m_jobs_);
        if (this->jobs_.empty())
        {
            std::cerr << std::format("[ExecutionSession.Worker({})] WARNING Worker woken up without available job.\n", id);
            continue;
        }

        Job job = std::move(this->jobs_.front());
        this->jobs_.pop_front();

        lk_jobs.unlock();

        // Run job
        std::cout << std::format("[ExecutionSession.Worker({})] Received {}. Running...\n", id, std::visit(summarizer, job));

        auto result = std::visit(executor, job);
        switch (result)
        {
            case JobExecutionStatus::Failure:
            {
                throw std::runtime_error("unhandled job failure");
            }

            case JobExecutionStatus::Success:
            {
                std::cout << std::format("[ExecutionSession.Worker({})] Finished job successfully.\n", id);
                break;
            }

            case JobExecutionStatus::Deferred:
            {
                std::cout << std::format("[ExecutionSession.Worker({})] Job has been deferred.\n", id);

                this->Submit(std::move(job));
                break;
            }
        }
    }
}

ExecutionSession::ExecutionSession()
{
    unsigned thread_max = std::max(static_cast<unsigned>(1), std::thread::hardware_concurrency());
    this->company_.reserve(thread_max);

    for (unsigned i = 0; i < thread_max; i++)
    {
        this->company_.emplace_back(&ExecutionSession::Work, this, i);
    }

    std::cout << std::format("[ExecutionSession] Initialized {} workers.\n", thread_max);
}

ExecutionSession::~ExecutionSession()
{
    {
        std::lock_guard lk(this->m_break_room_);
        this->shutdown_requested_ = true;
    }
    this->cv_break_room_.notify_all();

    for (auto &worker : this->company_)
    {
        worker.join();
    }
}
