#include <types.h>

// NOTES: Only valid for integers. If number is signed, cast to unsigned prior to call.
UINT16	ToggleBits(UINT16 idx0, UINT16 idx1, UINT16 number)	// Toggles bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in UINT16], idx1>=idx0
{
	UINT16	range = (1 << (idx1 + 1)) - (1 << idx0);
	UINT16	desiredBits = number & ~range;
	UINT16	bitsToToggle = number & range;
	return desiredBits | ((~bitsToToggle) & range);
}


// NOTES: Only valid for integers. If number is signed, cast to unsigned prior to call.
UINT16	ReverseBits(UINT16 idx0, UINT16 idx1, UINT16 number)	// Reverses bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in UINT16], idx1>=idx0
{
	UINT16	range = (1 << (idx1 + 1)) - (1 << idx0);
	UINT16	desiredBits = number & ~range;
	UINT16	bitsToReverse = number & range;
	UINT16	tmpBit;
	for (UINT16 i = 0; i <= (UINT16)(idx1 - idx0); i++) {
		tmpBit = (bitsToReverse >> (idx0 + i) & 0x1) << (idx1 - i);
		desiredBits |= tmpBit;
	}
	return desiredBits;
}

// NOTES: Only valid for integers. If number is signed, cast to unsigned prior to call.
UINT32	ToggleBits(UINT32 idx0, UINT32 idx1, UINT32	number)	// Toggles bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in UINT32], idx1>=idx0
{
	UINT32	range = (1 << (idx1 + 1)) - (1 << idx0);
	UINT32	desiredBits = number & ~range;
	UINT32	bitsToToggle = number & range;
	return desiredBits | ((~bitsToToggle) & range);
}


// NOTES: Only valid for integers. If number is signed, cast to unsigned prior to call.
UINT32	ReverseBits(UINT32 idx0, UINT32 idx1, UINT32 number)	// Reverses bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in UINT32], idx1>=idx0
{
	UINT32	range = (1 << (idx1 + 1)) - (1 << idx0);
	UINT32	desiredBits = number & ~range;
	UINT32	bitsToReverse = number & range;
	UINT32	tmpBit;
	for (UINT32 i = 0; i <= (UINT32)(idx1 - idx0); i++) {
		tmpBit = (bitsToReverse >> (idx0 + i) & 0x1) << (idx1 - i);
		desiredBits |= tmpBit;
	}
	return desiredBits;
}
