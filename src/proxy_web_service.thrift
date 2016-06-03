#!/usr/local/bin/thrift -out rpc --gen cpp

namespace cpp rpc

struct HttpResponse {
       1: bool was_successful,
       2: string document_body,
}

typedef string HttpRequest;

service ProxyWebService {
  HttpResponse get_response(1:HttpRequest request)
}	
