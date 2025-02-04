/*
 * Copyright (c) 2017, 2023, Oracle and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#pragma once

#include <array>

namespace keyword_diff_80 {

inline constexpr std::array<keyword_t, 142> added = {{
    {"ACTIVE", 0},
    {"ADMIN", 0},
    {"ARRAY", 0},
    {"ASSIGN_GTIDS_TO_ANONYMOUS_TRANSACTIONS", 0},
    {"ATTRIBUTE", 0},
    {"AUTHENTICATION", 0},
    {"BUCKETS", 0},
    {"BULK", 0},
    {"CHALLENGE_RESPONSE", 0},
    {"CLONE", 0},
    {"COMPONENT", 0},
    {"CUBE", 1},
    {"CUME_DIST", 1},
    {"DEFINITION", 0},
    {"DENSE_RANK", 1},
    {"DESCRIPTION", 0},
    {"EMPTY", 1},
    {"ENFORCED", 0},
    {"ENGINE_ATTRIBUTE", 0},
    {"EXCEPT", 1},
    {"EXCLUDE", 0},
    {"FACTOR", 0},
    {"FAILED_LOGIN_ATTEMPTS", 0},
    {"FINISH", 0},
    {"FIRST_VALUE", 1},
    {"FOLLOWING", 0},
    {"FUNCTION", 1},
    {"GENERATE", 0},
    {"GEOMCOLLECTION", 0},
    {"GET_MASTER_PUBLIC_KEY", 0},
    {"GET_SOURCE_PUBLIC_KEY", 0},
    {"GROUPING", 1},
    {"GROUPS", 1},
    {"GTID_ONLY", 0},
    {"HISTOGRAM", 0},
    {"HISTORY", 0},
    {"INACTIVE", 0},
    {"INITIAL", 0},
    {"INITIATE", 0},
    {"INTERSECT", 1},
    {"INVISIBLE", 0},
    {"JSON_TABLE", 1},
    {"JSON_VALUE", 0},
    {"KEYRING", 0},
    {"LAG", 1},
    {"LAST_VALUE", 1},
    {"LATERAL", 1},
    {"LEAD", 1},
    {"LOCKED", 0},
    {"MASTER_COMPRESSION_ALGORITHMS", 0},
    {"MASTER_PUBLIC_KEY_PATH", 0},
    {"MASTER_TLS_CIPHERSUITES", 0},
    {"MASTER_ZSTD_COMPRESSION_LEVEL", 0},
    {"MEMBER", 0},
    {"NESTED", 0},
    {"NETWORK_NAMESPACE", 0},
    {"NOWAIT", 0},
    {"NTH_VALUE", 1},
    {"NTILE", 1},
    {"NULLS", 0},
    {"OF", 1},
    {"OFF", 0},
    {"OJ", 0},
    {"OLD", 0},
    {"OPTIONAL", 0},
    {"ORDINALITY", 0},
    {"ORGANIZATION", 0},
    {"OTHERS", 0},
    {"OVER", 1},
    {"PASSWORD_LOCK_TIME", 0},
    {"PATH", 0},
    {"PERCENT_RANK", 1},
    {"PERSIST", 0},
    {"PERSIST_ONLY", 0},
    {"PRECEDING", 0},
    {"PRIVILEGE_CHECKS_USER", 0},
    {"PROCESS", 0},
    {"RANDOM", 0},
    {"RANK", 1},
    {"RECURSIVE", 1},
    {"REFERENCE", 0},
    {"REGISTRATION", 0},
    {"REPLICA", 0},
    {"REPLICAS", 0},
    {"REQUIRE_ROW_FORMAT", 0},
    {"REQUIRE_TABLE_PRIMARY_KEY_CHECK", 0},
    {"RESOURCE", 0},
    {"RESPECT", 0},
    {"RESTART", 0},
    {"RETAIN", 0},
    {"RETURNING", 0},
    {"REUSE", 0},
    {"ROLE", 0},
    {"ROW", 1},
    {"ROWS", 1},
    {"ROW_NUMBER", 1},
    {"SECONDARY", 0},
    {"SECONDARY_ENGINE", 0},
    {"SECONDARY_ENGINE_ATTRIBUTE", 0},
    {"SECONDARY_LOAD", 0},
    {"SECONDARY_UNLOAD", 0},
    {"SKIP", 0},
    {"SOURCE_AUTO_POSITION", 0},
    {"SOURCE_BIND", 0},
    {"SOURCE_COMPRESSION_ALGORITHMS", 0},
    {"SOURCE_CONNECTION_AUTO_FAILOVER", 0},
    {"SOURCE_CONNECT_RETRY", 0},
    {"SOURCE_DELAY", 0},
    {"SOURCE_HEARTBEAT_PERIOD", 0},
    {"SOURCE_HOST", 0},
    {"SOURCE_LOG_FILE", 0},
    {"SOURCE_LOG_POS", 0},
    {"SOURCE_PASSWORD", 0},
    {"SOURCE_PORT", 0},
    {"SOURCE_PUBLIC_KEY_PATH", 0},
    {"SOURCE_RETRY_COUNT", 0},
    {"SOURCE_SSL", 0},
    {"SOURCE_SSL_CA", 0},
    {"SOURCE_SSL_CAPATH", 0},
    {"SOURCE_SSL_CERT", 0},
    {"SOURCE_SSL_CIPHER", 0},
    {"SOURCE_SSL_CRL", 0},
    {"SOURCE_SSL_CRLPATH", 0},
    {"SOURCE_SSL_KEY", 0},
    {"SOURCE_SSL_VERIFY_SERVER_CERT", 0},
    {"SOURCE_TLS_CIPHERSUITES", 0},
    {"SOURCE_TLS_VERSION", 0},
    {"SOURCE_USER", 0},
    {"SOURCE_ZSTD_COMPRESSION_LEVEL", 0},
    {"SRID", 0},
    {"STREAM", 0},
    {"SYSTEM", 1},
    {"THREAD_PRIORITY", 0},
    {"TIES", 0},
    {"TLS", 0},
    {"UNBOUNDED", 0},
    {"UNREGISTER", 0},
    {"URL", 0},
    {"VCPU", 0},
    {"VISIBLE", 0},
    {"WINDOW", 1},
    {"ZONE", 0},
}};

inline constexpr std::array<keyword_t, 13> removed = {{
    {"ANALYSE", 0},
    {"CUBE", 0},
    {"DES_KEY_FILE", 0},
    {"FUNCTION", 0},
    {"MASTER_SERVER_ID", 0},
    {"MAX_STATEMENT_TIME", 0},
    {"NONBLOCKING", 0},
    {"OLD_PASSWORD", 0},
    {"PARSE_GCOL_EXPR", 0},
    {"REDOFILE", 0},
    {"ROW", 0},
    {"ROWS", 0},
    {"SQL_CACHE", 0},
}};

}  // namespace keyword_diff_80
