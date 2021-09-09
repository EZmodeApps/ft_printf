#include "ft_printf.h"
#include <stdio.h>

void ft_putchar(char c, t_flags *flags)
{
    write(1, &c, 1);
    flags->str_len++;
}

int	ft_strlen(const char *s)
{
    int	i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

int parse_width(const char *str, int index, t_flags *flags, va_list arg)
{
    int num;

    num = 0;
    if (str[index] >= '0' && str[index] <= '9')
    {
        while (str[index] >= '0' && str[index] <= '9')
        {
            num = (num * 10) + str[index] - '0';
            index++;
        }
        flags->width = num;
    }
    else
    {
        flags->width = va_arg(arg, int);
        index++;
    }
    if (flags->width < 0)
    {
        flags->minus = 1;
        flags->zero = 0;
        flags->width *= -1;
    }
    return (index);
}

int parse_precision(const char *str, int index, t_flags *flags, va_list arg)
{
    int num;

    num = 0;
    if (str[index] >= '0' && str[index] <= '9')
    {
        while (str[index] >= '0' && str[index] <= '9')
        {
            num = (num * 10) + str[index] - '0';
            index++;
        }
        flags->precision = num;
    }
    else
    {
    	if (str[index] == '*')
    		flags->precision_star = 1;
        flags->precision = va_arg(arg, int);
        index++;
    }
    return (index);
}

void ft_putstr(char *str, t_flags *flags)
{
    int i;

    i = 0;
    while (str[i])
    {
        write (1, &str[i], 1);
        i++;
        flags->str_len += 1;
    }
}

void ft_putnstr(char *str, int n, t_flags *flag)
{
    int i;

    i = 0;
    while (i < n)
    {
        write(1, &str[i], 1);
        flag->str_len += 1;
        i++;
    }
}

void ft_putnchar(char c, int n, t_flags *flag)
{
    int i;

    i = 0;
    while (i < n)
    {
        write(1, &c, 1);
        flag->str_len += 1;
        i++;
    }
}

void str_process(t_flags *flag, va_list arg)
{
    char *s;

    s = va_arg(arg, char *);
    if (!s)
        s = "(null)";
    if (flag->precision < 0 || ft_strlen(s) < flag->precision)
        flag->precision = ft_strlen(s);
    if (flag->minus == 1)
    {
            ft_putnstr(s, flag->precision, &(*flag));
			ft_putnchar(' ', flag->width - flag->precision, &(*flag));
    }
    if (flag->minus == 0)
    {
        ft_putnchar(' ', flag->width - flag->precision, &(*flag));
        ft_putnstr(s, flag->precision, &(*flag));
    }
}

int parse_flags(const char *str, int index, t_flags *flags)
{
    if (str[index] == '0' || str[index] == '-')
    {
        while (str[index] == '0' || str[index] == '-')
        {
            if (str[index] == '-')
                flags->minus = 1;
            index++;
        }
        if (flags->minus == 0)
            flags->zero = 1;
    }
    return (index);
}

void    char_process(t_flags *flags, va_list arg)
{
    unsigned char c;

    c = (unsigned char)va_arg(arg, int);
    if (flags->minus == 0)
    {
        ft_putnchar(' ', flags->width - 1, &(*flags));
        ft_putchar(c, &(*flags));
    }
    else
    {
        ft_putchar(c, &(*flags));
        ft_putnchar(' ', flags->width - 1, &(*flags));
    }
}

void    percent_process(t_flags *flags)
{

    if (flags->minus == 0)
    {
        ft_putnchar(' ', flags->width - 1, &(*flags));
        ft_putchar('%', &(*flags));
    }
    else
    {
        ft_putchar('%', &(*flags));
        ft_putnchar(' ', flags->width - 1, &(*flags));
    }
}

int	ft_count_chars(int n)
{
    int		i;

    i = 0;
    if (n == 0)
        return (1);
    while (n)
    {
        n = n / 10;
        i++;
    }
    return (i);
}

char	*ft_strdup(const char *s1)
{
    int		i;
    int		length;
    char	*copy;

    i = 0;
    length = ft_strlen(s1);
    copy = (char *)malloc(sizeof(char) * (length + 1));
    if (copy)
    {
        while (s1[i])
        {
            copy[i] = s1[i];
            i++;
        }
        copy[i] = '\0';
        return (copy);
    }
    else
        return (NULL);
}

char	*ft_itoa(int n)
{
    char	*res;
    int		j;

    if (n <= -2147483648)
        return (ft_strdup("2147483648"));
    if (n < 0)
        n *= -1;
    j = ft_count_chars(n);
    res = (char *)malloc(sizeof(char) * (j + 1));
    if (!res)
        return (NULL);
    if (n == 0)
        res[0] = '0';
    res[j] = '\0';
    while (n)
    {
        res[j - 1] = (n % 10) + '0';
        n = n / 10;
        j--;
    }
    return (res);
}

char	*ft_itoa_uns(unsigned int n)
{
    char            *res;
    unsigned int    j;

    if (n >= 4294967295)
        return (ft_strdup("4294967295"));
    j = ft_count_chars(n);
    res = (char *)malloc(sizeof(char) * (j + 1));
    if (!res)
        return (NULL);
    if (n == 0)
        res[0] = '0';
    res[j] = '\0';
    while (n)
    {
        res[j - 1] = (n % 10) + '0';
        n = n / 10;
        j--;
    }
    return (res);
}

void parse_int(int number, char *num, t_flags *flags)
{
    if (flags-> precision < 0 && flags->zero == 1)
    {
        flags->precision = flags->width - flags->int_minus;
        flags->width = 0;
    }
    if (ft_strlen(num) > flags->precision && flags->dot == 0)
        flags->precision = ft_strlen(num);
    if (flags->dot == 1 && flags->precision == 0 && number == 0)
        return ;
    if (ft_strlen(num) > flags->precision)
        flags->precision = ft_strlen(num);
    if (flags->minus == 1)
    {
        if (flags->int_minus == 1)
            ft_putchar('-', &(*flags));
        ft_putnchar('0', flags->precision - ft_strlen(num), &(*flags));
        ft_putstr(num, &(*flags));
        ft_putnchar(' ', flags->width - flags->precision - flags->int_minus, &(*flags));
    }
    if (flags->minus == 0)
    {
        ft_putnchar(' ', flags->width - flags->precision - flags->int_minus, &(*flags));
        if (flags->int_minus == 1)
            ft_putchar('-', &(*flags));
        ft_putnchar('0', flags->precision - ft_strlen(num), &(*flags));
        ft_putstr(num, &(*flags));
    }
}

void int_process(t_flags *flags, va_list arg)
{
    int number;
    char *num;

    number = va_arg(arg, int);
    if (flags->precision == 0 && number == 0)
        ft_putnchar(' ', flags->width, &(*flags));
    if (number < 0)
        flags->int_minus = 1;
	num  = ft_itoa(number);
    parse_int(number, num, &(*flags));
    free (num);
}

//void parse_unsigned_int(unsigned int number, char *num, t_flags *flags)
void parse_unsigned_int(char *num, t_flags *flags)
{
    if (ft_strlen(num) > flags->precision) // && flags->dot == 0)
        flags->precision = ft_strlen(num);
    if (flags->minus == 1)
    {
        ft_putnchar('0', flags->precision - ft_strlen(num), &(*flags));
        ft_putstr(num, &(*flags));
        ft_putnchar(' ', flags->width - flags->precision, &(*flags));
    }
    if (flags->minus == 0)
    {
        ft_putnchar(' ', flags->width - flags->precision, &(*flags));
        ft_putnchar('0', flags->precision - ft_strlen(num), &(*flags));
        ft_putstr(num, &(*flags));
    }
}

void unsigned_int_process(t_flags *flags, va_list arg)
{
    unsigned int number;
    char *num;

    number = va_arg(arg, unsigned int);
    if (flags->precision == 0 && number == 0)
    {
        ft_putnchar(' ', flags->width, &(*flags));
        return;
    }
    if (flags->zero == 1 && flags->precision == 0)
    {
        flags->precision = flags->width;
        flags->width = 0;
    }
    if (flags->dot == 1 && flags->precision == 0)
        return ;
    num  = ft_itoa_uns(number);
    parse_unsigned_int(num, &(*flags));
    free (num);
}

int	pointer_len(unsigned long p)
{
	int		i;

	i = 0;
	while (p)
	{
		p = p / 16;
		i++;
	}
	return (i);
}

char	*ft_itoa_hex(unsigned long n, int x_type)
{
    char            *res;
    unsigned int    j;

    j = pointer_len(n);
    res = (char *)malloc(sizeof(char) * (j + 1));
    if (!res)
        return (NULL);
    if (n == 0)
        res[0] = '0';
    res[j] = '\0';
    while (n)
    {
        if (n % 16 < 10)
            res[j - 1] = (n % 16) + 48;
        else
        {
            if (x_type == 0)
                res[j - 1] = (n % 16) + 87;
            if (x_type == 1)
                res[j - 1] = (n % 16) + 55;
        }
        n = n / 16;
        j--;
    }
    return (res);
}


//int	hex_small_register(unsigned long p, t_flags *flags)
//{
//    if (p > 15)
//        hex_small_register(p / 16, &(*flags));
//    if ((p % 16) >= 0 && (p % 16) <= 9)
//        ft_putchar((p % 16) + '0', &(*flags));
//    if ((p % 16) == 10)
//        ft_putchar('a', &(*flags));
//    if ((p % 16) == 11)
//        ft_putchar('b', &(*flags));
//    if ((p % 16) == 12)
//        ft_putchar('c', &(*flags));
//    if ((p % 16) == 13)
//        ft_putchar('d', &(*flags));
//    if ((p % 16) == 14)
//        ft_putchar('e', &(*flags));
//    if ((p % 16) == 15)
//        ft_putchar('f', &(*flags));
//}

void parse_pointer(char *pointer, t_flags *flags)
{
	int len;

	len = ft_strlen(pointer);
	if (flags->minus == 1)
	{
		ft_putstr("0x", &(*flags));
		ft_putstr(pointer, &(*flags));
		ft_putnchar(' ', flags->width - (len + 2), &(*flags));
	}
	else
	{
		ft_putnchar(' ', flags->width - (len + 2), &(*flags));
		ft_putstr("0x", &(*flags));
		ft_putstr(pointer, &(*flags));
	}
}

void pointer_process(t_flags *flags, int x_type, va_list arg)
{
    unsigned long p;
    char *pointer;

    p = (unsigned long)va_arg(arg, void *);
	if (p == 0 && flags->precision == 0)
	{
		ft_putstr("\0", &(*flags));
		return ;
	}
	if (p == 0)
	{
		pointer = "0";
		parse_pointer(pointer, &(*flags));
		return ;
	}
    //hex_small_register(p, &(*flags));
	pointer = ft_itoa_hex(p, x_type);
	parse_pointer(pointer, &(*flags));
    free (pointer);
}

void parse_hex(char *hex, t_flags *flags)
{
    int len;

    len = ft_strlen(hex);
    if (flags-> precision < 0 && flags->zero == 1)
    {
        flags->precision = flags->width - flags->int_minus;
        flags->width = 0;
    }
    if (len > flags->precision)
        flags->precision = len;
    if (flags->minus == 1)
    {
        ft_putnchar('0', flags->precision - len, &(*flags));
        ft_putstr(hex, &(*flags));
        ft_putnchar(' ', flags->width - flags->precision, &(*flags));
    }
    if (flags->minus == 0)
    {
        ft_putnchar(' ', flags->width - len, &(*flags));
        ft_putnchar('0', flags->precision - len, &(*flags));
        ft_putstr(hex, &(*flags));
    }
}

void hex_process(t_flags *flags, int x_type, va_list arg)
{
    unsigned int x;
    char *hex;

    x = (unsigned int)va_arg(arg, unsigned int);
    if (x == 0 && flags->precision == 0)
    {
        ft_putstr("\0", &(*flags));
        return ;
    }
    if (x == 0)
    {
        hex = "0";
        parse_hex(hex, &(*flags));
        return ;
    }
    //hex_small_register(p, &(*flags));
    hex = ft_itoa_hex(x, x_type);
    parse_hex(hex, &(*flags));
    free (hex);
}

void what_type(char str, t_flags *flag, va_list arg)
{
    int x_type;

    x_type = 0;
    if (str == 'c')
        char_process(&(*flag), arg);
    if (str == 's')
        str_process(&(*flag), arg);
    if (str == 'd' || str == 'i')
        int_process(&(*flag), arg);
    if (str == '%')
        percent_process(&(*flag));
    if (str == 'p')
        pointer_process(&(*flag), x_type, arg);
    if (str == 'u')
        unsigned_int_process(&(*flag), arg);
    if (str == 'x')
        hex_process(&(*flag), x_type, arg);
    if (str == 'X')
    {
        x_type = 1;
        hex_process(&(*flag), x_type, arg);
    }
}

int parse_str(const char *str, int index, t_flags *flag, va_list arg)
{
    index++;
    if (str[index] == '-' || str[index] == '0')
        index = parse_flags(str, index, &(*flag));
    if ((str[index] >= '0' && str[index] <= '9') || str[index] == '*')
        index = parse_width(str, index, &(*flag), arg);
    if (str[index] == '.')
    {
        flag->dot = 1;
        index++;
        if ((str[index] >= '0' && str[index] <= '9') || str[index] == '*')
            index = parse_precision(str, index, &(*flag), arg);
        else
        	flag->precision = 0;
    }
    what_type(str[index], &(*flag), arg);
    return (index);
}


void struct_init(t_flags *flags)
{
    flags->dot = 0;
    flags->precision_star = 0;
	flags->minus = 0;
	flags->zero = 0;
	flags->width = 0;
	flags->precision = -1;
	flags->int_minus = 0;
}

int	ft_printf(const char *str, ...)
{
	int i;
	t_flags flags;
    va_list arg;

	i = 0;
    va_start(arg, str);
	flags.str_len = 0;
	while (str[i])
	{
        if (str[i] == '%')
        {
            struct_init(&flags);
            i = parse_str(str, i, &flags, arg);
//            printf("\nwdith: %d\n", flags.width);
//            printf("pre: %d\n", flags.precision);
//            printf("dot: %d\n", flags.dot);
//            printf("minus: %d\n", flags.minus);
//            printf("zero: %d\n", flags.zero);
        }
        else
            ft_putchar(str[i], &flags);
		i++;
	}
    va_end(arg);
	return (flags.str_len);
}

//int main()
//{
//	char *s1 = "hello fucking c";
//	char z = 'a';
//	int d = -2147483648;
//	unsigned int ii = 789567;
//	unsigned int uns = 4294967295;
//    unsigned int unss = 4242;
//    char *p_c = "i hate c";
//   // int *p_i = 6666666;
//    char *p_null = NULL;
//    int a;
//    int b;
//
//	int a = printf("ORIGINAL: he %40.20i %s hello\n", d, s1);
//	int b = ft_printf("MY      : he %40.20i %s hello\n", d, s1);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);

//	a = printf("ORIGINAL: he %12.5s hello\n", s1);
//	b = ft_printf("MY      : he %12.5s hello\n", s1);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %.s hello\n", s1);
//	b = ft_printf("MY      : he %.s hello\n", s1);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %25.2s hello\n", s1);
//	b = ft_printf("MY      : he %25.2s hello\n", s1);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %25.s hello\n", s1);
//	b = ft_printf("MY      : he %25.s hello\n", s1);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %25.2s hello\n", s1);
//    b = ft_printf("MY      : he %25.2s hello\n", s1);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %25.s hello\n", s1);
//    b = ft_printf("MY      : he %25.s hello\n", s1);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %.d hello\n", 0);
//    b = ft_printf("MY      : he %.d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %010d hello\n", -1234);
//    b = ft_printf("MY      : he %010d hello\n", -1234);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %0.d hello\n", -12345);
//    b = ft_printf("MY      : he %0.d hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-23.d hello\n", 0);
//    b = ft_printf("MY      : he %-23.d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-7d hello\n", 0);
//    b = ft_printf("MY      : he %-7d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %10d hello\n", 0);
//    b = ft_printf("MY      : he %10d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-9d hello\n", -1234);
//    b = ft_printf("MY      : he %-9d hello\n", -1234);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %.7d hello\n", -12345);
//    b = ft_printf("MY      : he %.7d hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-.d hello\n", 0);
//    b = ft_printf("MY      : he %-.d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-10.4d hello\n", 0);
//    b = ft_printf("MY      : he %-10.4d hello\n", 0);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %*.*% hello\n", 10, 5);
//    b = ft_printf("MY      : he %*.*% hello\n", 10, 5);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %u hello\n", -4294967295);
//    b = ft_printf("MY      : he %u hello\n", -4294967295);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-10.89u hello\n", unss);
//    b = ft_printf("MY      : he %-10.89u hello\n", unss);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-.u hello\n", unss);
//    b = ft_printf("MY      : he %-.u hello\n", unss);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %p hello\n", p_c);
//    b = ft_printf("MY      : he %p hello\n", p_c);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %p %s %d %c %% hello\n", p_i, s1, d, z);
//    b = ft_printf("MY      : he %p %s %d %c %% hello\n", p_i, s1, d, z);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %p hello\n", p_null);
//    b = ft_printf("MY      : he %p hello\n", p_null);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-111p hello\n", p_c);
//    b = ft_printf("MY      : he %-111p hello\n", p_c);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %111p hello\n", p_c);
//	b = ft_printf("MY      : he %111p hello\n", p_c);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %*p hello\n", 100, p_c);
//	b = ft_printf("MY      : he %*p hello\n", 100, p_c);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-87p hello\n", NULL);
//    b = ft_printf("MY      : he %-87p hello\n", NULL);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//	a = printf("ORIGINAL: he %79p hello\n", NULL);
//	b = ft_printf("MY      : he %79p hello\n", NULL);
//	printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %x hello\n",ii);
//    b = ft_printf("MY      : he %x hello\n", ii);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %79x hello\n", -12345);
//    b = ft_printf("MY      : he %79x hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %79X hello\n", -12345);
//    b = ft_printf("MY      : he %79X hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-79X hello\n", -12345);
//    b = ft_printf("MY      : he %-79X hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %80X hello\n", -12345);
//    b = ft_printf("MY      : he %80X hello\n", -12345);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %80X hello\n", NULL);
//    b = ft_printf("MY      : he %80X hello\n", NULL);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-80X hello\n", NULL);
//    b = ft_printf("MY      : he %-80X hello\n", NULL);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL: he %-80X hello\n", 4294967295);
//    b = ft_printf("MY      : he %-80X hello\n", 4294967295);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);

//    a = printf("ORIGINAL:  %4.3s %-4.3s \n", "123", "4567");
//    b = ft_printf("MY      :  %4.3s %-4.3s \n", "123", "4567");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL:  %4.2s %-4.2s \n", "123", "4567");
//    b = ft_printf("MY      :  %4.2s %-4.2s \n", "123", "4567");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf("ORIGINAL:  %-3.s\n", NULL);
//    b = ft_printf("MY      :  %-3.s\n", NULL);
//    printf("orlen: %d\nmylen: %d\n\n", a, b);

//   a = printf("%-9.1s%s",NULL, "1");
//    printf("\n");
//   b = ft_printf("%-9.1s%s", NULL, "1");
//    printf("\n");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);

//   a = printf(" *%-*.*d* *%*.*d* ", 6, 2, 102, 10, 21, -101);
//    printf("\n");
//   b = ft_printf(" *%-*.*d* *%*.*d* ", 6, 2, 102, 10, 21, -101);
//    printf("\n");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf(" %.u ", 0);
//    printf("\n");
//    b = ft_printf(" %.u ", 0);
//    printf("\n");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//
//    a = printf(" --0*%0*.0u*0 0*%0*.10u*0-- ", -21, 2147483647, 21, -2147483648);
//    printf("\n");
//    b = ft_printf(" --0*%0*.0u*0 0*%0*.10u*0-- ", -21, 2147483647, 21, -2147483648);
//    printf("\n");
//    printf("orlen: %d\nmylen: %d\n\n", a, b);
//}


//if (width > precision)
//{
//	if (precision > ft_strlen)
//	{
//		if (dot == 1 && precision == 0)
//		{
//
//		}
//		else if (dot == 1 && precision != 0)
//		{
//
//		}
//		else if (dot == 0)
//		{
//
//		}
//	}
//	else
//	{
//		if (dot == 1 && precision == 0)
//		{}
//		else if (dot == 1 && precision != 0)
//		{}
//		else if (dot == 0)
//	}
//	}
//else
//{
//	if(dot == 1 && pre != 0 && precision > ft_strlen && width > ft_strlen)
//}
//}

//x - unsigned int;
//pointer - unsigned long long;
