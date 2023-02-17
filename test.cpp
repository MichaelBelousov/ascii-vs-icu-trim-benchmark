
#include <string>
#include <stdlib.h>
#include <locale>
#include <regex>
#include <iostream>
#include <exception>

#define U_CHARSET_IS_UTF8 1
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/bytestream.h>

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
    icu::UnicodeString us (s.data(), s.size(), US_INV);
    //auto us = icu::UnicodeString::fromUTF8(s.c_str());
    //s.clear();
    //us.toUTF8String(s);
    us.trim();
    //auto sink = icu::CheckedArrayByteSink(s.data(), s.capacity());
    //us.toUTF8(sink);
    // in theory if the utf8 is normalized trimming could increase the size of the string and break
    // out the buffer, no? Perhaps better to use StringByteSink
    //s.resize(sink.NumberOfBytesAppended());
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
static_assert(sizeof(EMSPC) === 4);

BENCHMARK(TrimTest, std::string&(*)(std::string&))
{
  SETUP_BENCHMARK(
    const auto trim_func = GET_ARGUMENT(0);
    std::vector<std::string> utf8_strings {
      "test" EMSPC,
      EMSPC "te" EMSPC "st" EMSPC,
      EMSPC "te" EMSPC "st" EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC,
      "      te      st                                                      ",
      "this is a debateably long string without any non-ascii characters",
    };
    std::vector<std::string> ascii_trimmed_strings {
      "test" EMSPC,
      EMSPC "te" EMSPC "st" EMSPC,
      EMSPC "te" EMSPC "st" EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC EMSPC,
      "te      st",
      "this is a debateably long string without any non-ascii characters",
    };
    std::vector<std::string> utf8_trimmed_strings {
      "test",
      "te" EMSPC "st",
      "te" EMSPC "st",
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
  //("BentleyAsciiTrim", BentleyAsciiTrim),
  //("BentleyLocaleTrim", BentleyLocaleTrim),
  ("IcuTrim", IcuTrim)
  //("RegexTrim", RegexTrim)
)

CRITERION_BENCHMARK_MAIN()

