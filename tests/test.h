/*
  +--------------------------------------------------------------------------+
  | libcat                                                                   |
  +--------------------------------------------------------------------------+
  | Licensed under the Apache License, Version 2.0 (the "License");          |
  | you may not use this file except in compliance with the License.         |
  | You may obtain a copy of the License at                                  |
  | http://www.apache.org/licenses/LICENSE-2.0                               |
  | Unless required by applicable law or agreed to in writing, software      |
  | distributed under the License is distributed on an "AS IS" BASIS,        |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
  | See the License for the specific language governing permissions and      |
  | limitations under the License. See accompanying LICENSE file.            |
  +--------------------------------------------------------------------------+
  | Author: Twosee <twose@qq.com>                                            |
  | Author: codinghuang <2812240764@qq.com>                                  |
  +--------------------------------------------------------------------------+
 */

#include <gtest/gtest.h>

#include <memory>

#include "cat_api.h"

/* common macros */

#define SKIP_IF(expression) do { if (expression) { GTEST_SKIP(); return; } } while (0)

#define TEST_BUFFER_SIZE_STD               8192

#define TEST_IO_TIMEOUT                    ::testing::CONFIG_IO_TIMEOUT

#define TEST_MAX_REQUEST                   ::testing::CONFIG_MAX_REQUEST
#define TEST_MAX_CONCURRENCY               ::testing::CONFIG_MAX_CONCURRENCY

#define TEST_LISTEN_HOST                   "localhost"
#define TEST_LISTEN_IPV4                   "127.0.0.1"
#define TEST_LISTEN_IPV6                   "::"
#ifndef CAT_OS_WIN
#define TEST_PIPE_PATH                     "/tmp/cat_test.sock"
#else
#define TEST_PIPE_PATH                     "\\\\?\\pipe\\cat_test"
#endif
#define TEST_SERVER_BACKLOG                8192

#define TEST_REMOTE_HTTP_SERVER            CAT_STRL(TEST_REMOTE_HTTP_SERVER_HOST), TEST_REMOTE_HTTP_SERVER_PORT
#define TEST_REMOTE_HTTP_SERVER_HOST       "www.microsoft.com"
#define TEST_REMOTE_HTTP_SERVER_PORT       80

#define TEST_REMOTE_HTTPS_SERVER           CAT_STRL(TEST_REMOTE_HTTPS_SERVER_HOST), TEST_REMOTE_HTTPS_SERVER_PORT
#define TEST_REMOTE_HTTPS_SERVER_HOST      TEST_REMOTE_HTTP_SERVER_HOST
#define TEST_REMOTE_HTTPS_SERVER_PORT      443

#define TEST_REMOTE_IPV6_HTTP_SERVER_HOST  "www.google.com"

#define TEST_HTTP_STATUS_CODE_OK           200
#define TEST_HTTP_CLIENT_FAKE_USERAGENT    "curl/7.58.0"

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res
#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)

#define DEFER(X) std::shared_ptr<void> UNIQUE_NAME(defer)(nullptr, [&](...){ X; })

/* common functions */

static inline bool env_is(const char *name, const char *value)
{
    char *env = getenv(name);
    return env && strcmp(env, value) == 0;
}

static inline bool is_valgrind(void)
{
    return env_is("USE_VALGRIND", "1");
}

static inline bool is_offline(void)
{
    return env_is("OFFLINE", "1");
}

/* common vars */

namespace testing
{
    extern cat_timeout_t CONFIG_IO_TIMEOUT;
    extern uint32_t CONFIG_MAX_REQUEST;
    extern uint32_t CONFIG_MAX_CONCURRENCY;
}
