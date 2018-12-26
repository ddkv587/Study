#include <iostream>
#include <asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void time_handle( const asio::error_code& e, asio::deadline_timer* t, int* count )
{
    if ( *count < 5 ) {
        std::cout << "Hello asio with async! index=" << *count << std::endl;
        ++( *count );
        t->expires_at( t->expires_at() + boost::posix_time::seconds( 1 ) );
        t->async_wait( boost::bind( time_handle, asio::placeholders::error, t, count ) );
    }
}

int main( int argc, char** argv )
{
    asio::io_context        io;
    asio::deadline_timer    t( io, boost::posix_time::seconds( 1 ) );
#if 0   // sync time
    t.wait();
    std::cout << "Hello asio with sync!" << std::endl;
#endif
    
    int count = 0;
    t.async_wait( boost::bind( time_handle, asio::placeholders::error, &t, &count ) );
    
    io.run();

    std::cout << "final count: " << count << std::endl;

    return 0;
}