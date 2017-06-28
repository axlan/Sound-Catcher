
#include "Value.h"
#include <string.h>
#include <stdlib.h>

Value::Value():type(NONE){}
Value::Value(int val):type(INT){data.i = val;}
void Value::set_int(int val) { data.i = val;}
int Value::get_int() const { return data.i;}
Value::Value(float val):type(FLOAT){data.f = val;}
void Value::set_float(float val) { data.f = val;}
float Value::get_float() const { return data.f;}
Value::Value(bool val):type(BOOL){data.b = val;}
void Value::set_bool(bool val) { data.b = val;}
bool Value::get_bool() const { return data.b;}
Value::Value(const char* val):type(STRING){data.str = val;}
void Value::set_str(const char* val) { data.str = val;}
const char* Value::get_str() const { return data.str;}

void Value::set_val(const char* val) {
  switch(type) {
    case NONE: break;
    case INT: data.i = atoi(val); break;
    case BOOL: data.b = atoi(val); break;
    case FLOAT: data.f = atof(val); break;
    case STRING: data.str = val; break;
  }
}

Value::Type Value::get_type() const { return type;}
