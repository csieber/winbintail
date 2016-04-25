#include <iostream>
#include <fstream>
#include <inttypes.h>

#ifdef _WIN32
	#include <windows.h>
	#include <fcntl.h> // _O_BINARY
	unsigned int _CRT_fmode = _O_BINARY;

#else
	#include <unistd.h>
#endif

/*
 * Settings
 */
const int32_t poll_interval_millis = 100;
const int32_t buff_size = 500000;

int main(int argc, const char** argv) {

	if (argc == 1) {

		std::cerr << "Usage: winbintail [file]" << std::endl;
		return 1;
	}

	std::ifstream file;

	file.open(argv[1], std::ifstream::in | std::ifstream::binary);

	if (file.fail()) {

		std::cerr << "Failed to open file!" << std::endl;
		return 1;
	}

	char buff[buff_size];
	std::streamsize read;

	while (!file.fail()) {

		read = file.readsome(buff, buff_size);

		if (read > 0) {

			std::cout.write(buff, read);
			std::cout.flush();
			continue;
		}

#ifdef _WIN32
		Sleep(poll_interval_millis);
#else
		usleep(poll_interval_millis*1000);
#endif
	}

	file.close();

	return 0;
}
