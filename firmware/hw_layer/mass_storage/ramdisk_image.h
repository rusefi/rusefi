// This file will be replaced by a ramdisk image containing the corresponding ini
// Defining this macro tells the ramdisk to instead mount a null device instead of
// the filesystem image, since we don't have one.
static const unsigned char ramdisk_image[] = {
  // sad hack to make mre-legacy_f4 happy
};
