#ifndef _MATCHER_HPP
#define _MATCHER_HPP

#include <set>

#include "CB.hpp"

class Matcher {
public:
  virtual ~Matcher(){}
  virtual bool isMatched(CB) const = 0;
  virtual int findLastMatched(int) const = 0;
  virtual const std::set<CB> & findMatchedSends(CB) const = 0;
};
#endif
