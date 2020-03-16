#include <chrono>
#include <thread>
#include <vector>

#include "cpputils/logger.hpp"
#include "cpputils/worker.h"

void loggerExample() {
  // create logger object
  cpputils::Logger logger;

  // log with info prefix
  logger.info() << "we did it!";

  // log with warning prefix
  logger.warning() << "well, we didn't!";

  // use global logger
  namespace log = cpputils::log;

  log::exportToFile("./app_log.log");
  log::info() << 5 << 6;
  log::warning() << "5 " << 6.4;
  log::debug() << 5.2f << 'g';
  log::critical() << "No definition in " << 14 << " returning " << -1;

  std::vector<int> g;
  log::info() << g;
  g = {1, 2, 3};
  log::info() << g;

  // hide datetime
  logger.configure(cpputils::Logger::Config::SHOW_PREFIX |
                   cpputils::Logger::Config::SHOW_DEBUG);
  logger.critical() << "no timestamp";
}

void workerExample() {
  class Model {
   public:
    void runLongTask(const std::string &name) {
      for (int i = 1; i <= kModelTaskNum; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(kDelay));
        cpputils::log::info() << name << " " << i << "/" << kModelTaskNum;
      }
    }

    void runShortTask(const std::string &name) {
      cpputils::log::info() << name;
    }

   private:
    const int kModelTaskNum{8};
    const int kDelay{500};
  };

  class View {
   public:
    void runGuiStuff(const std::string &name) {
      for (int i = 1; i <= kViewTaskNum; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(kDelay));
        cpputils::log::info() << name << " " << i << "/" << kViewTaskNum;
      }
    }

   private:
    const int kViewTaskNum{6};  // should be smaller than kWorkerTaskNum
    const int kDelay{500};
  };

  Model model;
  View view;
  cpputils::Worker worker;
  worker.start();
  worker.async([m = &model] { m->runLongTask("computations"); });
  view.runGuiStuff("rendering gui");

  worker.async(std::bind(&Model::runLongTask, model, "computations 2"));
  worker.priorityAsync([m = &model] { m->runShortTask("small change"); });

  view.runGuiStuff("rendering gui again");
}

int main() {
  loggerExample();
  workerExample();

  return 0;
}
