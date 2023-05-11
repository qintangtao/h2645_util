#include <stdio.h>
#include <h2645_version.h>

int main(int argc, char *argv[])
{
	printf("version: %d.%d.%d\n", H2645_UTIL_VERSION_MAJOR, H2645_UTIL_VERSION_MINOR, H2645_UTIL_VERSION_DEBUG);

	return 0;
}