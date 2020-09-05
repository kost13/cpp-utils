# CppUtils [work in progress]
![GitHub Actions](https://github.com/kost13/cpp-utils/workflows/cpp-utils-ci/badge.svg)

## Logger 
Thread-safe logging tool. It provides stream interface.
```
cpputils::log::info() << "File loaded in " << time << " ms";
// 2020-03-16 20:00:00: [    info] File loaded in 420 ms
```
It allows adding timestamp, logging category, hiding debug logs, exporting log to file.
User can construct Logger object or use static object defined in logger.cc and hidden from the user.
```
cpputils::Logger logger;
logger.warning() << "local logger";
cpputils::log::debug() << "whatever";
```

Logger supports std::vector 
```
std::vector<int> v = {1,2,3};
cpputils::log::info() << v;
// 2020-03-12 15:15:12: [    info] [1, 2, 3]
```

## Worker
Worker class allow executing functions in separate thread.
```
Worker worker;
worker.start() // starts worker thread
worker.async([]{ /* do some stuff */  }); // run lambda in worker's thread
worker.async([]{ /* do some stuff */  }); // function is added to the tasks queue
worker.priorityAsync([]{ /* do some stuff */  }); // function is added to the front
                                                     of the tasks queue
worker.async(std::bind(&Foo::method, foo_object, args...); // async call with std:bind
```

## GuardedThread
sd::thread that in destructor calls joint() if it's jointable.
