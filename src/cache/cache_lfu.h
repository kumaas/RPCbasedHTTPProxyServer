#include "cache_base.h"
#include <unordered_map>
#include <utility>

class Cache_LFU : public CacheBase 
{
  typedef std::tuple<int, std::string, CacheData> FreqIDInfoTuple;
  typedef std::list< FreqIDInfoTuple* > CacheEntries;
  typedef std::unordered_map<std::string, CacheEntries::iterator* > CacheMap;

  public:
  	Cache_LFU(size_t max_size = CacheBase::DEFAULT_CACHE_SIZE) : CacheBase(max_size), _current_size(0) {}

    bool get(const std::string & id, CacheData & data)
    {
    	auto m_itr = _map.find(id);
  		if (m_itr != _map.end())
	    {
			auto l_itr_p = m_itr->second;
			auto tuple_p = **l_itr_p;
			CacheData& temp_data = std::get<2>(*tuple_p);
			data.assign(temp_data.start(), temp_data.size());
			
			int curr_pr = ++std::get<0>(***l_itr_p);
			auto next_pos_itr = *l_itr_p;
			if(++next_pos_itr != _entries.end())
			{
				_entries.erase(*l_itr_p);
				while( (next_pos_itr != _entries.end()) && (std::get<0>(**next_pos_itr) < curr_pr) )	
					++next_pos_itr;
				m_itr->second = new CacheEntries::iterator(_entries.insert(next_pos_itr, tuple_p));
				delete l_itr_p;
			}
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
			auto& tuple = *_entries.front();
			_current_size -= std::get<2>(tuple).size();
			auto l_itr_p = _map.find(std::get<1>(tuple))->second;
			_map.erase(std::get<1>(tuple));
			delete &tuple;
			_entries.pop_front();
			delete l_itr_p;
		}

		CacheData new_data;
		new_data.fill(data.start(), data.size());
		_current_size += data.size();
		_entries.push_front(new FreqIDInfoTuple(1, id, new_data));
		std::pair<std::string, CacheEntries::iterator* > new_pair(id, new CacheEntries::iterator(_entries.begin()) );
		_map.insert(new_pair);
	};   

  private:
  	CacheEntries _entries;
  	CacheMap _map;
  	size_t _current_size;
};