#include "v6turbo/engine/ExecutionSession.h"
#include <chrono>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include "v6turbo/parser/Parser.h"

using namespace v6;

ContextualModuleThread::ContextualModuleThread(ExecutionSession &es, std::filesystem::path relative_path) : es_(es), path_(std::move(relative_path))
{
    // Load Source
    auto const absolute_path = this->es_.package.root / this->path_;
    std::ifstream file(absolute_path);

    this->source_ = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    std::cout << "[" << absolute_path << "] Loaded source." << std::endl;
    std::cout << this->source_ << std::endl;
}

void ExecutionSession::Exit(int result) {}

ContextualModuleThread &ExecutionSession::InsertModule(std::filesystem::path relative_path)
{
    std::lock_guard lk(this->m_modules_);

    auto [it, inserted] = this->modules_.emplace(relative_path.string(), ContextualModuleThread{*this, relative_path});

    return it->second;
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
    this->Shutdown();
}
