int errno;

void __assert_func (const char *file, int line, const char *func, const char *failedexpr) {
}

int *__errno () {
  return &errno;
}
