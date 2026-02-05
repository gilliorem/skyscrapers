#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_hint
{
	int *hint_arr;
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

void	parse_argv(char *argv_one, t_hint *hint)
{
	int	j = 0;
	//for (int i = 0; i < 32; i+=2)
	for (int i = 0; argv_one[i]; i+=2)
	{
		hint->hint_arr[j] = argv_one[i] - '0';
		j++;
	}
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

//  should I create an object grid with
//  	we are going to check in the same order as the hints.
//	coll1top; col2top; col3top; col4top
//		colbot->no need (for same number)
//	row1left
//	row2left
//	row3left
//	row4left

// 		we can take the k out because each time we compare 
// 		as we go down row we dont want to repeat comparaison
// 			we dont need to do the last one as we already compare
// 			is it the same loop that bubble sort ?

//		yep I got the logic wrongly: I mixed row and col.

bool	same_n_row(int **grid, int row)
{
	int k = 1;
	for (int col = 0; col < 4; col++)
	{
		k = col + k;
		if (k  > 3)
			break;
//		printf("col: %d\n", col);
//		printf("k: %d\n", k);
//		printf("cell %d\n", grid[col][col]);
		while (k < 4)
		{
			if (grid[row][col] == grid[row][k])
			{
				printf("same number in col %d and col %d\n", col, k);
			}
			k++;
		}
		k = 1;
	}
	return true;
}

// the constraints: 
// 	the same number cannnot be place twice in the same col/row

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
	
	hint->hint_arr = (int *)calloc(16, sizeof(int));

	hint->col_top = calloc(4, sizeof(int));
	hint->col_bot = calloc(4, sizeof(int));
	hint->row_left = calloc(4, sizeof(int));
	hint->row_right = calloc(4, sizeof(int));

	return hint;
}

void	fill_hint_arr(t_hint *hint)
{
	for (int i = 0; i < 4; i++)
	{
		hint->col_top[i] = hint->hint_arr[i];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->col_bot[i] = hint->hint_arr[i+4];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->row_left[i] = hint->hint_arr[i+8];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->row_right[i] = hint->hint_arr[i+12];
	}
}

int	main(int argc, char *argv[])
{
	int	**solved_grid;
	if (!check_arg(argc, argv))
		return 0;
	t_hint	*hint = init_hint();
	parse_argv(argv[1], hint);
	fill_hint_arr(hint);
	printf("SOLVED GRID:\n");
	solved_grid = create_solved_grid();
	print_grid(solved_grid);
	solve_grid(solved_grid, hint);
	printf("PRE FILED SOLVED GRID:\n");
	print_grid(solved_grid);
	same_n_row(solved_grid, 1);
	printf("expected output:\ncol 0 and 3;\ncol 2 and 3;\n");

}
