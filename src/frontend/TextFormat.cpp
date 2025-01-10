#include "TextFormat.hpp"
#include <filesystem>
#include <string>

unordered_map<string, string> fileIcons = {
  {".cpp", " "},
  {".hpp", " "},
  {".c", " "},
  {".h", " "},
  {".py", " "},
  {".txt", "󰈔 "},
  {".jpg", "󰈥 "},
  {".png", "󰸭 "},
  {".html", " "},
  {".css", " "},
  {".js", " "},
  {".ts", " "},
  {".php", " "},
  {".sh", " "},
  {".rs", " "},
  {".toml", " "},
  {".yaml", " "},
  {".go", "󰟓 "},
  {".md", "  "},
  {".java", " "},
  {"directory", " "},
  {"default", " "}
};

string formatText(const path& path, FormatType type) {
  switch (type) {
    case None: {
      return path;
    }
    case Simple: {
      if (is_directory(path)) {
        return path.string() + "/";
      }
      return path;
    }
    case NerdFont: {
      if (is_directory(path)) {
        return fileIcons["directory"] + path.string();
      }
      string extension = path.extension().string();
      if (fileIcons.find(extension) != fileIcons.end()) {
          return fileIcons[extension] + path.string();
      } else {
          return fileIcons["default"] + path.string();
      }
    }
  };
  return path;
}

string formatText(const directory_entry& entry, FormatType type) {
  return formatText(entry.path(), type);
}
