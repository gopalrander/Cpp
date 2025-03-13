#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#define RUNNER_COMPLETION_MICROSECONDS 100
#define WAIT_AND_KILL_SECONDS 2

/// This example shows how a conditional variable is used incorrectly. 
/// The completion signal without a lamda on the conditional itself can totally miss the "notify_all" call, 
/// as there are chances that the "runner" is so fast.

/// @brief Runner mock. This is where actual work happens. 
/// Runner takes RUNNER_COMPLETION_MICROSECONDS seconds to finish 
void Runner()
{
    std::this_thread::sleep_for(std::chrono::microseconds(RUNNER_COMPLETION_MICROSECONDS));
    return;
}

void WaiterAndKiller()
{
    //create a thread with a completion signal.
    //take lock on the mutex and wait for conditional variable.

    std::mutex synchronizationMutex;
    std::condition_variable completionSignal;

    bool completed = false;

    std::thread killOnTimeout(
        [&]
        {
            std::unique_lock<std::mutex> completionSignalLock{synchronizationMutex};
            completionSignal.wait_for(completionSignalLock, std::chrono::seconds(WAIT_AND_KILL_SECONDS)
                //, [&] { return completed; }
            );

            if(!completed)
            {
                fwprintf(stderr, L"timed out");

                std::terminate();
            }
        });

    Runner();

    {
        std::unique_lock<std::mutex> completionSignalLock{synchronizationMutex};
        completed = true;
        
        completionSignal.notify_all();
    } 
    
    killOnTimeout.join();

    fwprintf(stderr, L"completed");
}


int main()
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    WaiterAndKiller();

    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

    std::cout<<"\nRuntime seconds "<< std::chrono::duration<double>(end_time - start_time).count();

    return 0;
}