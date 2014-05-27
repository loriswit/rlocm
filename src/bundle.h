#ifndef BUNDLE_H
#define BUNDLE_H

///#include "pathlist.h"
#include "process.h"

struct file_info
{
    std::string name;
	uint64_t offset;
	uint32_t size;
    uint32_t zsize;
    bool ok;
};

class bundle
{
    public:
        bundle(const std::string& bundle_name);
        void open(const std::string& bundle_name);

        void patch(const std::string& patch_folder);
        file_info seek(const std::string& filepath, bool out = false);

        bool check_training(void);
        bool install_training_room(int resource = 101);
        bool uninstall_training_room(void);
        std::vector<uint8_t> load_resource(int name);

        void close(void);
        std::string get_last_error(void);

        uint32_t get_long(void);
        uint32_t get_long(std::streampos pos);
        uint64_t get_longlong(void);
        uint64_t get_longlong(std::streampos pos);
        std::string get_string(std::streamsize n);
        std::string get_string(std::streamsize n, std::streampos pos);
        static unsigned int swap(unsigned int i);

        /// CURRENTLY UNUSED METHODS:
        /**
        std::vector<uint8_t> compress_file(const std::string& filename);
        std::vector<uint8_t> decompress_data(std::streampos offset, size_t in_data_size, size_t out_data_size);
        std::vector<uint8_t> read_data(std::streampos offset, size_t in_data_size);
        void write_file(const std::string& filepath, const std::string& dir, const std::vector<uint8_t>& data);
        */

        operator bool() const;
        bool is_open;

    private:
        std::fstream file;
        std::string filename;
        bool success;

        unsigned int base_off;
        unsigned int files; // number of files

        std::string last_error;
};

#endif // BUNDLE_H
