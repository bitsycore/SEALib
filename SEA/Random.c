#include "Random.h"

#include "Compat/ThreadCompat.h"

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

static uint64_t rotateLeft(const uint64_t x, const int k) {
	return (x << k) | (x >> (64 - k));
}

static void initSeed256(uint64_t seed[4]) {
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

// =======================================
// MARK: Public
// =======================================

static uint64_t Random_Uint64() {
	if (SEED_INITIALIZED == false) {
		SEED_INITIALIZED = true;
		initSeed256(SEED);
	}

	const uint64_t result = rotateLeft(SEED[0] + SEED[3], 23) + SEED[0];

	const uint64_t t = SEED[1] << 17;

	SEED[2] ^= SEED[0];
	SEED[3] ^= SEED[1];
	SEED[1] ^= SEED[2];
	SEED[0] ^= SEED[3];

	SEED[2] ^= t;

	SEED[3] = rotateLeft(SEED[3], 45);

	return result;
}

static uint32_t Random_Uint32() {
	return (uint32_t) (Random_Uint64() >> 32);
}

static int64_t Random_Int64() {
	return (int64_t) Random_Uint64();
}

static int32_t Random_Int32() {
	return (int32_t) Random_Uint32();
}

static uint16_t Random_Uint16() {
	return (uint16_t) (Random_Uint64() & 0xFFFF);
}

static int16_t Random_Int16() {
	return (int16_t) (Random_Uint64() & 0xFFFF);
}

static uint8_t Random_Uint8() {
	return (uint8_t) (Random_Uint64() & 0xFF);
}

static int8_t Random_Int8() {
	return (int8_t) (Random_Uint64() & 0xFF);
}

static float Random_Float() {
	return ((float) (Random_Uint64() >> 40)) / 16777216.0f; // 2^24
}

static double Random_Double() {
	return ((double) (Random_Uint64() >> 11)) / 9007199254740992.0; // 2^53
}

static bool Random_Bool() {
	return (Random_Uint64() >> 63) != 0;
}

static void Random_Bytes(uint8_t* buffer, size_t length) {
	while (length >= 8) {
		uint64_t r = Random_Uint64();
		for (int i = 0; i < 8; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
		buffer += 8;
		length -= 8;
	}

	if (length > 0) {
		uint64_t r = Random_Uint64();
		for (size_t i = 0; i < length; ++i) {
			buffer[i] = (uint8_t) (r & 0xFF);
			r >>= 8;
		}
	}
}

const struct SEA_Random_CLS SEA_Random = {
	.Uint64 = Random_Uint64,
	.Uint32 = Random_Uint32,
	.Int64 = Random_Int64,
	.Int32 = Random_Int32,
	.Uint16 = Random_Uint16,
	.Int16 = Random_Int16,
	.Uint8 = Random_Uint8,
	.Int8 = Random_Int8,
	.Float = Random_Float,
	.Double = Random_Double,
	.Bool = Random_Bool,
	.Bytes = Random_Bytes,
};