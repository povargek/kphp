#pragma once

#include <string>

#include "common/tlo-parsing/tlo-parsing-tools.h"

#include "compiler/data/data_ptr.h"

class CodeGenerator;

namespace tl_gen {

void write_tl_query_handlers(CodeGenerator &W);

std::string cpp_tl_struct_name(const char *prefix, std::string tl_name, const std::string &template_args_postfix = "");

vk::tl::type *get_tl_type_of_php_class(ClassPtr klass);
vk::tl::combinator *get_tl_constructor_of_php_class(ClassPtr klass);
ClassPtr get_php_class_of_tl_constructor(const vk::tl::combinator *c, const std::string &template_params_suf = "");
ClassPtr get_php_class_of_tl_function(vk::tl::combinator *f);
std::string get_tl_function_of_php_class(ClassPtr klass);

bool is_php_class_a_tl_function(ClassPtr klass);
bool is_php_class_a_tl_constructor(ClassPtr klass);
bool is_php_class_a_tl_polymorphic_type(ClassPtr klass);
bool is_php_class_a_tl_array_item(ClassPtr klass);
bool is_tl_type_a_php_array(vk::tl::type *t);

bool is_type_dependent(vk::tl::type *type, const vk::tl::tl_scheme *);
bool is_type_dependent(vk::tl::combinator *constructor, const vk::tl::tl_scheme *);
} // namespace tl_gen
