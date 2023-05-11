#include "h265_ps.h"
#include <string.h>
#include "h2645_util.h"

int h265_get_vps_sps_pps(uint8_t *data, int len, uint8_t *vps, int *vps_len, uint8_t *sps, int *sps_len, uint8_t *pps, int *pps_len)
{
	uint8_t nalu_t; int nalu_len;
	uint8_t *r, *end = data + len;
	*vps_len = 0, *sps_len = 0; *pps_len = 0;

	r = avc_find_startcode(data, end);

	while (r < end)
	{
		uint8_t *r1;

		while (!*(r++));
		r1 = avc_find_startcode(r, end);
		nalu_t = (r[0] >> 1) & 0x3F;
		nalu_len = (int)(r1 - r);

		if (nalu_t == 32) {
			memcpy(vps, r, nalu_len);
			*vps_len = nalu_len;
		}
		else if (nalu_t == 33) {
			memcpy(sps, r, nalu_len);
			*sps_len = nalu_len;
		}
		else if (nalu_t == 34) {
			memcpy(pps, r, nalu_len);
			*pps_len = nalu_len;
		}

		if (*vps_len > 0 && *sps_len > 0 && *pps_len > 0)
			break;

		r = r1;
	}

	return (*vps_len > 0 && *sps_len > 0 && *pps_len > 0) ? 0 : -1;
}