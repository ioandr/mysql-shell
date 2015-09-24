/*
* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
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

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "base_session.h"

namespace shcore {
  class Shell_js_mysqlx_schema_tests : public Shell_core_test_wrapper
  {
  protected:
    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp()
    {
      Shell_core_test_wrapper::SetUp();

      bool initilaized(false);
      _shell_core->switch_mode(Shell_core::Mode_JScript, initilaized);

      exec_and_out_equals("var mysqlx = require('mysqlx').mysqlx;");

      exec_and_out_equals("var session = mysqlx.getNodeSession('" + _uri + "');");

      exec_and_out_equals("session.sql('drop schema if exists js_shell_test;').execute();");
      exec_and_out_equals("session.createSchema('js_shell_test');");
      exec_and_out_equals("session.setCurrentSchema('js_shell_test');");
      exec_and_out_equals("var result = session.sql('create table table1 (name varchar(50));').execute();");
      exec_and_out_equals("session.sql('create view view1 (my_name) as select name from table1;').execute();");

      exec_and_out_equals("session.js_shell_test.createCollection('collection1')");
    }
  };

  // Tests schema.getName()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_name)
  {
    exec_and_out_equals("print(session.mysql.getName());", "mysql");
  }

  // Tests schema.name
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_name)
  {
    exec_and_out_equals("print(session.mysql.name);", "mysql");
  }

  // Tests schema.getSession()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_session)
  {
    std::string uri = mysh::strip_password(_uri);

    exec_and_out_equals("var schema = session.mysql;");

    exec_and_out_equals("var schema_session = schema.getSession();");

    exec_and_out_equals("print(schema_session)", "<NodeSession:" + uri + ">");
  }

  // Tests schema.session
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_session)
  {
    std::string uri = mysh::strip_password(_uri);

    exec_and_out_equals("var schema = session.mysql;");

    exec_and_out_equals("print(schema.session)", "<NodeSession:" + uri + ">");
  }

  // Tests schema.getSchema()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_schema)
  {
    std::string uri = mysh::strip_password(_uri);

    exec_and_out_equals("var schema = session.mysql;");

    exec_and_out_equals("var schema_schema = schema.getSchema();");

    exec_and_out_equals("print(schema_schema)", "null");
  }

  // Tests schema.schema
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_schema)
  {
    std::string uri = mysh::strip_password(_uri);

    exec_and_out_equals("var schema = session.mysql;");

    exec_and_out_equals("print(schema.schema)", "null");
  }

  // Tests schema.getTables()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_tables)
  {
    exec_and_out_equals("var tables = session.js_shell_test.getTables();");

    exec_and_out_equals("print(tables.table1)", "<Table:table1>");
  }

  // Tests schema.tables
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_tables)
  {
    exec_and_out_equals("print(session.js_shell_test.tables.table1)", "<Table:table1>");
  }

  // Tests schema.getViews()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_views)
  {
    exec_and_out_equals("var views = session.js_shell_test.getViews();");

    exec_and_out_equals("print(views.view1)", "<View:view1>");
  }

  // Tests schema.views
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_views)
  {
    exec_and_out_equals("print(session.js_shell_test.views.view1)", "<View:view1>");
  }

  // Tests schema.getCollections()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_collections)
  {
    exec_and_out_equals("var collections = session.js_shell_test.getCollections();");

    exec_and_out_equals("print(collections.collection1)", "<Collection:collection1>");
  }

  // Tests schema.collections
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_collections)
  {
    exec_and_out_equals("print(session.js_shell_test.collections.collection1)", "<Collection:collection1>");
  }

  // Tests schema.getTable()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_table)
  {
    exec_and_out_equals("var table = session.js_shell_test.getTable('table1');");

    exec_and_out_equals("print(table)", "<Table:table1>");
  }

  // Tests schema.getView()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_view)
  {
    exec_and_out_equals("var view = session.js_shell_test.getView('view1');");

    exec_and_out_equals("print(view)", "<View:view1>");
  }

  // Tests schema.getCollection()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_collection)
  {
    exec_and_out_equals("var collection = session.js_shell_test.getCollection('collection1');");

    exec_and_out_equals("print(collection)", "<Collection:collection1>");
  }

  // Tests schema.getCollectionAsTable()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_get_collection_as_table)
  {
    exec_and_out_equals("var table = session.js_shell_test.getCollectionAsTable('collection1')");

    exec_and_out_equals("print(table)", "<Table:collection1>");
  }

  // Tests schema.<object>
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_object)
  {
    exec_and_out_equals("print(session.js_shell_test.table1)", "<Table:table1>");
    exec_and_out_equals("print(session.js_shell_test.view1)", "<View:view1>");
    exec_and_out_equals("print(session.js_shell_test.collection1)", "<Collection:collection1>");
  }

  // Tests schema.createCollection()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_create_collection)
  {
    exec_and_out_equals("var collection = session.js_shell_test.createCollection('test_collection');");

    exec_and_out_equals("print(collection)", "<Collection:test_collection>");
  }

  // Tests schema.drop() and schema.existInDatabase()
  TEST_F(Shell_js_mysqlx_schema_tests, mysqlx_schema_drop_exist_in_database)
  {
    exec_and_out_equals("var schema = session.createSchema('my_sample_schema');");

    exec_and_out_equals("print(schema.existInDatabase());", "true");

    exec_and_out_equals("schema.drop();");

    exec_and_out_equals("print(schema.existInDatabase());", "false");

    exec_and_out_equals("session.close();");
  }
}