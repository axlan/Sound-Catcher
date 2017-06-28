#pragma once

class Value
{
public:
  enum Type
  {
    NONE,
    INT,
    FLOAT,
    BOOL,
    STRING
  };

  Value();
  Value(int val);
  void set_int(int val) ;
  int get_int() const ;
  Value(float val);
  void set_float(float val) ;
  float get_float() const ;
  Value(bool val);
  void set_bool(bool val) ;
  bool get_bool() const ;
  Value(const char* val);
  void set_str(const char* val) ;
  const char* get_str() const;

  void set_val(const char* val);

  Type get_type() const;

private:
  Type type;
  union Data {
    int i;
    float f;
    bool b;
    const char* str;
  } data;  
};
