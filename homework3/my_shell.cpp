#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>

bool is_path(const std::string& path)
{
	if (std::find(path.begin(), path.end(), '/') != path.end())
		return true;
	return false;
}

std::string path_from_file(const std::string& file)
{
	std::string main_pathes = getenv("PATH");
	auto it1 = main_pathes.begin();
	auto it2 = std::find(it1, main_pathes.end(), ':');
	while (it2 != main_pathes.end())
	{
		std::string maybe = std::string(it1, it2)+"/"+file;
		if (access(maybe.c_str(), F_OK) != -1)
			return maybe;
		it1 = it2+1;
		it2 = std::find(it1, main_pathes.end(), ':');
	}
	return "";
}

int main()
{
	while (true)
	{
		std::cout << "Enter command" << std::endl;
		std::string input;
		if (!std::getline(std::cin, input))
		{
			break;
		}
		int pr_num = fork();
		if (pr_num < 0)
		{
			std::cerr << "error while creating child process\n";
			exit(-1);
		}
		if (pr_num > 0)
		{
			int status;
			wait(&status);
		}
		if (pr_num == 0)
		{
			std::string file_for_out = "/opt/silentshell/"+std::to_string(getpid())+"/out.std";
			std::string file_for_err = "/opt/silentshell/"+std::to_string(getpid()) + "/err.std";
			std::string file_for_in = "/opt/silentshell/"+std::to_string(getpid()) + "/in.std";
			int fd_for_out = open(file_for_out.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd_for_out < 0)
			{
				std::cerr << "error out\n";
				exit(-1);
			}
			int fd_for_err = open(file_for_err.c_str(), O_WRONLY | O_CREAT, S_IWUSR);
			if (fd_for_err < 0)
			{
				std::cerr << "error err\n";
				exit(-1);
			}
			int fd_for_in = open(file_for_in.c_str(), O_RDONLY | O_CREAT, S_IRUSR);
			if (fd_for_in < 0)
			{
				std::cerr << "error in\n";
				exit(-1);
			}
			int if_err_in = dup2(fd_for_in, 0);
			if (if_err_in < 0)
			{
				exit(-1);
			}
			int if_err_out = dup2(fd_for_out, 1);
			if (if_err_out < 0)
			{
				exit(-1);
			}
			int if_err_err = dup2(fd_for_err, 2);
			if (if_err_err < 0)
			{
				exit(-1);
			}
			std::stringstream ss(input);
			std::string first_arg;
			ss >> first_arg;
			bool path_or_file = is_path(first_arg);
			std::vector<std::string> v_args;
			std::string if_path;
			if (path_or_file)
				v_args.push_back(first_arg);
			else
			{
				if_path = path_from_file(first_arg);
				if (if_path == "")
				{
					std::cerr << "no such file" << std::endl;
					exit(-1);
				}
				v_args.push_back(if_path);
			}
			while (ss)
			{
				std::string tmp;
				ss >> tmp;
				v_args.push_back(std::move(tmp));
			}
			v_args.pop_back();
			char** args = new char*[v_args.size()+1];
			size_t i=0;
			for (i; i < v_args.size(); ++i)
			{
				args[i] = new char[v_args[i].size()]{};
				for (size_t j=0; j < v_args[i].size(); ++j)
					args[i][j] = v_args[i][j];
			}
			args[i] = NULL;
			if (path_or_file)
			{
				const char* path = first_arg.c_str();
				int if_er = execvp(path, args);
				if (if_er < 0)
				{
					std::cerr << "Error" << std::endl;
				}
			}
			else
			{
				const char* file = if_path.c_str();
				int if_er = execvp(file, args);
				if (if_er < 0)
				{
					std::cerr << "Error" << std::endl;
				}
			}
			for (size_t j=0; j < v_args.size(); ++j)
				delete[] args[j];
			delete[] args;
			return 0;
		}
	}

	return 0;
}
