#ifndef FT_PRINTF_H
#define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_flags
{
    int dot;
    int precision_star;
	int	width;
	int	minus;
	int	zero;
	//char zero; //либо пробел, либо ноль, по умолчанию пробел
	int	precision;
	int int_minus;
    int str_len; //длина строки
}				t_flags;

int	ft_printf(const char *str, ...);
void ft_putchar(char c, t_flags *flags);

#endif
