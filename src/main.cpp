// Tool to rename files based on Regex

#include <string>
#include <filesystem>

#include <cxxopts.hpp>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  try {
    // =================================================================================================
    // CLI
    cxxopts::Options options(argv[0], "Tool to rename files based on Regex");
    options.positional_help("[optional args]").show_positional_help();

    // clang-format off
    options.add_options()("help", "Print help")
      ("n,no-act", "show changes without doing anythings", cxxopts::value<bool>()->default_value("False"))
      ("d,directory", "directory", cxxopts::value<std::string>(), "DIRECTORY")
      ("r,recursive", "find file recursively", cxxopts::value<bool>())
      ;
    // clang-format on
    options.parse_positional({"directory"});
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
    for(auto& p: fs::directory_iterator(directory)) {
      fs::path path_absolute = fs::absolute(p.path());
      if (!fs::is_regular_file(path_absolute)) {
        continue;
      }
      std::cout << path_absolute << std::endl;
      std::cout << path_absolute.filename() << std::endl;
    }


  } catch (const cxxopts::OptionException &e) {
    std::cout << "Error: parsing options: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
