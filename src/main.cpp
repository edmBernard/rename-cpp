// Tool to rename files based on Regex

#include <string>
#include <filesystem>
#include <regex>

#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fmt/color.h>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  try {
    spdlog::cfg::load_env_levels();

    // =================================================================================================
    // CLI
    cxxopts::Options options(argv[0], "Tool to rename files based on Regex");
    options.positional_help("[optional args]").show_positional_help();

    // clang-format off
    options.add_options()
      ("h,help", "Print help")
      ("no-dry-run", "Actually apply the changes")
      ("d,directory", "The directory containing files to rename", cxxopts::value<std::string>(), "DIRECTORY")
      ("regex", "The regular expression that will be matched against the filename", cxxopts::value<std::string>())
      ("format", "The regex replacement format string", cxxopts::value<std::string>())
      ("v,verbose", "Print names of files successfully renamed")
      ("no-color", "Disable color in verbose mode")
      ;
    // clang-format on
    options.parse_positional({"regex", "format", "directory"});
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (!result.count("directory")) {
      spdlog::error("Directory argument required");
      return 1;
    }

    fs::path directory(result["directory"].as<std::string>());
    if (!fs::exists(directory)) {
      spdlog::error("Specified directory does not exist");
      return 1;
    }
    if (!fs::is_directory(directory)) {
      spdlog::error("Specified directory is not a directory");
      return 1;
    }

    if (!result.count("regex")) {
      spdlog::error("Missing input regex");
      return 1;
    }

    if (!result.count("format")) {
      spdlog::error("Missing format regex, if you tried to pass empty string use --format=\"\" instead.");
      return 1;
    }

    spdlog::debug("CommandLine Argument passed: ");
    spdlog::debug("  directory  : {}", result["directory"].as<std::string>());
    spdlog::debug("  regex      : {}", result["regex"].as<std::string>());
    spdlog::debug("  format     : {}", result["format"].as<std::string>());
    spdlog::debug("  no-dry-run : {}", result["no-dry-run"].count());
    spdlog::debug("  no-color   : {}", result["no-color"].count());

    const std::regex rule(result["regex"].as<std::string>());
    const std::string format(result["format"].as<std::string>());

    bool noRenamingDone = true;
    for(auto& p: fs::directory_iterator(directory)) {
      fs::path pathAbsolute = fs::absolute(p.path());
      if (!fs::is_regular_file(pathAbsolute)) {
        continue;
      }

      const std::string newFilename = std::regex_replace(pathAbsolute.filename().string(), rule, format);
      const fs::path newPath = pathAbsolute.parent_path() / newFilename;

      if (pathAbsolute != newPath) {
        if (result.count("verbose")) {
          if (result.count("no-color")) {
            fmt::print("{:40} will be renamed in  {}\n", pathAbsolute.filename().string(), newPath.filename().string());
          } else {
            fmt::print(fg(fmt::color::steel_blue), "{:40}", pathAbsolute.filename().string());
            fmt::print(" will be renamed in  ", pathAbsolute.filename().string(), newPath.filename().string());
            fmt::print(fg(fmt::color::aqua), "{}\n", newPath.filename().string());
          }
        }

        noRenamingDone = false;
        if (result.count("no-dry-run")) {
          fs::rename(pathAbsolute, newPath);
        }

      }
    }

    if (noRenamingDone) {
      spdlog::info("No renaming to do with current parameters.");
    }

    if (!result.count("no-dry-run")) {
      spdlog::info("If you are sure you want to apply the changes, run this command with the --no-dry-run option.");
    }

  } catch (const cxxopts::OptionException &e) {
    spdlog::error("Parsing options : {}", e.what());
    return 1;
  } catch (const std::exception &e) {
    spdlog::error("{}", e.what());
    return 1;
  }
  return 0;
}
