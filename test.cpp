
#include <string>
#include <stdlib.h>
#include <locale>
#include <regex>
#include <iostream>
#include <exception>

#define U_CHARSET_IS_UTF8 1
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/utypes.h>
#include <unicode/utf8.h>
#include <unicode/uchar.h>

#include "./criterion.hpp"

std::string& BentleyAsciiTrim (std::string& s)
    {
    if (s.empty ())
        return s;

    size_t firstNonSpaceIdx = 0;
    for (; (firstNonSpaceIdx < s.length ()) && isspace (s.at (firstNonSpaceIdx)); ++firstNonSpaceIdx)
        ;

    size_t lastNonSpaceIdx = s.length () - 1;
    for (; (lastNonSpaceIdx > firstNonSpaceIdx) && isspace (s.at (lastNonSpaceIdx)); --lastNonSpaceIdx)
        ;

    s.erase ((s.begin () + lastNonSpaceIdx + 1), s.end ());
    s.erase (s.begin (), (s.begin () + firstNonSpaceIdx));
    return s;
    }

// doesn't work, since this is a utf8 string, need to iterate over it by code points
std::string& BentleyLocaleTrim (std::string& s)
    {
    if (s.empty ())
        return s;

    size_t firstNonSpaceIdx = 0;
    for (; (firstNonSpaceIdx < s.length ()) && isspace (s.at (firstNonSpaceIdx), std::locale("en_US.utf8")); ++firstNonSpaceIdx)
        ;

    size_t lastNonSpaceIdx = s.length () - 1;
    for (; (lastNonSpaceIdx > firstNonSpaceIdx) && isspace (s.at (lastNonSpaceIdx), std::locale("en_US.utf8")); --lastNonSpaceIdx)
        ;

    s.erase ((s.begin () + lastNonSpaceIdx + 1), s.end ());
    s.erase (s.begin (), (s.begin () + firstNonSpaceIdx));
    return s;
    }

std::string& IcuTrim (std::string& s)
    {
    if (s.empty ())
        return s;

    size_t firstNonSpaceIdx = 0;
    int32_t len = s.size();
    const char* start = s.data();
    for (auto i = 0; i < len;) {
      UChar32 c;
      const auto prev_i = i;
      U8_NEXT(start, i, len, c);
      const auto incSize = i - prev_i;
      if (u_isUWhiteSpace(c)) {
      //if (u_isspace(c)) {
        firstNonSpaceIdx += incSize;
      } else {
        break;
      }
    }

    size_t lastNonSpaceIdx = len - 1;
    for (auto i = len; i > firstNonSpaceIdx;) {
      UChar32 c;
      const auto prev_i = i;
      U8_PREV(start, 0, i, c);
      const auto decSize = prev_i - i;
      //if (u_isspace(c)) {
      if (u_isUWhiteSpace(c)) {
        lastNonSpaceIdx -= decSize;
      } else {
        break;
      }
    }

    s.erase ((s.begin () + lastNonSpaceIdx + 1), s.end ());
    s.erase (s.begin (), (s.begin () + firstNonSpaceIdx));
    return s;
    }

/*
std::regex unicode_spaces{"[\r\n\t\f\v \u00a0\u1680\u2000-\u200a\u2028\u2029\u202f\u205f\u3000\ufeff]"};

std::string& RegexTrim (std::string& s)
    {
    std::string out{};
    std::regex_replace(std::back_inserter(out), s.begin(), s.end(), unicode_spaces, "");
    s = out;
    return s;
    }
*/

#define NBSP "\u00a0"
#define EMSPC "\u2003"
static_assert(sizeof(EMSPC) - 1 == 3, "emspace char size was not 3");
#define TESTSPC EMSPC

BENCHMARK(TrimTest, std::string&(*)(std::string&))
{
  SETUP_BENCHMARK(
    const auto trim_func = GET_ARGUMENT(0);
    std::vector<std::string> utf8_strings {
      "test" TESTSPC,
      TESTSPC "te" TESTSPC "st" TESTSPC,
      TESTSPC "te" TESTSPC "st" TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC,
      "      te      st                                                      ",
      "this is a debateably long string without any non-ascii characters",
    };
    std::vector<std::string> ascii_trimmed_strings {
      "test" TESTSPC,
      TESTSPC "te" TESTSPC "st" TESTSPC,
      TESTSPC "te" TESTSPC "st" TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC TESTSPC,
      "te      st",
      "this is a debateably long string without any non-ascii characters",
    };
    std::vector<std::string> utf8_trimmed_strings {
      "test",
      "te" TESTSPC "st",
      "te" TESTSPC "st",
      "te      st",
      "this is a debateably long string without any non-ascii characters",
    };
  )

  for (auto& s : utf8_strings) {
    trim_func(s);
  }

  TEARDOWN_BENCHMARK(
    for (auto i = 0; i < utf8_trimmed_strings.size(); ++i) {
      const auto trimmed_string = (trim_func == BentleyAsciiTrim ? ascii_trimmed_strings : utf8_trimmed_strings)[i];
      if (utf8_strings[i] != trimmed_string) {
        std::cerr << "string " << i << " was not trimmed correctly, should be '" << trimmed_string << "' but was '" << utf8_strings[i] << "'" << std::endl;
        throw std::runtime_error("bad trimmed string");
      }
    }
  )
}

INVOKE_BENCHMARK_FOR_EACH(TrimTest,
  ("BentleyAsciiTrim", BentleyAsciiTrim),
  ("IcuTrim", IcuTrim)
  //("BentleyLocaleTrim", BentleyLocaleTrim),
  //("RegexTrim", RegexTrim)
)

CRITERION_BENCHMARK_MAIN()

