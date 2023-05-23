#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <getopt.h>
#include <h2645_version.h>
#include <h264_ps.h>
#include <h265_ps.h>

static void
print_usage(FILE *out, const char *prog, int exit_code)
{
	fprintf(out,
		"Syntax: %s { OPTS } -H <4:h264, 5:h265> <input file> {<output VPS file> <output SPS file> <output PPS file>}\n"
		" -e      - extract VPS SPS PPS\n"
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
	FILE *f_i = NULL;
	FILE *f_vps = NULL;
	FILE *f_sps = NULL;
	FILE *f_pps = NULL;
	uint8_t *data = NULL;
	int64_t len;
	int64_t r_len;
	uint8_t vps[256]; uint32_t vps_len;
	uint8_t sps[256]; uint32_t sps_len;
	uint8_t pps[256]; uint32_t pps_len;

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
		(!extract && (argc - optind) < 1) ||
		(extract && type == 4 && (argc - optind) < 3) ||
		(extract && type == 5 && (argc - optind) < 4)) {
		print_usage(stdout, argv[0], EXIT_FAILURE);
	}

	input_file = argv[optind++];
	if (extract) {
		if (type == 5)
			vps_file = argv[optind++];
		sps_file = argv[optind++];
		pps_file = argv[optind++];
	}
	
	printf("version: %d.%d.%d\n", H2645_UTIL_VERSION_MAJOR, H2645_UTIL_VERSION_MINOR, H2645_UTIL_VERSION_DEBUG);

	f_i = fopen(input_file, "rb");
	if (!f_i) {
		fprintf(stderr, "open input file error.");
		goto fail;
	}

	_fseeki64(f_i, 0, SEEK_END);
	len = _ftelli64(f_i);
	_fseeki64(f_i, 0, SEEK_SET);

	if (len <= 0) {
		fprintf(stderr, "get input file length error. size=%d\n", len);
		goto fail;
	}

	data = malloc(len);
	if (!data) {
		fprintf(stderr, "alloc memory error.");
		goto fail;
	}

	r_len = fread(data, 1, len, f_i);
	if (r_len != len) {
		fprintf(stderr, "read input file error.");
		goto fail;
	}

	if (extract) {
		if (type == 5) {
			f_vps = fopen(vps_file, "wb");
			if (!f_vps) {
				fprintf(stderr, "open vps file error.");
				goto fail;
			}
		}

		f_sps = fopen(sps_file, "wb");
		if (!f_sps) {
			fprintf(stderr, "open sps file error.");
			goto fail;
		}

		f_pps = fopen(pps_file, "wb");
		if (!f_pps) {
			fprintf(stderr, "open pps file error.");
			goto fail;
		}

		if (type == 4) {
			if (!h264_get_sps_pps(data, len, sps, &sps_len, pps, &pps_len)) {
				fwrite(sps, 1, sps_len, f_sps);
				fwrite(pps, 1, pps_len, f_pps);
			} else {
				fprintf(stderr, "extract SPS PPS error.");
			}
		} else if (type == 5) {
			if (!h265_get_vps_sps_pps(data, len, vps, &vps_len, sps, &sps_len, pps, &pps_len)) {
				fwrite(vps, 1, vps_len, f_vps);
				fwrite(sps, 1, sps_len, f_sps);
				fwrite(pps, 1, pps_len, f_pps);
			} else {
				fprintf(stderr, "extract VPS SPS PPS error.");
			}
		}
	} 
	else if (decode_VPS) {

	}
	else if (decode_SPS) {
		SPS sps;
		if (!h264_decode_sps(data, len, &sps)) {
			static const char csp[4][5] = { "Gray", "420", "422", "444" };
			fprintf(stdout,
				"sps:%u profile:%d/%d poc:%d ref:%d %dx%d fps:%d %s %s crop:%u/%u/%u/%u %s %s %"PRId32"/%"PRId32" b%d reo:%d\n",
				sps.sps_id, sps.profile_idc, sps.level_idc,
				sps.poc_type,
				sps.ref_frame_count,
				h264_get_width(&sps), h264_get_height(&sps), h264_get_framerate(&sps),
				sps.frame_mbs_only_flag ? "FRM" : (sps.mb_aff ? "MB-AFF" : "PIC-AFF"),
				sps.direct_8x8_inference_flag ? "8B8" : "",
				sps.crop_left, sps.crop_right,
				sps.crop_top, sps.crop_bottom,
				sps.vui_parameters_present_flag ? "VUI" : "",
				csp[sps.chroma_format_idc],
				sps.timing_info_present_flag ? sps.num_units_in_tick : 0,
				sps.timing_info_present_flag ? sps.time_scale : 0,
				sps.bit_depth_luma,
				sps.bitstream_restriction_flag ? sps.num_reorder_frames : -1
			);
		}
		else {
			fprintf(stderr, "decode SPS error.");
		}
	}
	else if (decode_PPS) {
		SPS sps;
		PPS pps;
		if (!h264_decode_pps(data, len, &sps, &pps))
		{
			fprintf(stdout,
				"pps:%u sps:%u %s slice_groups:%d ref:%u/%u %s qp:%d/%d/%d/%d %s %s %s %s\n",
				pps.pps_id, pps.sps_id,
				pps.cabac ? "CABAC" : "CAVLC",
				pps.slice_group_count,
				pps.ref_count[0], pps.ref_count[1],
				pps.weighted_pred ? "weighted" : "",
				pps.init_qp, pps.init_qs, pps.chroma_qp_index_offset[0], pps.chroma_qp_index_offset[1],
				pps.deblocking_filter_parameters_present ? "LPAR" : "",
				pps.constrained_intra_pred ? "CONSTR" : "",
				pps.redundant_pic_cnt_present ? "REDU" : "",
				pps.transform_8x8_mode ? "8x8DCT" : "");
		}
		else {
			fprintf(stderr, "decode PPS error.");
		}
	}

fail:
	if (f_i)
		fclose(f_i);
	if (f_vps)
		fclose(f_vps);
	if (f_sps)
		fclose(f_sps);
	if (f_pps)
		fclose(f_pps);
	if (data)
		free(data);

	return 0;
}