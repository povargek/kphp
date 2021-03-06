// Compiler for PHP (aka KPHP)
// Copyright (c) 2020 LLC «V Kontakte»
// Distributed under the GPL v3 License, see LICENSE.notice.txt

#include "compiler/data/define-data.h"

DefineData::DefineData() :
  id(),
  type_(def_unknown),
  val(nullptr) {}

DefineData::DefineData(std::string name, VertexPtr val, DefineType type_) :
  id(),
  type_(type_),
  val(val),
  name(std::move(name)) {}
