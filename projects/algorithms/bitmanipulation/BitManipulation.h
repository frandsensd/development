#pragma once

//
// Defined for types UINT16 and UINT32
//

// Toggles bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in T], idx1>=idx0
template <typename T>
T	ToggleBits(T idx0, T idx1, T number);

// Reverses bits between range 2^idx1..2^idx0, idxn=[0..bits-1 in T], idx1>=idx0
template <typename T>
T	ReverseBits(T idx0, T idx1, T number);

