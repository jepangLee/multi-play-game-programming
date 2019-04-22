#ifndef BYTE_SWAPPER_H
#define BYTE_SWAPPER_H


#include "pch.h"
#include "TypeAliaser.h"

template<class T, size_t tSize> class ByteSwapper;

template<class T> 
class ByteSwapper<T, 2> {
public:
	T Swap(T inData) const {
		uint16_t result = ByteSwap(T inData);
		return TiypeAliaser<uint16_t, T>(result).Get();
	}
	T ByteSwap(T inData) { return (inData >> 8) | (inData << 8); }
};

template<class T>
class ByteSwapper<T, 4> {
public:
	T Swap(T inData) const {
		uint32_t result = ByteSwap(TypeAliaser<T, uint32_t>(inData).Get());
		return TiypeAliaser<uint32_t, T>(result).Get();
	}
	T ByteSwap(T inData) { 
		return	((inData >> 24) & 0x000000ff) |
				((inData >> 8) & 0x0000ff00) |
				((inData >> 8) & 0x00ff0000) |
				((inData >> 24) & 0xff000000);
	}
};

template<class T>
class ByteSwapper<T, 8> {
public:
	T Swap(T inData) const {
		uint64_t result = ByteSwap(TypeAliaser<T, uint64_t>(inData).Get());
		return TiypeAliaser<uint64_t, T>(result).Get();
	}
	T ByteSwap(T inData) {
		return	((inData >> 56) & 0x00000000000000ff)	|
				((inData >> 40) & 0x000000000000ff00)	|
				((inData >> 24) & 0x0000000000ff0000)	|
				((inData >>  8) & 0x00000000ff000000)	|
				((inData >>  8) & 0x000000ff00000000)	|
				((inData >> 24) & 0x0000ff0000000000)	|
				((inData >> 40) & 0x00ff000000000000)	|
				((inData >> 56) & 0xff00000000000000);
	}
};

template<class T>
T ByteSwap(T inData) {
	return ByteSwapper<T, sizeof(T)>().Swap(inData);
}

#endif