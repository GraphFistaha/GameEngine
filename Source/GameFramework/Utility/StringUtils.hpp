#pragma once

#include <string>
#include <string_view>

namespace GameFramework::Utils
{

template<typename CharT>
constexpr std::basic_string_view<CharT> Trim(std::basic_string_view<CharT> text, std::basic_string_view<CharT> exclude = " \n\r\t\v\0")
{
  if (const size_t leftShift = text.find_first_not_of(exclude); leftShift != std::basic_string_view<CharT>::npos)
  {
    text.remove_prefix(leftShift);
  }
  else
  {
    return {};
  }

  if (const size_t rightShift = text.find_last_not_of(exclude);
      rightShift != std::basic_string_view<CharT>::npos)
  {
    text.remove_suffix(text.size() - rightShift - 1);
  }
  else
  {
    return {};
  }

  return text;
}

template<typename CharT>
constexpr std::vector<std::basic_string_view<CharT>> Split(std::basic_string_view<CharT> text, CharT delimiter)
{
  std::vector<std::basic_string_view<CharT>> result;
  size_t begin = 0, end = -1;
  do
  {
    begin = end + 1;
    end = text.find(delimiter, begin);
    if (end != std::basic_string_view<CharT>::npos)
      result.push_back(text.substr(begin, end - begin));
    else
      result.push_back(text.substr(begin, text.size() - begin));
  } while (end != std::basic_string_view<CharT>::npos);
  return result;
}

} // namespace GameFramework::utils
