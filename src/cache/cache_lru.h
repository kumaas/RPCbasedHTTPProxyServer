#include "cache_base.h"
#include <unordered_map>
#include <utility>

class Cache_LRU : public CacheBase 
{
  typedef std::pair<std::string, CacheData> IdInfoPair;
  typedef std::list< IdInfoPair* > CacheEntries;
  typedef std::unordered_map<std::string, CacheEntries::iterator* > CacheMap;

  public:
  	Cache_LRU(size_t max_size = CacheBase::DEFAULT_CACHE_SIZE) : CacheBase(max_size), _current_size(0) {}

    bool get(const std::string & id, CacheData & data)
    {
    	auto m_itr = _map.find(id);
  		if (m_itr != _map.end())
	    {
			auto l_itr_p = m_itr->second;
			auto pair_p = **l_itr_p;
			CacheData& temp_data = pair_p->second;
			data.assign(temp_data.start(), temp_data.size());
			
			_entries.erase(*l_itr_p);
			_entries.push_front(pair_p);
			m_itr->second = new CacheEntries::iterator(_entries.begin());
			delete l_itr_p;
			return true;
	    }
	    return false;

    };
	
	void add(const std::string & id, CacheData & data)
	{
		if (data.size() > _max_size) {	return;	}
		if (_map.find(id) != _map.end())	return;
		while ((_max_size - _current_size) < data.size())
		{
			auto pair_p = _entries.back();
			_current_size -= pair_p->second.size();
			auto l_itr_p = _map.find(pair_p->first)->second;
			_map.erase(pair_p->first);
			delete pair_p;
			_entries.pop_back();
			delete l_itr_p;
		}

		CacheData new_data;
		new_data.fill(data.start(), data.size());
		_current_size += data.size();
		_entries.push_front(new IdInfoPair(id, new_data));
		std::pair<std::string, CacheEntries::iterator* > new_pair(id, new CacheEntries::iterator(_entries.begin()) );
		_map.insert(new_pair);
	};   

  private:
  	CacheEntries _entries;
  	CacheMap _map;
  	size_t _current_size;
};