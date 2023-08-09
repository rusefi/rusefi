#define ATOI_ERROR_CODE 311223344

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int indexOf(const char *string, char ch);
bool strEqualCaseInsensitive(const char *str1, const char *str2);
bool strEqual(const char *str1, const char *str2);
int atoi(const char *string);
float atoff(const char *string);

#ifdef __cplusplus
}
#endif /* __cplusplus */
