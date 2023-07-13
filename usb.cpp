#include <gtest/gtest.h>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class USB
{
private:
    std::string folderPath;
    std::string destinationPath;

public:
    USB(const std::string& folderPath, const std::string& destinationPath)
        : folderPath(folderPath), destinationPath(destinationPath)
    {
    }

    std::vector<std::string> importDataFromFolder()
    {
        std::vector<std::string> copiedFiles;

        DIR* directory = opendir(folderPath.c_str());
        if (directory == nullptr) {
            std::cerr << "Failed to open directory: " << folderPath << std::endl;
            return copiedFiles;
        }

        dirent* entry;
        while ((entry = readdir(directory)) != nullptr) {
            std::string fileName = entry->d_name;
            if (fileName != "." && fileName != "..") {
                std::string sourceFilePath = folderPath + "/" + fileName;
                std::string destinationFilePath = destinationPath + "/" + fileName;

                std::ifstream sourceFile(sourceFilePath, std::ios::binary);
                std::ofstream destinationFile(destinationFilePath, std::ios::binary);

                if (sourceFile && destinationFile) {
                    destinationFile << sourceFile.rdbuf();
                    copiedFiles.push_back(destinationFilePath);
                    std::cout << "Copied file: " << fileName << std::endl;
                } else {
                    std::cerr << "Failed to copy file: " << fileName << std::endl;
                }

                sourceFile.close();
                destinationFile.close();
            }
        }

        closedir(directory);

        return copiedFiles;
    }
};

class USBTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create the mock_usb directory if it doesn't exist
        if (mkdir(usbFolderPath.c_str(), 0755) == -1) {
            std::cerr << "Failed to create directory: " << usbFolderPath << std::endl;
            return;
        }

        // Create some test files in the mock_usb folder
        std::ofstream file1(usbFolderPath + "/file1.txt");
        file1 << "Test data for file1";
        file1.close();

        std::ofstream file2(usbFolderPath + "/file2.txt");
        file2 << "Test data for file2";
        file2.close();
    }

    void TearDown() override
    {
        // Clean up the mock_usb directory after the tests
        fs::remove_all(usbFolderPath);
    }

    std::string usbFolderPath = "/path/to/mock_usb";
    std::string destinationFolderPath = "/path/to/mock_destination";
};

TEST_F(USBTest, ImportDataFromFolderTest)
{
    USB usb(usbFolderPath, destinationFolderPath);

    // Call the importDataFromFolder() method
    std::vector<std::string> copiedFiles = usb.importDataFromFolder();

    // Assert that the expected files are copied
    ASSERT_EQ(copiedFiles.size(), 2);
    ASSERT_EQ(copiedFiles[0], destinationFolderPath + "/file1.txt");
    ASSERT_EQ(copiedFiles[1], destinationFolderPath + "/file2.txt");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
