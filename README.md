# A HTTPProxyServer based on RPC

The idea is there is a RPC method which is served by a server.  
Another client calls this method and is executed remotely on the server.  

In addition the server is suppose to serve basically simple HTTP requests like GET.  
The server caches the result hence to provide better performance.  

Libraries used:  
1) libcurl for HTTP request  
2) Apache thrift for RPC calls  

Different caching schemes have been analyzed. They are:  
1) Least recently used  
2) Least frequently used  
3) Random  

Some test files are also there to measure the performance of these caching schemes.  

The client and servers are test on the same machine and on the different machines.  
