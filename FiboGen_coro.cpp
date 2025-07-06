#include <coroutine>
#include <iostream>
#include <utility>
class CoroType
{
public:
    struct promise_type{
        CoroType get_return_object()
        { 
            return CoroType{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        
        std::suspend_always initial_suspend()
        { return {}; }
        
        std::suspend_always yield_value(int val)
        {
            value = val;
            return {};
        };
        
        void return_void(){}
        void unhandled_exception(){}
        std::suspend_always final_suspend() noexcept { return {}; }

        int value;
    };

public:

    CoroType(std::coroutine_handle<promise_type> h) :
        handle(h)
    {}
public:
    // user interface members
    void resume() { handle.resume(); }
    int get_next() { 
        resume();
        return handle.promise().value; 
    }

private:
    // member vars
    std::coroutine_handle<promise_type> handle;
};

CoroType MakeFiboGen()
{
    int i1 = 1;
    int i2 = 1;
    while(1)
    {
        co_yield i1;
        i1 = std::exchange(i2, i1+i2);
    }
}


CoroType HelloCoro()
{
    std::cout<<"hello!"<<std::endl;
    //co_await  
    co_return;
};

int main()
{
    CoroType c = MakeFiboGen();
    std::cout<<c.get_next();
    std::cout<<c.get_next();
    std::cout<<c.get_next();
    std::cout<<c.get_next();
    std::cout<<c.get_next();
    return 0;
}
