// Tool to rename files based on Regex

#include <string>
#include <filesystem>
#include <regex>

#include <cxxopts.hpp>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  try {
    // =================================================================================================
    // CLI
    cxxopts::Options options(argv[0], "Tool to rename files based on Regex");
    options.positional_help("[optional args]").show_positional_help();

    // clang-format off
    options.add_options()
      ("h,help", "Print help")
      ("n,no-act", "show what files would have been renamed but do nothing")
      ("d,directory", "the directory containing files to rename", cxxopts::value<std::string>(), "DIRECTORY")
      ("regex", "the regular expression that will be matched against the filename", cxxopts::value<std::string>())
      ("format", "the regex replacement format string", cxxopts::value<std::string>())
      ("v,verbose", "print names of files successfully renamed")
      ;
    // clang-format on
    options.parse_positional({"match", "replace", "directory"});
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (!result.count("directory")) {
      std::cout << "Error: directory argument required" << std::endl;
      return 1;
    }

    fs::path directory(result["directory"].as<std::string>());
    if (!fs::exists(directory)) {
      std::cout << "Error: directory does not exist" << std::endl;
      return 1;
    }
    if (!fs::is_directory(directory)) {
      std::cout << "Error: specified directory is not a directory" << std::endl;
      return 1;
    }

    const std::regex rule(result["match"].as<std::string>());
    const std::string format(result["replace"].as<std::string>());

    for(auto& p: fs::directory_iterator(directory)) {
      fs::path pathAbsolute = fs::absolute(p.path());
      if (!fs::is_regular_file(pathAbsolute)) {
        continue;
      }

      const std::string newFilename = std::regex_replace(pathAbsolute.filename().string(), rule, format);
      const fs::path newPath = pathAbsolute.parent_path() / newFilename;

      if (pathAbsolute != newPath) {
        if (result.count("verbose")) {
          std::cout << pathAbsolute.filename() << "  will be renamed in  " << newPath.filename() << std::endl;
        }

        if (!result.count("no-act")) {
          fs::rename(pathAbsolute, newPath);
        }
      }
    }


  } catch (const cxxopts::OptionException &e) {
    std::cout << "Error: parsing options: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
