#include "Patterns.h"
#include "Value.h"
#include "macros.h"

 ValList* PatternBase::get_vals()
  {
    return &vals;
  }
  const NameList* PatternBase::get_names() const
  {
    return &names;
  }
  void PatternBase::add_val(const char * name, Value* val) {
    vals.push_back(val);
    names.push_back(name);
  }


static const char* patterns[]={"led_test",
                               "blank",
                               "minimal_clock",
                               "amplitude_basic",
                               "amplitude_star",
                               "fft_spoke",
                               "fft_rotate"};

const char** PatternBase::get_patterns()
{
    return patterns;
}

int PatternBase::num_patterns()
{
    return COUNT_OF(patterns);
}

PatternBase * PatternBase::get_pattern(int idx) {
    switch(idx) {
        case 0: return new update_led_test();
        case 1: return new update_blank();
        case 2: return new update_minimal_clock();
        case 3: return new update_amplitude_basic();
        case 4: return new update_amplitude_star();
        case 5: return new update_fft_spoke();
        case 6: return new update_fft_rotate();
        default: return NULL;
    }
}