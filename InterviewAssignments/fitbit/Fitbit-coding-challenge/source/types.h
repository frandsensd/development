#ifndef _FITBIT12BITBIN2TEXTUTIL_TYPES_H_
#define _FITBIT12BITBIN2TEXTUTIL_TYPES_H_

/* Standard types
*/
typedef		unsigned char			uint8_t;
typedef		unsigned short			uint16_t;

/* rawsample_t is the type used to store a raw accelerometer sample from binary file.
** Ensure there's a match in bit allocations between ACCELEROMETERRESOLUTION and rawsample_t!
*/
typedef		unsigned short			rawsample_t;

/* sample_t is the type used to store an accelerometer sample.
** Ensure there's a match in bit allocations between BYTESPERSAMPLE and sample_t!
*/
typedef		unsigned short			sample_t;

#define TRUE					1
#define FALSE					0

#endif /* _FITBIT12BITBIN2TEXTUTIL_TYPES_H_ */

