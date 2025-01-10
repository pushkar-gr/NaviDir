#include <gtest/gtest.h>
#include "FileManager.hpp"
#include "Config.hpp"

using namespace std::filesystem;

class FileManagerTest : public ::testing::Test {
protected:
  path tempDir;

  void SetUp() override {
    tempDir = temp_directory_path() / "NaviDirTestEnv";
    try {
      if (exists(tempDir)) {
        remove_all(tempDir);
      }
      create_directory(tempDir);
    } catch (const exception& e) {
      cerr << "Failed to remove temporary directory: " << e.what() << endl;
      exit(EXIT_FAILURE);
    }
  }

  void TearDown() override {
    try {
      if (exists(tempDir)) {
        remove_all(tempDir);
      }
    } catch (const exception& e) {
      cerr << "Failed to remove temporary directory: " << e.what() << endl;
      exit(EXIT_FAILURE);
    }
  }
};

TEST_F(FileManagerTest, ConstructorDefault) {
  Config config;
  FileManager fm(&config);
  ASSERT_EQ(*fm.getCurrentPath(), current_path());
}

TEST_F(FileManagerTest, ConstructorWithPath) {
  Config config;
  FileManager fm(tempDir, &config);
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, ConstructorWithDirEntry) {
  Config config;
  FileManager fm((directory_entry(tempDir)), &config);
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, SelectFileByPath) {
  Config config;
  path filePath = tempDir / "test.txt";
  std::ofstream(filePath) << "content";

  FileManager fm(tempDir, &config);

  ASSERT_TRUE(fm.selectFile(filePath));
  ASSERT_EQ(fm.getSelectedFile().path(), filePath);
}

TEST_F(FileManagerTest, SelectFileByIndex) {
  Config config;
  path filePath = tempDir / "test.txt";
  std::ofstream(filePath) << "content";

  FileManager fm(tempDir, &config);

  fm.selectFile(0);
  ASSERT_EQ(fm.getSelectedFile().path(), filePath);
}

TEST_F(FileManagerTest, SwitchPath) {
  Config config;
  path subDir = tempDir / "subDir";
  create_directory(subDir);

  FileManager fm(tempDir, &config);

  ASSERT_EQ(fm.switchPath(subDir), subDir);
  ASSERT_EQ(*fm.getCurrentPath(), subDir);
}

TEST_F(FileManagerTest, SwitchToParent) {
  Config config;
  path subDir = tempDir / "subDir";
  create_directory(subDir);

  FileManager fm(subDir, &config);

  ASSERT_EQ(fm.switchToParent(), tempDir);
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, GetSelectedFileContent) {
  Config config;
  path filePath = tempDir / "test.txt";
  string content = "file content\n";
  std::ofstream(filePath) << content;

  FileManager fm(tempDir, &config);

  fm.selectFile(filePath);
  
  ASSERT_EQ(fm.getSelectedFileContent(), content);
}

TEST_F(FileManagerTest, FileIterators) {
  Config config;
  path p1 = tempDir / "test1";
  create_directory(p1);
  path p2 = tempDir / "test2";
  create_directory(p2);
  path p3 = tempDir / "test3";
  create_directory(p3);

  FileManager fm(tempDir, &config);

  auto it = fm.filesBegin();
  ASSERT_EQ((*(it++))->path(), p1);
  ASSERT_EQ((*(it++))->path(), p2);
  ASSERT_EQ((*(it++))->path(), p3);
}
