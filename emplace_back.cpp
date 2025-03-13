#include <iostream>
#include <vector>

struct my_type
{
    int m_x{};
    int m_y{};

    my_type() = default;
    my_type(int x, int y): m_x{x}, m_y{y} 
    { puts("ctor");}

    my_type (const my_type& other)
    :   m_x{other.m_x}, 
        m_y{other.m_y}
    { puts("copy ctor");}

    my_type (my_type&& other) noexcept
    :   m_x{other.m_x}, 
        m_y{other.m_y}
    { puts("move ctor");}

    ~ my_type()
    { puts("dtor");}

};
int main()
{
    std::vector<my_type> p;
    p.reserve(4);
    auto t =  my_type(1,2); 
    p.emplace_back(std::move(t));
    puts("---------------------------");
    //p.emplace_back(1,2);
    return 0;
}