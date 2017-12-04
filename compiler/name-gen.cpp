#include "compiler/name-gen.h"

#include "compiler/data.h"
#include "compiler/io.h"
#include "compiler/stage.h"

string register_unique_name (const string &prefix) {
  //static set <string> v;
  //static volatile int x = 0;
  //AutoLocker <volatile int *> locker (&x);
  //if (v.count (prefix)) {
    //fprintf (stderr, "%s\n", prefix.c_str());
    //assert (0);
  //}
  //v.insert (prefix);
  return prefix;
}

string gen_shorthand_ternary_name () {
  AUTO_PROF (next_name);
  static volatile int x = 0;
  AutoLocker <volatile int *> locker (&x);
  SrcFilePtr file = stage::get_file();
  unsigned long long h = hash_ll (file->unified_file_name);
  static map<unsigned long long, int> name_map;
  int *i = &(name_map[h]);
  int cur_i = (*i)++;
  char tmp[50];
  sprintf (tmp, "%llx_%d", h, cur_i);
  return string("shorthand_ternary_cond$ut") + tmp;
}

string gen_anonymous_function_name () {
  AUTO_PROF (next_name);
  static volatile int x = 0;
  AutoLocker <volatile int *> locker (&x);
  SrcFilePtr file = stage::get_file();
  unsigned long long h = hash_ll (file->unified_file_name);
  static map<unsigned long long, int> name_map;
  int *i = &(name_map[h]);
  int cur_i = (*i)++;
  char tmp[50];
  sprintf (tmp, "%llx_%d", h, cur_i);
  return string("anonymous_function$ut") + tmp;
}



string gen_const_string_name (const string &str) {
  AUTO_PROF (next_const_string_name);
  unsigned long long h = hash_ll (str);
  char tmp[50];
  sprintf (tmp, "const_string$us%llx", h);
  return tmp;
}

string gen_const_regexp_name (const string &str) {
  AUTO_PROF (next_const_string_name);
  unsigned long long h = hash_ll (str);
  char tmp[50];
  sprintf (tmp, "const_regexp$us%llx", h);
  return tmp;
}

string gen_unique_name (string prefix, bool flag) {
  for (int i = 0; i < (int)prefix.size(); i++) {
    int c = prefix[i];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
    } else {
      prefix[i] = '_';
    }
  }
  prefix += "$u";
  AUTO_PROF (next_name);
  FunctionPtr function = stage::get_function();
  if (function.is_null() || flag) {
    return register_unique_name (prefix);
  }
  map <long long, int> *name_gen_map = &function->name_gen_map;
  int h = hash (function->name);
  long long ph = hash_ll (prefix);
  int *i = &(*name_gen_map)[ph];
  int cur_i = (*i)++;
  char tmp[50];
  sprintf (tmp, "%x_%d", h, cur_i);
  return register_unique_name (prefix + tmp);
}

inline string resolve_uses(FunctionPtr current_function, string class_name, char delim) {
  if (class_name[0] != '\\') {
    if (class_name == "static" || class_name == "self" || class_name == "parent") {
      kphp_error(!current_function->namespace_name.empty(),
                 "parent::<func_name>, static::<func_name> or self::<func_name> can be used only inside class");
      if (class_name == "parent") {
        kphp_assert(!current_function->class_extends.empty());
        class_name = resolve_uses(current_function, current_function->class_extends, delim);
      } else if (class_name == "static") {
        class_name = current_function->class_context_name;
      } else {
        class_name = current_function->namespace_name + "\\" + current_function->class_name;
      }
    } else {
      size_t slash_pos = class_name.find('\\');
      if (slash_pos == string::npos) {
        slash_pos = class_name.length();
      }
      string class_name_start = class_name.substr(0, slash_pos);
      map<string, string> const &uses = current_function->namespace_uses;
      bool use_used = false;
      for (map<string, string>::const_iterator it = uses.begin(); it != uses.end(); ++it) {
        if (class_name_start == it->first) {
          class_name = it->second + class_name.substr(class_name_start.length());
          use_used = true;
          break;
        }
      }
      if (!use_used) {
        class_name = current_function->namespace_name + "\\" + class_name;
      }
    }
  }
  if (class_name[0] == '\\') {
    class_name = class_name.substr(1);
  }
  for (size_t i = 0; i < class_name.size(); i++) {
    if (class_name[i] == '\\') {
      class_name[i] = delim;
    }
  }
  return class_name;
}


string get_context_by_prefix(FunctionPtr function, string const &class_name, char delim) {
  if (class_name == "static" || class_name == "self" || class_name == "parent") {
    return resolve_uses(function, "\\" + function->class_context_name, delim);
  }
  return resolve_uses(function, class_name, delim);
}

string get_full_static_member_name(FunctionPtr function, string const &name, bool append_with_context) {
  size_t pos$$ = name.find("::");
  if (pos$$ != string::npos) {
    string prefix_name = name.substr(0, pos$$);
    string class_name = resolve_uses(function, prefix_name, '$');
    string fun_name = name.substr(pos$$ + 2);
    string new_name = class_name + "$$" + fun_name;
    if (append_with_context) {
      string context_name = get_context_by_prefix(function, prefix_name);
      if (context_name != class_name) {
        new_name += "$$" + context_name;
      }
    }
    return new_name;
  } else {
    return name;
  }
}
