#pragma once

#include <vector>
#include <array>
#include "Value.h"

class PatternNames;

typedef std::vector<Value*> ValList;
typedef std::vector<const char*> NameList;

class PatternBase
{
protected:
  ValList vals;
  NameList names;

public:
  virtual void update() = 0;
  ValList* get_vals();
  const NameList* get_names() const;
  void add_val(const char * name, Value* val);

  static const char** get_patterns();
  static int num_patterns();
  static PatternBase * get_pattern(int idx);
  
};

#include "TestPatterns.h"
#include "Clocks.h"
#include "SoundVisualizer.h"

