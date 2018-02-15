/*
 * Copyright (c) 2017, 2018, Oracle and/or its affiliates. All rights reserved.
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

#include "src/mysqlsh/shell_console.h"
#include <string>

#include "mysqlshdk/libs/utils/logger.h"
#include "mysqlshdk/libs/utils/utils_string.h"

namespace mysqlsh {

Shell_console::Shell_console(shcore::Interpreter_delegate *deleg)
    : m_ideleg(deleg) {}

void Shell_console::print(const std::string &text) {
  m_ideleg->print(m_ideleg->user_data, text.c_str());
  log_debug("%s", text.c_str());
}

void Shell_console::println(const std::string &text) {
  m_ideleg->print(m_ideleg->user_data, (text + "\n").c_str());
  log_debug("%s", text.c_str());
}

void Shell_console::print_error(const std::string &text) {
  m_ideleg->print(m_ideleg->user_data, "ERROR: ");
  m_ideleg->print(m_ideleg->user_data, (text + "\n").c_str());
  log_error("%s", text.c_str());
}

void Shell_console::print_warning(const std::string &text) {
  m_ideleg->print(m_ideleg->user_data, "WARNING: ");
  m_ideleg->print(m_ideleg->user_data, (text + "\n").c_str());
  log_warning("%s", text.c_str());
}

void Shell_console::print_info(const std::string &text) {
  m_ideleg->print(m_ideleg->user_data, "INFO: ");
  m_ideleg->print(m_ideleg->user_data, (text + "\n").c_str());
  log_info("%s", text.c_str());
}

bool Shell_console::prompt(const std::string &prompt, std::string *ret_val) {
  if (m_ideleg->prompt(m_ideleg->user_data, prompt.c_str(), ret_val) ==
      shcore::Prompt_result::Ok)
    return true;
  return false;
}

static int process_label(const std::string &s, std::string *out_display,
                         std::string *out_clean_text) {
  out_display->clear();
  if (s.empty()) return 0;

  int letter = 0;
  char prev = 0;
  for (char c : s) {
    if (prev == '&') letter = c;
    if (c != '&') {
      if (prev == '&') {
        out_display->push_back('[');
        out_display->push_back(c);
        out_display->push_back(']');
      } else {
        out_display->push_back(c);
      }
      out_clean_text->push_back(c);
    }
    prev = c;
  }
  return letter;
}

Prompt_answer Shell_console::confirm(const std::string &prompt,
                                     Prompt_answer def,
                                     const std::string &yes_label,
                                     const std::string &no_label,
                                     const std::string &alt_label) {
  assert(def != Prompt_answer::ALT || !alt_label.empty());

  Prompt_answer final_ans = Prompt_answer::NONE;
  std::string ans;
  int yes_letter = 0;
  int no_letter = 0;
  int alt_letter = 0;
  std::string def_str;
  std::string clean_yes_text, clean_no_text, clean_alt_text;
  if (yes_label == "&Yes" && no_label == "&No" && alt_label.empty()) {
    std::string display_text;
    yes_letter = process_label(yes_label, &display_text, &clean_yes_text);
    no_letter = process_label(no_label, &display_text, &clean_no_text);

    if (def == Prompt_answer::YES)
      def_str = "[Y/n]";
    else if (def == Prompt_answer::NO)
      def_str = "[y/N]";
    else
      def_str = "[y/n]";
  } else {
    std::string display_text;
    yes_letter = process_label(yes_label, &display_text, &clean_yes_text);
    if (!display_text.empty()) def_str.append(display_text).append("/");

    no_letter = process_label(no_label, &display_text, &clean_no_text);
    if (!display_text.empty()) def_str.append(display_text).append("/");

    alt_letter = process_label(alt_label, &display_text, &clean_alt_text);
    if (!display_text.empty()) def_str.append(display_text).append("/");

    def_str.pop_back();  // erase trailing /

    switch (def) {
      case Prompt_answer::YES:
        def_str.append(" (default ").append(clean_yes_text).append(")");
        break;

      case Prompt_answer::NO:
        def_str.append(" (default ").append(clean_no_text).append(")");
        break;

      case Prompt_answer::ALT:
        def_str.append(" (default ").append(clean_alt_text).append(")");
        break;

      default:
        break;
    }
  }

  while (final_ans == Prompt_answer::NONE) {
    if (this->prompt(prompt + " " + def_str, &ans)) {
      if (ans.empty()) {
        final_ans = def;
      } else {
        if (std::toupper(ans[0]) == yes_letter ||
            shcore::str_caseeq(ans, clean_yes_text)) {
          final_ans = Prompt_answer::YES;
        } else if (!clean_no_text.empty() &&
                   (std::toupper(ans[0]) == no_letter ||
                    shcore::str_caseeq(ans, clean_no_text))) {
          final_ans = Prompt_answer::NO;
        } else if (!clean_alt_text.empty() &&
                   (std::toupper(ans[0]) == alt_letter ||
                    shcore::str_caseeq(ans, clean_alt_text))) {
          final_ans = Prompt_answer::ALT;
        } else {
          println("\nPlease pick an option out of " + def_str);
        }
      }
    } else {
      break;
    }
  }
  return final_ans;
}  // namespace mysqlsh

shcore::Prompt_result Shell_console::prompt_password(const std::string &prompt,
                                                     std::string *out_val) {
  return m_ideleg->password(m_ideleg->user_data, prompt.c_str(), out_val);
}

}  // namespace mysqlsh
