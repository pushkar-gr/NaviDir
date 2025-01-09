#include <gtest/gtest.h>
#include "FileManager.hpp"

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
  FileManager fm;
  ASSERT_EQ(*fm.getCurrentPath(), current_path());
}

TEST_F(FileManagerTest, ConstructorWithPath) {
  FileManager fm(tempDir);
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, ConstructorWithDirEntry) {
  FileManager fm((directory_entry(tempDir)));
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, SelectFileByPath) {
  path filePath = tempDir / "test.txt";
  std::ofstream(filePath) << "content";

  FileManager fm(tempDir);

  ASSERT_TRUE(fm.selectFile(filePath));
  ASSERT_EQ(fm.getSelectedFile().path(), filePath);
}

TEST_F(FileManagerTest, SelectFileByIndex) {
  path filePath = tempDir / "test.txt";
  std::ofstream(filePath) << "content";

  FileManager fm(tempDir);

  fm.selectFile(0);
  ASSERT_EQ(fm.getSelectedFile().path(), filePath);
}

TEST_F(FileManagerTest, SwitchPath) {
  path subDir = tempDir / "subDir";
  create_directory(subDir);

  FileManager fm(tempDir);

  ASSERT_EQ(fm.switchPath(subDir), subDir);
  ASSERT_EQ(*fm.getCurrentPath(), subDir);
}

TEST_F(FileManagerTest, SwitchToParent) {
  path subDir = tempDir / "subDir";
  create_directory(subDir);

  FileManager fm(subDir);

  ASSERT_EQ(fm.switchToParent(), tempDir);
  ASSERT_EQ(*fm.getCurrentPath(), tempDir);
}

TEST_F(FileManagerTest, GetSelectedFileContent) {
  path filePath = tempDir / "test.txt";
  string content = "file content\n";
  std::ofstream(filePath) << content;

  FileManager fm(tempDir);

  fm.selectFile(filePath);
  
  ASSERT_EQ(fm.getSelectedFileContent(), content);
}

TEST_F(FileManagerTest, FileIterators) {
  path p1 = tempDir / "test1";
  create_directory(p1);
  path p2 = tempDir / "test2";
  create_directory(p2);
  path p3 = tempDir / "test3";
  create_directory(p3);

  FileManager fm(tempDir);

  auto it = fm.filesBegin();
  ASSERT_EQ((*(it++))->path(), p1);
  ASSERT_EQ((*(it++))->path(), p2);
  ASSERT_EQ((*(it++))->path(), p3);
}
