#define ATOI_ERROR_CODE 311223344

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define TO_LOWER(x) (((x)>='A' && (x)<='Z') ? (x) - 'A' + 'a' : (x))

// todo: do we even need? does 'strlen' just work like we use it in cli_registry?
uint32_t efiStrlen(const char *param);

int indexOf(const char *string, char ch);
bool strEqualCaseInsensitive(const char *str1, const char *str2);
bool strEqual(const char *str1, const char *str2);
int atoi(const char *string);
float atoff(const char *string);

#ifdef __cplusplus
}
#endif /* __cplusplus */
