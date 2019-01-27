/* pe15-1.c */
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
// C99 -- otherwise use int
// for CHAR_BIT
// for strchr()
int bstr_to_dec(const char *str);
bool check_val(const char *str);
char *s_gets(char *st, int n);
int main(void)
{
    const size_t SLEN = CHAR_BIT * sizeof(int) + 1;
    char value[SLEN];
    printf("Enter a binary number with up to %zu digits: ", SLEN - 1);
    while (s_gets(value, SLEN) && value[0] != '\0')
    {
        if (!check_val(value))
            puts("A binary number contains just 0s and 1s.");
        else
            printf("%s is %d\n", value, bstr_to_dec(value));
        puts("Enter next value (empty line to quit):");
    }
    puts("Done");
    return 0;
}
int bstr_to_dec(const char *str)
{
    int val = 0;
    while (*str != '\0')
        val = 2 * val + (*str++ - '0');
    return val;
}
bool check_val(const char *str)
{
    bool valid = true;
    while (valid && *str != '\0')
    {
        if (*str != '0' && *str != '1')
            valid = false;
        ++str;
    }
    return valid;
}
char *s_gets(char *st, int n)
{
    char *ret_val;
    char *find;

    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');
        //
        if (find)
            //
            *find = '\0';
        //
        else
            while (getchar() != '\n')
                continue;
        //
    }
    return ret_val;
}