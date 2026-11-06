#include "mapjson/Folder.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <utility>
#include "mapjson/Limits.hpp"

namespace rtype::mapjson {
    namespace {
        namespace fs = std::filesystem;

        bool isJson(const fs::path &file)
        {
            std::string extension = file.extension().string();

            std::ranges::transform(extension, extension.begin(),
                [](const unsigned char c) { return std::tolower(c); });
            return extension == ".json";
        }

        std::vector<fs::path> listJson(const fs::path &folder)
        {
            std::vector<fs::path> files;
            std::error_code error;

            for (fs::directory_iterator it(folder, error);
                !error && it != fs::directory_iterator(); it.increment(error)) {
                if (it->is_regular_file(error) && isJson(it->path())) {
                    files.push_back(it->path());
                }
            }
            std::ranges::sort(files);
            if (files.size() > MAX_FILES) {
                files.resize(MAX_FILES);
            }
            return files;
        }

        MapEntry loadEntry(const fs::path &file)
        {
            MapEntry entry;
            std::string text;

            entry.file = file.filename().string();
            entry.error = readMapFile(file, text);
            if (!entry.error.empty()) {
                return entry;
            }
            entry.hash = contentHash(text);
            ParseResult parsed = parseMap(text);
            entry.map = std::move(parsed.map);
            entry.error = std::move(parsed.error);
            return entry;
        }
    }

    std::string readMapFile(const fs::path &file, std::string &text)
    {
        std::error_code error;
        const auto size = fs::file_size(file, error);

        if (error) {
            return "cannot read the file";
        }
        if (size > MAX_FILE_BYTES) {
            return "map: file bigger than "
                + std::to_string(MAX_FILE_BYTES / 1024) + " KiB";
        }
        std::ifstream in(file, std::ios::binary);

        text.resize(static_cast<std::size_t>(size));
        if (!in.read(text.data(), static_cast<std::streamsize>(size))) {
            return "cannot read the file";
        }
        return "";
    }

    std::vector<MapEntry> scanMaps(const fs::path &folder)
    {
        std::vector<MapEntry> entries;
        std::error_code error;

        if (!fs::is_directory(folder, error)) {
            fs::create_directories(folder, error);
            return entries;
        }
        for (const fs::path &file : listJson(folder)) {
            entries.push_back(loadEntry(file));
        }
        return entries;
    }
}
