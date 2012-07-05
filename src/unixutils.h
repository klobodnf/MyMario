#ifdef __unix__
#ifdef __cplusplus
extern "C"
{
#endif
char *get_homedir(void);
int check_and_create_dir(const char *name);
#ifdef __cplusplus
}
#endif
#endif
