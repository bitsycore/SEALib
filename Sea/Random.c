#include "Random.h"

#include "Compat.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <Windows.h>
	#include <bcrypt.h>
	#include <stdio.h>
#elif defined(__unix__) || defined(__APPLE__)
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
#else
	#include <stdio.h>
	#include <time.h>
#endif

static SEA_THREAD_LOCAL bool SEED_INITIALIZED = false;
static SEA_THREAD_LOCAL uint64_t SEED[4];

static uint64_t rotate_left(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static void seed256(uint64_t seed[4]) {
#if defined(_WIN32) || defined(_WIN64)
	BCRYPT_ALG_HANDLE hAlgorithm = NULL;
	NTSTATUS status = BCryptOpenAlgorithmProvider(
		&hAlgorithm,
		BCRYPT_RNG_ALGORITHM,
		NULL,
		0
	);
	if (status != 0) {
		fprintf(stderr, "Error opening algorithm provider: %08lx\n", status);
		exit(1);
	}
	status = BCryptGenRandom(
		hAlgorithm,
		(PBYTE) seed,
		sizeof(uint64_t) * 4,
		0
	);
	if (status != 0) {
		fprintf(stderr, "Error generating random bytes: %08lx\n", status);
		exit(1);
	}
	status = BCryptCloseAlgorithmProvider(hAlgorithm, 0);
	if (status != 0) {
		fprintf(stderr, "Error generating random bytes: %08lx\n", status);
		exit(1);
	}
#elif defined(__unix__) || defined(__APPLE__)
	const int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		perror("Failed to open /dev/urandom");
		exit(1);
	}
	if (read(fd, seed, sizeof(uint64_t) * 4) != sizeof(uint64_t) * 4) {
		perror("Failed to read random bytes from /dev/urandom");
		exit(1);
	}
	close(fd);
#else
	for (int i = 0; i < 4; ++i) {
		uint64_t time_part = (uint64_t)time(NULL) << 32;
        uint64_t rand_part = (uint64_t)rand();
        uint64_t clock_part = (uint64_t)clock() & 0xFFFFFFFF;
		seed[i] = time_part ^ rand_part ^ clock_part;
	}
#endif
}

static uint64_t RandUint64() {
	if (SEED_INITIALIZED == false) {
		SEED_INITIALIZED = true;
		seed256(SEED);
	}

	const uint64_t result = rotate_left(SEED[0] + SEED[3], 23) + SEED[0];

	const uint64_t t = SEED[1] << 17;

	SEED[2] ^= SEED[0];
	SEED[3] ^= SEED[1];
	SEED[1] ^= SEED[2];
	SEED[0] ^= SEED[3];

	SEED[2] ^= t;

	SEED[3] = rotate_left(SEED[3], 45);

	return result;
}

static uint32_t RandUint32() {
	return (uint32_t) (RandUint64() >> 32);
}

static int64_t RandInt64() {
	return (int64_t) RandUint64();
}

static int32_t RandInt32() {
	return (int32_t) RandUint32();
}

static uint16_t RandUint16() {
	return (uint16_t) (RandUint64() & 0xFFFF);
}

static int16_t RandInt16() {
	return (int16_t) (RandUint64() & 0xFFFF);
}

static uint8_t RandUint8() {
	return (uint8_t) (RandUint64() & 0xFF);
}

static int8_t RandInt8() {
	return (int8_t) (RandUint64() & 0xFF);
}

static float RandFloat() {
	return ((float) (RandUint64() >> 40)) / 16777216.0f; // 2^24
}

static double RandDouble() {
	return ((double) (RandUint64() >> 11)) / 9007199254740992.0; // 2^53
}

static bool RandBool() {
	return (RandUint64() >> 63) != 0;
}

static void FillRandomBytes(uint8_t* buffer, size_t length) {
	while (length >= 8) {
		uint64_t r = RandUint64();
		for (int i = 0; i < 8; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
		buffer += 8;
		length -= 8;
	}

	if (length > 0) {
		uint64_t r = RandUint64();
		for (size_t i = 0; i < length; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
	}
}

const struct SeaRandom_CLS SeaRandom = {
	.RandUint64 = RandUint64,
	.RandUint32 = RandUint32,
	.RandInt64 = RandInt64,
	.RandInt32 = RandInt32,
	.RandUint16 = RandUint16,
	.RandInt16 = RandInt16,
	.RandUint8 = RandUint8,
	.RandInt8 = RandInt8,
	.RandFloat = RandFloat,
	.RandDouble = RandDouble,
	.RandBool = RandBool,
	.FillRandomBytes = FillRandomBytes,
};