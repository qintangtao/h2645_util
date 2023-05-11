#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <h2645_version.h>
#include <h264_ps.h>

static void
print_usage(FILE *out, const char *prog, int exit_code)
{
	fprintf(out,
		"Syntax: %s { OPTS } -H <4:h264, 5:h265> <input file> {<output VPS file> <output SPS file> <output PPS file>}\n"
		" -e      - extract VPS or SPS or PPS\n"
		" -v      - decode VPS\n"
		" -s      - decode SPS\n"
		" -p      - decode PPS\n",
		prog);
	exit(exit_code);
}

int main(int argc, char *argv[])
{
	int type = 0;
	int extract = 0;
	int decode_VPS = 0;
	int decode_SPS = 0;
	int decode_PPS = 0;
	const char *input_file;
	const char *vps_file;
	const char *sps_file;
	const char *pps_file;

	int opt;
	while ((opt = getopt(argc, argv, "evspH:")) != EOF)
	{
		switch (opt)
		{
		case 'e':
			extract = 1;
			break;
		case 'v':
			decode_VPS = 1;
		break; 
		case 's':
			decode_SPS = 1;
		break; 
		case 'p':
			decode_PPS = 1;
			break;
		case 'H':
			type = atoi(optarg);
			if (type != 4 && type != 5)
				print_usage(stdout, argv[0], EXIT_FAILURE);
			break;
		case 'h':
			print_usage(stdout, argv[0], EXIT_SUCCESS);
			break;
		default:
			fprintf(stderr, "Unknown option %c\n", opt);
			break;
		}
	}

	if (extract == 0 && decode_VPS == 0 && decode_SPS == 0 && decode_PPS == 0)
		print_usage(stdout, argv[0], EXIT_FAILURE);

	if (optind >= argc || 
		(type == 4 && (argc - optind) < 3) || 
		(type == 5 && (argc - optind) < 4)) {
		print_usage(stdout, argv[0], EXIT_FAILURE);
	}

	input_file = argv[optind++];
	if (type == 5)
		vps_file = argv[optind++];
	sps_file = argv[optind++];
	pps_file = argv[optind++];
	

	printf("version: %d.%d.%d\n", H2645_UTIL_VERSION_MAJOR, H2645_UTIL_VERSION_MINOR, H2645_UTIL_VERSION_DEBUG);

	return 0;
}