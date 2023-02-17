
#include <string>
#include <stdlib.h>
#include <unicode/unistr.h>
#include <regex>
#include <iostream>
#include <exception>

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

std::string& IcuTrim (std::string& s)
    {
    auto us = icu::UnicodeString::fromUTF8(s.c_str());
    us.trim();
    us.toUTF8String(s);
    return s;
    }

std::regex unicode_spaces{"[\r\n\t\f\v \u00a0\u1680\u2000-\u200a\u2028\u2029\u202f\u205f\u3000\ufeff]"};

std::string& RegexTrim (std::string& s)
    {
    std::string out{};
    std::regex_replace(std::back_inserter(out), s.begin(), s.end(), unicode_spaces, "");
    s = out;
    return s;
    }

BENCHMARK(TrimTest, std::string&(*)(std::string&))
{
  SETUP_BENCHMARK(
    const auto trim_func = GET_ARGUMENT(0);
    std::vector<std::string> utf8_strings {
      "test\u00a0",
      "\u00a0te\u00a0st\u00a0",
      "\u00a0te\u00a0st\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0",
      "      te      st                                                      ",
      "this is an debateably long string without any non-ascii characters",
    };
    std::vector<std::string> ascii_trimmed_strings {
      "test\u00a0",
      "\u00a0te\u00a0st\u00a0",
      "\u00a0te\u00a0st\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0\u00a0",
      "te      st",
      "this is an debateably long string without any non-ascii characters",
    };
    std::vector<std::string> utf8_trimmed_strings {
      "test",
      "te\u00a0st",
      "te\u00a0st",
      "te      st",
      "this is an debateably long string without any non-ascii characters",
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
  ("IcuTrim", IcuTrim),
  ("RegexTrim", RegexTrim)
)

CRITERION_BENCHMARK_MAIN()

