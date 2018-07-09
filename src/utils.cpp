#include "utils.hpp"

#include <regex>

#include <fmt/format.h>

#include <boost/algorithm/string.hpp>

#include <render_pipeline/rpcore/rpobject.hpp>

static std::vector<std::string> split(const std::string& src, const std::regex& regex)
{
    return { std::sregex_token_iterator(src.begin(), src.end(), regex, -1), std::sregex_token_iterator() };
}

LMatrix4f parse_to_mat4(const std::string& str, const LMatrix4f& fallback)
{
    auto floats = split(boost::algorithm::trim_copy(str), std::regex(" "));
    if (floats.size() != LMatrix4f::num_components)
    {
        rpcore::RPObject::global_error("World", fmt::format("mat4 Size is not 16: {}", str));
        return fallback;
    }

    LMatrix4f result;
    try
    {
        for (size_t k = 0; k < LMatrix4f::num_components; ++k)
            (&result(0, 0))[k] = std::stof(floats[k]);
    }
    catch (const std::exception& err)
    {
        rpcore::RPObject::global_error("World", fmt::format("Failed to parse mat4: ", str));
        rpcore::RPObject::global_error("World", fmt::format("Error: ", err.what()));
        return fallback;
    }

    return result;
}
