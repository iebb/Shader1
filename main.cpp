#include <fstream>
#include <iostream>
#include <map>

#include <csignal>
#include <cstring>
#include <cstdint>

extern "C" {
#include <sys/stat.h>
#include <sys/sysinfo.h>
}

#include "key_test.hpp"
#include "gpg_helper.hpp"

volatile sig_atomic_t cleanup_flag = 0;

void signal_handler(int sig) {
    (void)sig;
    cleanup_flag = 1;
}

char buffer[30];
char f_buffer[120];

int _main(std::string pattern, unsigned start, unsigned count_max = 1) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    umask(0077);

    const int thread_per_block = 1024;
    const int time_offset = 1ull << 26;

    const int num_block = time_offset / thread_per_block;

    unsigned long long count = 0ULL;
    auto t0 = std::chrono::steady_clock::now();

    std::ifstream file("derfile.bin", std::ios::binary);
    std::vector<uint8_t> _key((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::cout << "Loaded key, size: " << _key.size() << std::endl;
    CudaManager manager(num_block, thread_per_block, 4294967295ull);
    manager.load_patterns(pattern);
    for (unsigned long i = start; i < 4294967295ull; i++) {
        _key[20] = (i >> 24) & 0xFF;
        _key[21] = (i >> 16) & 0xFF;
        _key[22] = (i >> 8) & 0xFF;
        _key[23] = (i) & 0xFF;
        for (unsigned long long j = time_offset - 1; j <= 4294967295ull; j += time_offset) {
            if (cleanup_flag) {
                 return 0;
            }
            manager.test_key(_key, j);
            u32 result_time = manager.get_result_time();
            if (result_time != UINT32_MAX) {
                // _key[5] = result_time;
                printf("Found %08x %08x\n", result_time, (unsigned)i);

                _key[16] = (result_time >> 24) & 0xFF;
                _key[17] = (result_time >> 16) & 0xFF;
                _key[18] = (result_time >> 8) & 0xFF;
                _key[19] = (result_time) & 0xFF;

                sprintf(buffer, "result-%08x%08x.bin", result_time, (unsigned)i);

                std::ofstream fs(buffer, std::ios::out | std::ios::binary);
                std::copy(_key.cbegin(), _key.cend(), std::ostreambuf_iterator<char>(fs));
                fs.close();

                sprintf(f_buffer, "python3 found.py result-%08x%08x.bin %08x%08x %d %d", result_time, (unsigned)i, result_time, (unsigned)i, count, elapsed.count());
                system(f_buffer);
                count_max -= 1;
                if (count_max == 0) {
                    return 0;
                }
            }

            auto t1 = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = t1 - t0;
            count += num_block * thread_per_block;
            printf("\rBlock %d | Speed: %.4lf MH/s", i, count / elapsed.count() / 1000000);
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 2) {
            return _main(std::string(argv[1]), 0);
        }
        if (argc == 3) {
            return _main(std::string(argv[1]), atoll(argv[2]));
        }
        if (argc == 4) {
            return _main(std::string(argv[1]), atoll(argv[2]), atoll(argv[3]));
        }
        std::cerr << "usage: "  << argv[0] << "[pattern] [start_at=0] [max_matches=1]"  << std::endl;
        return EXIT_FAILURE;
    } catch (const std::runtime_error &e) {
        // avoid annoying SIGABRT
        std::cerr << "usage: "  << argv[0] << "[pattern] [start_at=0] [max_matches=1]"  << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
