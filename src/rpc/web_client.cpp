#include "ProxyWebService.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include "../cache/time_util.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::rpc;
using namespace std;

typedef unsigned long long my_time_t;
my_time_t get_time()
{
	struct timeval time;
	gettimeofday(&time,NULL);
	return time.tv_usec+(my_time_t)time.tv_sec*1000000;
}

int main(int argc, char **argv) {
    if(argc<4)
    {
        std::cerr << "Usage: ./prog $host $port_no $workload_filename" << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream ss(argv[2]);
    int port;
    if (!(ss >> port))
    {   
        std::cerr << "Invalid number: " << argv[3] << std::endl;
        return EXIT_FAILURE;
    }

    boost::shared_ptr<TSocket> socket(new TSocket(argv[1], port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ProxyWebServiceClient client(protocol);

    string line;
    ifstream inputfile(argv[3]);

    unsigned long long total_time = 0;
    size_t numPages = 1;
    if(inputfile.is_open())
    {
        while(getline(inputfile,line))
        {
            transport->open();
            HttpRequest url(line);
            HttpResponse result;
            auto start = util::get_time();
            client.get_response(result, url);
            auto end = util::get_time();
            auto time_taken=util::get_diff_ms(end, start);
            total_time += time_taken;
            std::cout << "Page no. " << numPages++ << "  URL: " << url << "  Success: " << result.was_successful 
                    << "   Size: " << result.document_body.length() << "  time: " << time_taken << std::endl;
            transport->close();
        }
        transport->open();
        HttpRequest url("stats");
        HttpResponse result;
        client.get_response(result, url);
        transport->close();
    }
    else
    {
        cout<<"Couldn`t open workload"<<endl;
        return 1;
    }
    
    std::cout << "Total time: " <<  total_time/1000 << " micros."<< std::endl;
    return 0;
}

