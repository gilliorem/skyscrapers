#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_hint
{
	int *col_top;
	int *col_bot;
	int *row_left;
	int *row_right;
}t_hint;

int	check_arg(int argc, char *argv[])
{
	char err_size[] = "error. usage: ./prog <\"col1top col2top col3top col4top col1bottom col2bottom col3bottom col4bottom\
			  row1left row2left row3left row4left row1right row2right row3right row4right\">";
	char err_number[] = "Second argument has to be a digit between 1 and 4 follow by a space";
	if (argc != 2)
	{
		write(2, err_size, strlen(err_size));
		return  0;
	}
	int i = 0;
	while (argv[1][i])
	{
		if (i % 2 != 0 && argv[1][i] != ' ')
		{
			write(2, err_number, strlen(err_number));
			return 0;
		}
		else if (i % 2 == 0 && (argv[1][i] < '1' || argv[1][i] > '4'))
		{
			write(2, err_number, strlen(err_number));
			return 0;
		}
		i++;
	}
	if (i != 31)
	{
		write(2, err_number, strlen(err_number));
		return 0;
	}
	return 1;
}

int	*parse_argv(char *argv_one)
{
	int	*hint_arr = (int *)calloc(16, sizeof(int));
	int	j = 0;
	//for (int i = 0; i < 32; i+=2)
	for (int i = 0; argv_one[i]; i+=2)
	{
		hint_arr[j] = argv_one[i] - '0';
		j++;
	}
	return hint_arr;
}

int	**create_hint_grid(int *hint_arr)
{
	int	**hint_grid = (int **)calloc(4, sizeof(int *));
	
	int k = 0;	
	for (int j = 0; j < 4; j++)
	{
		hint_grid[j] = (int *)calloc(4, sizeof(int ));
		for (int i = 0; i < 4; i++)
		{
			hint_grid[j][i] = hint_arr[k];
			k++;
		}	
	}
	return hint_grid;
}

int	**create_solved_grid(void)
{
	int	**solved_grid = (int **)calloc(4, sizeof(int *));

	for (int j = 0; j < 4; j++)
	{
		solved_grid[j] = (int *)calloc(4, sizeof(int ));
	}
	return solved_grid;
}

void	solve_grid_one(int **grid, t_hint *hint)
{
	for (int k = 0; k < 4; k++)
	{
		if (hint->col_top[k] == 1)
			grid[0][k] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->col_bot[k] == 1)
			grid[3][k] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->row_right[k] == 1)
			grid[k][0] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->row_right[k] == 1)
			grid[k][3] = 4;
	}
}

void	solve_grid_four(int **grid, t_hint *hint)
{
	for (int k = 0; k < 4; k++)
	{
		if (hint->col_top[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[i][k] = i + 1;
			}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->col_bot[k] == 4)
		{
			for (int i = 3; i >= 0; i--)
			{
				grid[i][k] = (4 - i);
			}
		}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->row_left[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[k][i] = i + 1;
			}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->row_right[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[k][i] = 4 - i;
			}
	}
}

void	solve_grid(int **grid, t_hint *hint)
{
	solve_grid_one(grid, hint);
	solve_grid_four(grid, hint);
}

void	print_grid(int **grid)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			printf("%d ", grid[j][i]);
		}
		printf("\n");	
	}
}

t_hint 	*init_hint(void)
{
	t_hint *hint = calloc(1, sizeof(t_hint));
	hint->col_top = calloc(4, sizeof(int));
	hint->col_bot = calloc(4, sizeof(int));
	hint->row_left = calloc(4, sizeof(int));
	hint->row_right = calloc(4, sizeof(int));

	hint->col_top[0] = 2;
	hint->col_top[1] = 2;
	hint->col_top[2] = 2;
	hint->col_top[3] = 2;

	hint->col_bot[0] = 3;
	hint->col_bot[1] = 2;
	hint->col_bot[2] = 2;
	hint->col_bot[3] = 2;

	hint->row_left[0] = 2;
	hint->row_left[1] = 2;
	hint->row_left[2] = 2;
	hint->row_left[3] = 2;

	hint->row_right[0] = 3;
	hint->row_right[1] = 2;
	hint->row_right[2] = 3;
	hint->row_right[3] = 4;

	return hint;
}

int	main(int argc, char *argv[])
{
	int	**solved_grid;
	t_hint	*hint = init_hint();

	if (!check_arg(argc, argv))
		return 0;
	printf("SOLVED GRID:\n");
	solved_grid = create_solved_grid();
	print_grid(solved_grid);
	solve_grid(solved_grid, hint);
	printf("PRE FILED SOLVED GRID:\n");
	print_grid(solved_grid);

}
