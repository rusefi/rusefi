// Shared fallback bodies. Include in the Windows caller or the ELF defaults TU.
PUBLIC_API_WEAK __attribute__((noinline)) bool toothProfileStorageWrite() {
	return true;
}

PUBLIC_API_WEAK __attribute__((noinline)) bool toothProfileStorageRead() {
	return true;
}

PUBLIC_API_WEAK __attribute__((noinline)) bool vrModelStorageWrite() {
	return true;
}

PUBLIC_API_WEAK __attribute__((noinline)) bool vrModelStorageRead() {
	return true;
}
