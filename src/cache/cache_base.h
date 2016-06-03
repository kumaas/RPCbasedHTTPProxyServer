#ifndef CACHE_BASE_H
#define CACHE_BASE_H

#include <string>

class CacheData
{
  public:
    bool isValid()  { return (_start != NULL) && (_size != 0); }
    
    void fill(char *start, size_t size) 
    { 
      _start = (char*)malloc(size);
      _size = size;
      memcpy(_start, start, size);
    }

    void assign(char *start, size_t size) 
    { 
      _start = start;
      _size = size;
    }
    
    // bool get_location(char *start_, size_t size_)
    // { return true;  }
    
    char * start() 
    { return _start; }

    size_t size() 
    { return _size; }

    void reset()
    { 
      if(_start) { free(_start); }
      _start = NULL;
      _size = 0;
    }
    
    CacheData() : _start(NULL), _size(0) {}

  private:
    char* _start;
    size_t _size;
};


class CacheBase 
{
  protected:
    static const int DEFAULT_CACHE_SIZE = 1 << 9;
  public:
    virtual bool get(const std::string &, CacheData &) = 0;
    virtual void add(const std::string &, CacheData &) = 0;   

    CacheBase(size_t max_size = DEFAULT_CACHE_SIZE) : _max_size(max_size){ }
    virtual ~CacheBase() {};

  protected:
    const size_t _max_size;
};

#include "cache_no.h"
#include "cache_lru.h"
#include "cache_lfu.h"
#include "cache_rnd.h"

#endif
