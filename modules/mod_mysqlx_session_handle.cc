/*
 * Copyright (c) 2014, 2016, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "utils/utils_sqlstring.h"
#include "mod_mysqlx_session_handle.h"
#include "mysqlxtest_utils.h"
#include "mysqlx_connection.h"
#include "utils/utils_general.h"
#include <boost/algorithm/string.hpp>

using namespace mysqlsh;
using namespace shcore;
using namespace mysqlsh::mysqlx;

void SessionHandle::open(const std::string &host, int port, const std::string &schema,
                          const std::string &user, const std::string &pass,
                          const std::string &ssl_ca, const std::string &ssl_cert,
                          const std::string &ssl_key, const std::size_t timeout,
                          const std::string &auth_method, const bool get_caps) {
  ::mysqlx::Ssl_config ssl;
  memset(&ssl, 0, sizeof(ssl));

  std::string my_ssl_ca(ssl_ca);
  std::string my_ssl_ca_path;
  shcore::normalize_sslca_args(my_ssl_ca, my_ssl_ca_path);

  ssl.ca = my_ssl_ca.c_str();
  ssl.cert = ssl_cert.c_str();
  ssl.key = ssl_key.c_str();
  ssl.ca_path = my_ssl_ca_path.c_str();

  // TODO: Define a proper timeout for the session creation
  _session = ::mysqlx::openSession(host, port, schema, user, pass, ssl, 10000, auth_method, true);
}

std::shared_ptr< ::mysqlx::Result> SessionHandle::execute_sql(const std::string &sql) const {
  std::shared_ptr< ::mysqlx::Result> ret_val;

  try {
    ret_val = _session->executeSql(sql);
    ret_val->wait();
  }
  CATCH_AND_TRANSLATE();

  return ret_val;
}

void SessionHandle::enable_protocol_trace(bool value) {
  _session->connection()->set_trace_protocol(value);
}

void SessionHandle::reset() {
  if (_session) {
    _session->close();

    _session.reset();
  }
}

std::shared_ptr< ::mysqlx::Result> SessionHandle::execute_statement(const std::string &domain, const std::string& command, const Argument_list &args) const {
  // Will return the result of the SQL execution
  // In case of error will be Undefined
  std::shared_ptr< ::mysqlx::Result> ret_val;

  if (!_session)
    throw Exception::logic_error("Not connected.");
  else {
    // Converts the arguments from shcore to mysqlxtest format
    std::vector< ::mysqlx::ArgumentValue> arguments;
    for (size_t index = 0; index < args.size(); index++)
      arguments.push_back(get_argument_value(args[index]));

    try {
      _last_result = _session->executeStmt(domain, command, arguments);

      // Calls wait so any error is properly triggered at execution time
      _last_result->wait();

      ret_val = _last_result;
    }
    CATCH_AND_TRANSLATE();
  }

  return ret_val;
}

/*
 * This function verifies if the given object exist in the database, works for schemas, tables, views and collections.
 * The check for tables, views and collections is done is done based on the type.
 * If type is not specified and an object with the name is found, the type will be returned.
 *
 * Returns the name of the object as exists in the database.
 */
std::string SessionHandle::db_object_exists(std::string &type, const std::string &name, const std::string& owner) const {
  std::string statement;
  std::string ret_val;

  std::shared_ptr< ::mysqlx::Result> res;
  std::shared_ptr< ::mysqlx::Row> raw_entry;
  if (type == "Schema") {
    res = execute_statement("sql", sqlstring("show databases like ?", 0) << name, Argument_list());
    raw_entry = res->next();

    if (raw_entry)
      ret_val = raw_entry->stringField(0);
  } else {
    shcore::Argument_list args;
    args.push_back(Value(owner));
    args.push_back(Value(name));

    res = execute_statement("xplugin", "list_objects", args);
    raw_entry = res->next();

    if (raw_entry) {
      std::string object_name;
      std::string object_type;

      std::shared_ptr<std::vector< ::mysqlx::ColumnMetadata> > metadata = res->columnMetadata();
      for (size_t index = 0; index < metadata->size(); ++index) {
        if (metadata->at(index).name == "name")
          object_name = raw_entry->stringField((int)index);

        if (metadata->at(index).name == "type")
          object_type = raw_entry->stringField((int)index);
      }

      if (type.empty()) {
        type = object_type;
        ret_val = object_name;
      } else {
        boost::algorithm::to_upper(type);

        if (type == object_type)
          ret_val = object_name;
      }
    }
  }

  res->flush();

  return ret_val;
}

::mysqlx::ArgumentValue SessionHandle::get_argument_value(shcore::Value source) const {
  ::mysqlx::ArgumentValue ret_val;
  switch (source.type) {
    case shcore::Bool:
      ret_val = ::mysqlx::ArgumentValue(source.as_bool());
      break;
    case shcore::UInteger:
      ret_val = ::mysqlx::ArgumentValue(source.as_uint());
      break;
    case shcore::Integer:
      ret_val = ::mysqlx::ArgumentValue(source.as_int());
      break;
    case shcore::String:
      ret_val = ::mysqlx::ArgumentValue(source.as_string());
      break;
    case shcore::Float:
      ret_val = ::mysqlx::ArgumentValue(source.as_double());
      break;
    case shcore::Object:
    case shcore::Null:
    case shcore::Array:
    case shcore::Map:
    case shcore::MapRef:
    case shcore::Function:
    case shcore::Undefined:
      std::stringstream str;
      str << "Unsupported value received: " << source.descr();
      throw shcore::Exception::argument_error(str.str());
      break;
  }

  return ret_val;
}

shcore::Value SessionHandle::get_capability(const std::string& name) {
  shcore::Value ret_val;

  if (_session) {
    const Mysqlx::Connection::Capabilities &caps(_session->connection()->capabilities());
    for (int c = caps.capabilities_size(), i = 0; i < c; i++) {
      if (caps.capabilities(i).name() == name) {
        const Mysqlx::Connection::Capability &cap(caps.capabilities(i));
        if (cap.value().type() == Mysqlx::Datatypes::Any::SCALAR &&
            cap.value().scalar().type() == Mysqlx::Datatypes::Scalar::V_STRING)
            ret_val = shcore::Value(cap.value().scalar().v_string().value());
        else if (cap.value().type() == Mysqlx::Datatypes::Any::SCALAR &&
                 cap.value().scalar().type() == Mysqlx::Datatypes::Scalar::V_OCTETS)
                 ret_val = shcore::Value(cap.value().scalar().v_octets().value());
      }
    }
  }

  return ret_val;
}

uint64_t SessionHandle::get_client_id() {
  if (_session) {
    return _session->connection()->client_id();
  } else
    return 0;
}
