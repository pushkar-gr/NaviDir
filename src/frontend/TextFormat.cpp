#include "TextFormat.hpp"

unordered_map<string, string> fileIcons = {
  {".cpp", "  "},
  {".hpp", "  "},
  {".c", "  "},
  {".h", "  "},
  {".py", "  "},
  {".txt", "󰈔  "},
  {".jpg", "󰈥  "},
  {".png", "󰸭  "},
  {".html", "  "},
  {".css", "  "},
  {".js", "  "},
  {".ts", "  "},
  {".php", "  "},
  {".sh", "  "},
  {".rs", "  "},
  {".toml", "  "},
  {".yaml", "  "},
  {".go", "󰟓  "},
  {".md", "  "},
  {".java", "  "},
  {"directory", "  "},
  {"default", "  "}
};

string getIcon(const path& path) {
  if (is_directory(path)) {
    return fileIcons["directory"];
  }
  string extension = path.extension().string();
  if (fileIcons.find(extension) != fileIcons.end()) {
      return fileIcons[extension];
  } else {
      return fileIcons["default"];
  }
}

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
      return getIcon(path) + path.string();
    }
  };
  return path;
}

string formatText(const directory_entry& entry, FormatType type) {
  return formatText(entry.path(), type);
}

string formatText(const directory_entry& entry, FormatType type, Config *config) {
  return formatText(entry.path(), type, config);
}

char show(char op, perms perm, perms p) {
  return (perms::none == (perm & p)) ? '-' : op;
}

string formatText(const path& path, FormatType type, Config *config) {
  string text = "";
  if (config->displayPerms()) {
    perms perm = status(path).permissions();
    text.push_back(show('r', perms::owner_read, perm));
    text.push_back(show('w', perms::owner_write, perm));
    text.push_back(show('x', perms::owner_exec, perm));
    text.push_back(show('r', perms::group_read, perm));
    text.push_back(show('w', perms::group_write, perm));
    text.push_back(show('x', perms::group_exec, perm));
    text.push_back(show('r', perms::others_read, perm));
    text.push_back(show('w', perms::others_write, perm));
    text.push_back(show('x', perms::others_exec, perm));
    text.push_back(' ');
  }
  if (config->displayFileSize()) {
    if (is_regular_file(path)) {
      text += to_string(file_size(path));
    } else {
      text += "-";
    }
    text.push_back(' ');
  }
  if (config->displayDateMod()) {
    if (is_regular_file(path)) {
      file_time_type ftime = last_write_time(path);
      auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(ftime - file_time_type::clock::now() + chrono::system_clock::now());
      time_t cftime = chrono::system_clock::to_time_t(sctp);
      ostringstream oss;
      oss << put_time(localtime(&cftime), "%Y-%m-%d %H:%M:%S");
      text += oss.str();
    } else {
      text += "-";
    }
    text.push_back(' ');
  }
  switch (type) {
    case None: {
      return text + path.string();
    }
    case Simple: {
      if (is_directory(path)) {
        return text + path.string() + "/";
      }
      return text + path.string();
    }
    case NerdFont: {
      return getIcon(path) + text + path.string();
    }
  };
  return text + path.string();
} 
