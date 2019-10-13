#pragma once
#include <string>
#include <map>
#include "shush-format.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace shush {
namespace logs {
inline const std::string DEFAULT_DIRECTORY      = "logs/";
inline const std::string DEFAULT_FILE_NAME      = "{project_name}-{date}.log";
inline const std::string DEFAULT_LOG_PREFIX_LOG = "{date} | LOG: ";
inline const std::string DEFAULT_LOG_SUFFIX_LOG = "\n";
inline const std::string DEFAULT_LOG_PREFIX_DBG = "{date} | DBG: ";
inline const std::string DEFAULT_LOG_SUFFIX_DBG = "\n";
inline const std::string DEFAULT_LINE_TEMPLATE  = "{prefix}{LOG}{suffix}";

inline const char        LOG_LEVEL_LOWER_BOUND  = 0;
inline const char        LOG_LEVEL_UPPER_BOUND  = 1;
inline const char        LOG_LEVEL_DBG          = 1;
inline const char        LOG_LEVEL_LOG          = 0;
inline const char        DEFAULT_LOGGING_LEVEL  = 1;

inline const std::string HELLO_STRING           = "STARTED LOGGING {project-name}";

/**
* @brief A simple noexcept logger.
* @dtails Available logging levels:
* - 0 - only basic information is written to logs file.
* - 1 - all debug information is written.
*
* logs_file_name is a template of a filename that the logs
* will be written to. Uses format syntax of shush::format.
*/
class Logger {
public:
  /**
   * @brief Creates the logger and starts logging by creating (if not present)
   * the .log file.
   *
   * @note Throughout all work of a program there should be only one file
   * opened for writing. So, if logging started previously, your file_names
   * won't have any effect and logging will proceed in previously opened file.
   *
   * @param[in] project_name   the name of a project, which by default
   *                           will be in file name.
   * @param[in] logging_level  logging level, 0 for only logs, 1 for
   *                           logs and debug info.
   *                           Default value is DEFAULT_LOGGING_LEVEL.
   * @param[in] logs_directory Directory where the file with logs will be stored.
   * @param[in] logs_file_name a template of a file name that will be
   *                           given to logs file. Supports shush::format.
   *                           Default value is DEFAULT_FILE_NAME.
   * @param[in] var_map        the map that has values for variables in
   *                           logs_file_name template. The same that is
   *                           used in shush::format. These variables could
   *                           be used later in other methods.
   */
  Logger(std::string        project_name,
         const char         logging_level  = DEFAULT_LOGGING_LEVEL,
         std::string        logs_directory = DEFAULT_DIRECTORY,
         std::string        logs_file_name = DEFAULT_FILE_NAME,
         std::map<std::string, std::string> var_map =
            std::map<std::string, std::string>());
  ~Logger();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  /**
   * \defgroup SetFileName
   * Set the file name template which will be used in logging, closes previous
   * file and opens the new.
   * Uses shush::format.
   *
   * @{
   */
  void SetFileName(const std::string& file_name) noexcept;
  void SetDirectory(const std::string& directory) noexcept;
  /** @} */

  void SetLogLevel(char log_level) noexcept;

  /**
   * \defgroup SetLogPrefix
   * Sets how the lines of log/dbg will look like.
   * The default template is DEFAULT_LINE_TEMPLATE.
   *
   * @note Remember to set "\\n" in the end of suffix.
   *
   * @{
   */
  void SetLogPrefix(const std::string& str) noexcept;
  void SetLogSuffix(const std::string& str) noexcept;
  void SetDbgPrefix(const std::string& str) noexcept;
  void SetDbgSuffix(const std::string& str) noexcept;

  void SetLogPrefix(std::string&& str) noexcept;
  void SetLogSuffix(std::string&& str) noexcept;
  void SetDbgPrefix(std::string&& str) noexcept;
  void SetDbgSuffix(std::string&& str) noexcept;
  /** @} */

  /**
   * Sets the template for lines of logger to write in logs file.
   * Default is DEFAULT_LINE_TEMPLATE.
   * You can use variables {prefix}, {suffix}, {LOG}
   */
  void SetLineTemplate(const std::string& str) noexcept;
  void SetLineTemplate(std::string&& str) noexcept;
  
  void Log(const std::string& str) noexcept;
  void Log(std::string&& str) noexcept;
  void Log(const char* const str) noexcept;
  void Dbg(const std::string& str) noexcept;
  void Dbg(std::string&& str) noexcept;
  void Dbg(const char* const str) noexcept;
  
private:
  // Update suffix, prefix.
  inline void UpdateFixes(char cur_level);
  void WriteFormattedTemplate(char cur_level);

  std::map<std::string, std::string> var_map_;
  std::string project_name_;
  char log_level_;
  static std::string file_name_;
  static std::string directory_;
  static std::string full_path_;

  std::string line_template_;
  std::string log_prefix_;
  std::string log_suffix_;
  std::string dbg_prefix_;
  std::string dbg_suffix_;

  static std::ofstream file_;
};

}
}
