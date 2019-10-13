#include "shush-logs.hpp"

shush::logs::Logger::
Logger(std::string project_name, 
       const char logging_level,
       std::string logs_directory,
       std::string logs_file_name,
       std::map<std::string, std::string> var_map)
    : var_map_(std::move(var_map))
    , project_name_(std::move(project_name))
    , log_level_(logging_level)
    , line_template_(DEFAULT_LINE_TEMPLATE)
    , log_prefix_(DEFAULT_LOG_PREFIX_LOG)
    , log_suffix_(DEFAULT_LOG_SUFFIX_LOG)
    , dbg_prefix_(DEFAULT_LOG_PREFIX_DBG)
    , dbg_suffix_(DEFAULT_LOG_SUFFIX_DBG) {
  var_map_["project_name"] = project_name_;

  if (!file_.is_open()) {
    file_name_ = std::move(logs_file_name);
    directory_ = std::move(logs_directory);
    full_path_ = format::FormatString(directory_ + file_name_, var_map_);
    std::filesystem::create_directories(directory_);
    file_.open(full_path_, std::ios::app | std::ios::out);
  }

  Log(HELLO_STRING);
}

shush::logs::Logger::~Logger() {
  if (file_.is_open()) {
    file_.close();
  }
}

void shush::logs::Logger::SetFileName(const std::string& file_name) noexcept {
  file_name_ = file_name;
  full_path_ = format::FormatString(directory_ + file_name, var_map_);
  file_.open(full_path_, std::ios::app);
  Log(HELLO_STRING);
}

void shush::logs::Logger::SetDirectory(const std::string& directory) noexcept {
  directory_ = directory;
  std::filesystem::create_directories(directory_);
  full_path_ = format::FormatString(directory + file_name_, var_map_);
  file_.open(full_path_, std::ios::app);
  Log(HELLO_STRING);
}

void shush::logs::Logger::SetLogLevel(char log_level) noexcept {
  if (log_level >= LOG_LEVEL_LOWER_BOUND && log_level <= LOG_LEVEL_UPPER_BOUND) {
    log_level_ = log_level;
  }
}

void shush::logs::Logger::SetLogPrefix(const std::string& str) noexcept {
  log_prefix_ = str;
}

void shush::logs::Logger::SetLogSuffix(const std::string& str) noexcept {
  log_suffix_ = str;
}

void shush::logs::Logger::SetDbgPrefix(const std::string& str) noexcept {
  dbg_prefix_ = str;
}

void shush::logs::Logger::SetDbgSuffix(const std::string& str) noexcept {
  dbg_suffix_ = str;
}

void shush::logs::Logger::SetLogPrefix(std::string&& str) noexcept {
  log_prefix_ = std::move(str);
}

void shush::logs::Logger::SetLogSuffix(std::string&& str) noexcept {
  log_suffix_ = std::move(str);
}

void shush::logs::Logger::SetDbgPrefix(std::string&& str) noexcept {
  dbg_prefix_ = std::move(str);
}

void shush::logs::Logger::SetDbgSuffix(std::string&& str) noexcept {
  dbg_suffix_ = std::move(str);
}

void shush::logs::Logger::SetLineTemplate(const std::string& str) noexcept {
  line_template_ = str;
}

void shush::logs::Logger::SetLineTemplate(std::string&& str) noexcept {
  line_template_ = std::move(str);
}

inline void shush::logs::Logger::UpdateFixes(char cur_level) {
  if (cur_level == LOG_LEVEL_LOG) {
    var_map_["prefix"] = log_prefix_;
    var_map_["suffix"] = log_suffix_;
  } else if (log_level_ == LOG_LEVEL_DBG) {
    var_map_["prefix"] = dbg_prefix_;
    var_map_["suffix"] = dbg_suffix_;
  }
}

void shush::logs::Logger::WriteFormattedTemplate(char cur_level) {
  UpdateFixes(cur_level);

  if (!file_.is_open()) {
    file_.open(full_path_, std::ios::app);
  }

  // Two times, because inner parts may have variables.
  file_ << format::FormatString(format::FormatString(line_template_, var_map_),
                                var_map_);
}

void shush::logs::Logger::Log(const std::string& str) noexcept {
  var_map_["LOG"] = str;
  WriteFormattedTemplate(LOG_LEVEL_LOG);
}

void shush::logs::Logger::Log(std::string&& str) noexcept {
  var_map_["LOG"] = std::move(str);
  WriteFormattedTemplate(LOG_LEVEL_LOG);
}

void shush::logs::Logger::Log(const char* const str) noexcept {
  var_map_["LOG"] = str;
  WriteFormattedTemplate(LOG_LEVEL_LOG);
}

void shush::logs::Logger::Dbg(const std::string& str) noexcept {
  if (log_level_ == LOG_LEVEL_DBG) {
    var_map_["LOG"] = str;
    WriteFormattedTemplate(LOG_LEVEL_DBG);
  }
}

void shush::logs::Logger::Dbg(std::string&& str) noexcept {
  if (log_level_ == LOG_LEVEL_DBG) {
    var_map_["LOG"] = std::move(str);
    WriteFormattedTemplate(LOG_LEVEL_DBG);
  }
}

void shush::logs::Logger::Dbg(const char* const str) noexcept {
  if (log_level_ == LOG_LEVEL_DBG) {
    var_map_["LOG"] = str;
    WriteFormattedTemplate(LOG_LEVEL_DBG);
  }
}
