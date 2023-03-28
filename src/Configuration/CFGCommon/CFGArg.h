#ifndef CFG_ARG_H
#define CFG_ARG_H

#include <string>
#include <vector>

#include "CFGCommon.h"

struct CFGArg_RULE {
  CFGArg_RULE(const std::string& n, char s, const std::string& t, bool o,
              bool m, const void* p, const char* h,
              const std::vector<std::string> e)
      : name(n),
        short_name(s),
        type(t),
        type_name(t == "int" ? "integer" : (t == "str" ? "string" : t)),
        optional(o),
        multiple(m),
        ptr(p),
        help(h),
        enums(e) {
    CFG_ASSERT(name.size());
    CFG_ASSERT(type == "flag" || type == "int" || type == "str" ||
               type == "enum");
    CFG_ASSERT(ptr != nullptr);
    if (type == "flag" || type == "enum") {
      CFG_ASSERT(!multiple);
    }
    CFG_ASSERT(help != nullptr);
    if (type == "enum") {
      CFG_ASSERT(enums.size());
    } else {
      CFG_ASSERT(enums.size() == 0);
    }
  }
  const std::string name;
  const char short_name;
  const std::string type;
  const std::string type_name;
  const bool optional;
  const bool multiple;
  const void* ptr;
  const char* help;
  const std::vector<std::string> enums;
  uint32_t count = 0;
};

class CFGArg {
 public:
  CFGArg(const std::string& n, int i, int a, std::vector<CFGArg_RULE> r,
         const char* h);
  bool specified(const std::string& option);
  std::vector<std::string> args;
  bool help = false;

 protected:
  bool parse(int argc, const char* argv[], std::vector<std::string>* errors);
  void print();

 private:
  void post_error(std::string message, std::vector<std::string>* errors);
  bool parse_long_option(const std::string& option, CFGArg_RULE** ptr,
                         std::vector<std::string>* errors);
  bool parse_short_option(const std::string& option, CFGArg_RULE** ptr,
                          std::vector<std::string>* errors);
  bool check_option_value(CFGArg_RULE* rule, const std::string& value,
                          bool is_short, CFGArg_RULE** ptr,
                          std::vector<std::string>* errors);
  bool assign(CFGArg_RULE* rule, const std::string& value,
              std::vector<std::string>* errors);
  bool print_option(const std::string& option,
                    std::vector<std::string>* errors);
  const std::string name = "";
  const int min_arg = 0;
  const int max_arg = 0;
  const std::vector<CFGArg_RULE> rules = {};
  const char* help_msg = nullptr;
  uint32_t count = 0;
};

#endif