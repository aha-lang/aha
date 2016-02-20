#pragma once

#include <stdio.h>

#include <algorithm>
#include <unordered_map>
#include <memory>

#define BOOST_LOCALE_ENABLE_CHAR32_T
#define BOOST_LOCALE_ENABLE_CHAR16_T
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/functional/hash.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>
