/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "proxy_web_service_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace rpc {


HttpResponse::~HttpResponse() throw() {
}


void HttpResponse::__set_was_successful(const bool val) {
  this->was_successful = val;
}

void HttpResponse::__set_document_body(const std::string& val) {
  this->document_body = val;
}

uint32_t HttpResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->was_successful);
          this->__isset.was_successful = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->document_body);
          this->__isset.document_body = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t HttpResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("HttpResponse");

  xfer += oprot->writeFieldBegin("was_successful", ::apache::thrift::protocol::T_BOOL, 1);
  xfer += oprot->writeBool(this->was_successful);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("document_body", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->document_body);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(HttpResponse &a, HttpResponse &b) {
  using ::std::swap;
  swap(a.was_successful, b.was_successful);
  swap(a.document_body, b.document_body);
  swap(a.__isset, b.__isset);
}

HttpResponse::HttpResponse(const HttpResponse& other0) {
  was_successful = other0.was_successful;
  document_body = other0.document_body;
  __isset = other0.__isset;
}
HttpResponse& HttpResponse::operator=(const HttpResponse& other1) {
  was_successful = other1.was_successful;
  document_body = other1.document_body;
  __isset = other1.__isset;
  return *this;
}
void HttpResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "HttpResponse(";
  out << "was_successful=" << to_string(was_successful);
  out << ", " << "document_body=" << to_string(document_body);
  out << ")";
}

} // namespace
