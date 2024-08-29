#include "libft.h"

static int	count_words(const char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (ft_isspace(*str))
		{
			in_word = 0;
		}
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char	*copy_word(const char *start, const char *end)
{
	size_t	len;
	char	*word;

	len = end - start;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

char	**ft_split_space(char *str)
{
	int		word_count;
	char	**result;
	int		index;
	char	*start;

	word_count = count_words(str);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	index = 0;
	while (*str)
	{
		while (*str && ft_isspace(*str))
			str++;
		if (*str)
		{
			start = str;
			while (*str && !ft_isspace(*str))
				str++;
			result[index++] = copy_word(start, str);
		}
	}
	result[index] = NULL;
	return (result);
}
