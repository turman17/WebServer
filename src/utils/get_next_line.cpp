#include "webserv.hpp"


static int			line_updater(std::string*& line, char buffer[]);
static std::string*	ft_strjoinm(const std::string& s1, const std::string& s2);
static void			buffer_clear(char *buffer);
static void			free_line(std::string *line);

std::string*	getNextLine(const FileDescriptor& fd, char buffer[gnl::BUFFER_SIZE + 1])
{
	std::string*	line = NULL;
	int				flag;
	int				bytesRead;

	if (gnl::BUFFER_SIZE <= 0 || fd.badFileDescriptor())
		throw BadFileDescriptor();
	while (1)
	{
		if (buffer[0] == '\0')
		{
			bytesRead = read(fd, buffer, gnl::BUFFER_SIZE);
			if (bytesRead == -1) {
				free_line(line);
				throw BadRead();
			}
			else if (bytesRead == 0)
				return (line);
		}
		flag = line_updater(line, buffer);
		buffer_clear(buffer);
		if (flag == 1)
			return (line);
	}
}

static void	free_line(std::string *line)
{
	if (line)
		delete(line);
}

static int	line_updater(std::string*& line, char buffer[])
{
	try {
		std::string* join = ft_strjoinm(line ? *line : "", buffer);
		if (line)
			delete(line);
		line = join;
		if (line->find('\n') != std::string::npos)
			return (1);
		return (0);
	}
	catch (const std::exception&) {
		if (line)
			delete(line);
		throw;
	}
}

static std::string*	ft_strjoinm(const std::string& s1, const std::string& s2)
{
	std::string *s3;
	int			i;
	
	s3 = new std::string;
	i = 0;
	while (s1[i] != '\0')
		*s3 += s1[i++];
	i = 0;
	while (s2[i] != '\0')
	{
		*s3 += s2[i++];
		if (s2[i - 1] == '\n')
			break ;
	}
	return (s3);
}

static void	buffer_clear(char *buffer)
{
	int	j;
	int	i;

	i = 0;
	j = -1;
	while (buffer[i])
	{
		if (j != -1)
			buffer[j++] = buffer[i];
		else if (j == -1 && buffer[i] == '\n')
			j = 0;
		buffer[i++] = '\0';
	}
}
