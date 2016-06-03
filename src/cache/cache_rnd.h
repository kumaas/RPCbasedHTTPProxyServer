#include "cache_base.h"
#include <unordered_map>
#include <vector>
#include <cstdlib>

class Cache_RND : public CacheBase 
{
  typedef std::pair<size_t, CacheData> LocInfoPair;
  typedef std::vector<std::string> IDs;
  typedef std::unordered_map<std::string, LocInfoPair> CacheMap;

  public:
  	Cache_RND(size_t max_size = CacheBase::DEFAULT_CACHE_SIZE) : CacheBase(max_size), _current_size(0) {}

    bool get(const std::string & id, CacheData & data)
    {
    	auto m_itr = _map.find(id);
  		if (m_itr != _map.end())
	    {
			CacheData& temp_data = m_itr->second.second;
			data.assign(temp_data.start(), temp_data.size());
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
			  // H[url1] = mid, p2
			  // H[url2] = last, p1
			  // A [....url1... url2]
			  // mid = random loc
			  // A[mid ] = url2
			  // A.removelast
			  // H [url2].first = mid

			size_t last = _ids.size() - 1;
			size_t loc = rand() % (last + 1);
			auto m_itr = _map.find(_ids[loc]);
			_current_size -= m_itr->second.second.size();
			_map.erase(m_itr);
			if(loc != last)
			{
				_ids[loc] = _ids[last];
				_map.find(_ids[last])->second.first = loc;
			}
			_ids.pop_back();
		}

		CacheData new_data;
		new_data.fill(data.start(), data.size());
		_current_size += data.size();
		_ids.push_back(id);
		std::pair<std::string, LocInfoPair> new_pair(id, LocInfoPair(_ids.size() - 1, new_data ));
		_map.insert(new_pair);
		//std::cout << "curr: " << _current_size << std::endl;
	};   

  private:
  	IDs _ids;
  	CacheMap _map;
  	size_t _current_size;
};