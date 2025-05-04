#include "v6turbo/engine/ExecutionSession.h"
#include <chrono>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include "v6turbo/parser/Parser.h"

using namespace v6;

void ContextualModuleThread::Log(char const *log)
{
    std::cout << "[" << this->path_ << "] " << log << std::endl;
}

void ContextualModuleThread::Fence() {}

void ContextualModuleThread::Worker()
{
    // Load Source
    auto const absolute_path = this->es_.package.root / this->path_;
    std::ifstream file(absolute_path);

    this->Log("Loading source...");
    this->source_ = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    this->Log("Loaded source.");

    this->Log("Parsing...");
    auto ast = parser::Parser->Parse(this->source_);

    if (!ast.has_value())
    {
        this->Log("Failed to parse.");
        this->Log(ast.error().what());
        return;
    }

    this->Log("Parsed source.");
}

void ContextualModuleThread::Join()
{
    this->thread_.join();
}

ContextualModuleThread::ContextualModuleThread(ExecutionSession &es, std::filesystem::path relative_path) : es_(es), path_(std::move(relative_path)), thread_(&Worker, this) {}

void ExecutionSession::Exit(int result) {}

ContextualModuleThread &ExecutionSession::InsertModule(std::filesystem::path relative_path)
{
    std::lock_guard lk(this->m_modules_);

    auto [it, inserted] = this->modules_.emplace(relative_path.string(), std::make_unique<ContextualModuleThread>(*this, relative_path));

    return *it->second;
}

void ExecutionSession::RunMain()
{
    std::cout << "[ExecutionSession] Running main module from " << *this->package.main << "." << std::endl;
    (void)this->InsertModule(*this->package.main);
}

void ExecutionSession::Shutdown() {}

ExecutionSession::ExecutionSession(Package package) : package(std::move(package)) {}

ExecutionSession::~ExecutionSession()
{
    for (auto &[name, ctm] : this->modules_)
    {
        ctm->Join();
    }
}
