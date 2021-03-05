
#include "compressed_block_device.h"

#include <cstring>

#define BLOCK_SIZE 512

static bool is_inserted(void*) {
	// Device is always inserted
	return true;
}

static bool is_protected(void*) {
	// Write protected - we can't do random access writes to a compressed volume
	return true;
}

static bool connect(void* instance) {
  CompressedBlockDevice* cbd = reinterpret_cast<CompressedBlockDevice*>(instance);
  if (BLK_STOP == cbd->state) {
    cbd->state = BLK_READY;
  }
  return HAL_SUCCESS;
}

static bool disconnect(void* instance) {
	CompressedBlockDevice* cbd = reinterpret_cast<CompressedBlockDevice*>(instance);
  if (BLK_STOP != cbd->state) {
    cbd->state = BLK_STOP;
  }
  return HAL_SUCCESS;
}

static bool read(void* instance, uint32_t startblk, uint8_t* buffer, uint32_t n) {
	CompressedBlockDevice* cbd = reinterpret_cast<CompressedBlockDevice*>(instance);
	if (startblk == cbd->lastBlock) {
		return HAL_SUCCESS;
	}

	// If we just initialized, or trying to seek backwards, (re)initialize the decompressor
	if (cbd->lastBlock == -1 || startblk < cbd->lastBlock) {
		uzlib_uncompress_init(&cbd->d, cbd->dictionary, sizeof(cbd->dictionary));

		cbd->d.source = cbd->source;
		cbd->d.source_limit = cbd->d.source + cbd->sourceSize;
		cbd->d.source_read_cb = NULL;

		uzlib_gzip_parse_header(&cbd->d);

		cbd->lastBlock = -1;
	}

	// How many blocks do we need to decompress to get to the one requested?
	size_t blocks_ahead = startblk - cbd->lastBlock;

	for (size_t i = 0; i < blocks_ahead; i++) {
		cbd->d.dest = cbd->d.dest_start = buffer;
		cbd->d.dest_limit = buffer + BLOCK_SIZE;

		// Decompress one chunk
		uzlib_uncompress(&cbd->d);
	}

	// Save the current position in the stream so we can efficiently seek forward later
	cbd->lastBlock = startblk;

	return HAL_SUCCESS;
}

static bool write(void*, uint32_t, const uint8_t*, uint32_t) {
	// you shouldn't be able to do this anyway, so just swallow it, I guess?
	return HAL_SUCCESS;
}

constexpr size_t gzSize(const uint8_t* image, size_t imageSize) {
	// The last 4 bytes of the gzip stream encode the total size in bytes
	const uint8_t* pSize = image + imageSize - 1;
	size_t size = *pSize--;
	size = 256 * size + *pSize--;
	size = 256 * size + *pSize--;
	return 256 * size + *pSize--;
}

static bool get_info(void* instance, BlockDeviceInfo* bdip) {
	CompressedBlockDevice* cbd = reinterpret_cast<CompressedBlockDevice*>(instance);
	if (cbd->state != BLK_READY) {
		return HAL_FAILED;
	}

	// The last 4 bytes of the gzip stream encode the total size in bytes
	size_t size = gzSize(cbd->source, cbd->sourceSize);

	bdip->blk_num = size / BLOCK_SIZE;
	bdip->blk_size = BLOCK_SIZE;
	return HAL_SUCCESS;
}

static bool sync(void* instance) {
	CompressedBlockDevice* cbd = reinterpret_cast<CompressedBlockDevice*>(instance);
  if (BLK_READY != cbd->state) {
    return HAL_FAILED;
  }
  else {
    return HAL_SUCCESS;
  }
}

static const BaseBlockDeviceVMT cbdVmt = {
	(size_t)0, // instanceOffset
	is_inserted,
	is_protected,
	connect,
	disconnect,
	read,
	write,
	sync,
	get_info,
};

void compressedBlockDeviceObjectInit(CompressedBlockDevice* cbd) {
	cbd->vmt = &cbdVmt;
	memset(cbd->dictionary, 0, sizeof(cbd->dictionary));
	cbd->state = BLK_STOP;
}

void compressedBlockDeviceStart(CompressedBlockDevice* cbd, const uint8_t* source, size_t sourceSize) {
	cbd->source = source;
	cbd->sourceSize = sourceSize;
	cbd->state = BLK_READY;
}
