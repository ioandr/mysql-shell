/*
 * Copyright (c) 2019, 2020, Oracle and/or its affiliates. All rights reserved.
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

#ifndef MYSQLSHDK_LIBS_STORAGE_IFILE_H_
#define MYSQLSHDK_LIBS_STORAGE_IFILE_H_

#include <memory>
#include <string>
#include <unordered_map>

#if defined(_WIN32)
using off64_t = __int64;
using ssize_t = __int64;
#elif defined(__APPLE__)
using off64_t = off_t;
#endif

namespace mysqlshdk {
namespace oci {
struct Oci_options;
}
namespace storage {

enum class Mode { READ, WRITE, APPEND };

class IFile {
 public:
  IFile() = default;
  IFile(const IFile &other) = delete;
  IFile(IFile &&other) = default;

  IFile &operator=(const IFile &other) = delete;
  IFile &operator=(IFile &&other) = default;

  virtual ~IFile() = default;

  virtual void open(Mode m) = 0;
  virtual bool is_open() const = 0;
  virtual int error() const = 0;
  virtual void close() = 0;

  virtual size_t file_size() const = 0;
  virtual std::string full_path() const = 0;
  virtual std::string filename() const = 0;
  virtual bool exists() const = 0;

  virtual off64_t seek(off64_t offset) = 0;
  virtual off64_t tell() const = 0;
  virtual ssize_t read(void *buffer, size_t length) = 0;
  virtual ssize_t write(const void *buffer, size_t length) = 0;
  virtual bool flush() = 0;

  /**
   * Changes name of this file. File cannot be moved to another directory.
   *
   * @param new_name New name of the file.
   */
  virtual void rename(const std::string &new_name) = 0;
};

std::unique_ptr<IFile> make_file(
    const std::string &filepath,
    const std::unordered_map<std::string, std::string> &options = {});

std::unique_ptr<IFile> make_file(const std::string &filepath,
                                 const mysqlshdk::oci::Oci_options &options);

int fprintf(IFile *, const char *format, ...);
int fputs(const char *s, IFile *file);
int fputs(const std::string &s, IFile *file);

}  // namespace storage
}  // namespace mysqlshdk

#endif  // MYSQLSHDK_LIBS_STORAGE_IFILE_H_
