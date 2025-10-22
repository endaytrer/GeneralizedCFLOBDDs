
#include <cassert>
#include <functional>
#include "../utils/intpair.h"
#include "gcflobdd_node.h"
#include "return_map_T.h"

template<>
unsigned int ReturnMapBody<int>::Hash(unsigned int modsize) const
{
  unsigned int hvalue = 0;

  for (unsigned i = 0; i < mapArray.size(); i++)
  {
	  hvalue = (997* hvalue + mapArray[i]) % modsize;
  }
  return hvalue;
}

template<>
void ReturnMapBody<int>::setHashCheck()
{
  unsigned int hvalue = 0;

  for (unsigned i = 0; i < mapArray.size(); i++)
  {
	  hvalue = (117*(hvalue+1) + mapArray[i]);
  }
  hashCheck = hvalue;
}

template<> ReturnMapHandle<int> ReturnMapHandle<int>::Complement()
{
	ReturnMapHandle<int> answer;
	unsigned size = mapContents->mapArray.size();
	for (unsigned i = 0; i < size; i++)
	{
		answer.mapContents->mapArray.push_back(!mapContents->mapArray[i]);
	}
	answer.Canonicalize();
	return answer;
}