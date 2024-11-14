#pragma once

#include <memory>
#define BAlloca(_type_, _elements_ ) (_type_*)alloca(sizeof(_type_)*_elements_);
