#include "ProxyWebService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <curl/curl.h>
#include "../cache/cache_base.h"
#include <iostream>
#include <cstdio>
#include <sstream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::rpc;


bool get_page(const std::string &url, CacheData &);

class ProxyWebServiceHandler : virtual public ProxyWebServiceIf {
  public:
    ProxyWebServiceHandler(CacheBase* cache) : _cache(cache), _hit_count(0), _total_count(0), _hit_flags(100, 2){ }

    void get_response(HttpResponse& response, const HttpRequest& request)
    {
        if(request == "stats")
        {
            std::cout << "Hit rate:  " << float(_hit_count)/_total_count  << std::endl;
            std::cout << "Hit flags: ";
            for (auto elem: _hit_flags)
                std::cout << elem << ' ';
            std::cout << std::endl;
            return;
        }

        CacheData data;
        if(_cache->get(request, data))
        {
            //std::cout << "hit:  " << request << std::endl;
            response.document_body = std::string(data.start(), data.size());
            response.was_successful = true;
            ++_total_count;
            ++_hit_count;
        }
        else if(get_page(request, data))
        {
            //std::cout << "shit:  " << request << std::endl;
            response.document_body = std::string(data.start(), data.size());
            response.was_successful = true;
            _cache->add(request, data);
            _hit_flags[_total_count++] = 1;
        }
        else { response.was_successful = false; }
    }
  
  private:
    CacheBase* _cache;
    int _hit_count;
    int _total_count;
    std::vector<int> _hit_flags;
};

CacheBase* get_cache_impl(const char* cache_policy, size_t cache_size);

int main(int argc, char **argv) {
    if(argc<5)
    {
        std::cerr << "Usage: ./prog $host $port_no $cache_policy $cache_size" << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream ss(argv[2]);
    int port;
    if (!(ss >> port))
    {   
        std::cerr << "Invalid number: " << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    std::istringstream ss2(argv[4]);
    size_t cache_size;
    if (!(ss2 >> cache_size))
    {   
        std::cerr << "Invalid number: " << argv[4] << std::endl;
        return EXIT_FAILURE;
    }

    CacheBase * cache = get_cache_impl(argv[3], cache_size);
    if(!cache) 
    {
        std::cerr << "Cache policy name didn't match any." << std::endl;
        return EXIT_FAILURE;
    }

    //size_t hit_count = 0;
    boost::shared_ptr<ProxyWebServiceHandler> handler(new ProxyWebServiceHandler(cache));
    boost::shared_ptr<TProcessor> processor(new ProxyWebServiceProcessor(handler));
    boost::shared_ptr<TServerTransport> transport(new TServerSocket(argv[1],port));
    boost::shared_ptr<TTransportFactory> tFactory(new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> pFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, transport, tFactory, pFactory);
    server.serve();
    std::cout << "finishing" << std::endl;

    return 0;
}

size_t write_callback(void *buffer, size_t size, size_t num, void *up) {
  size_t realsize = size * num;
  ((CacheData *)up)->fill((char*)buffer, realsize);
  return realsize;
}

bool get_page(const std::string &url, CacheData &data){
  CURL *curl = curl_easy_init();
  if(   curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    data.reset();
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
   
    if(curl_easy_perform(curl) != CURLE_OK){
      std::cerr << "Curl failed in performing operation for the url: "<< url << std::endl;
      return false;
    }
    
    curl_easy_cleanup(curl);
    return true;
  }
  else {
    std::cerr << "Failed in creating CURL handle." << std::endl;
    return false;
  }
}

CacheBase* get_cache_impl(const char* cache_policy, size_t cache_size)
{
  if(!cache_policy)
  { return NULL; }
  else if(strcmp("cache_no", cache_policy) == 0)
  { return new Cache_No(cache_size); }
  else if(strcmp("cache_lru", cache_policy) == 0)
  { return new Cache_LRU(cache_size); }
  else if(strcmp("cache_lfu", cache_policy) == 0)
  { return new Cache_LFU(cache_size); }
  else if(strcmp("cache_rnd", cache_policy) == 0)
  { return new Cache_RND(cache_size); }
  else 
  { return NULL; }
}
