#include "cache_base.h"

class Cache_No : public CacheBase 
{
  public:
    bool get(const std::string &, CacheData &)	{	return false;	};
	void add(const std::string &, CacheData &) 	{	};   

	Cache_No(size_t cache_size) : CacheBase(cache_size) {}
};