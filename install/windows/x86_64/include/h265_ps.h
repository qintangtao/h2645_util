#ifndef H265_PS_H
#define H265_PS_H

#include <stdint.h>
#include "h2645_vui.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* split vps and sps and pps
	*/
	int h265_get_vps_sps_pps(uint8_t *data, int len, uint8_t *vps, int *vps_len, uint8_t *sps, int *sps_len, uint8_t *pps, int *pps_len);



#ifdef __cplusplus
}
#endif

#endif /* H265_PS_H */