#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_hint
{
	int *hint_arr;
	int *top;
	int *bot;
	int *left;
	int *right;
}t_hint;

typedef struct s_combinaison
{
	int size;
	int tab[4];
}t_combinaison;

bool	same_n_row(int **grid, int row);
void	replace_same_row(int **grid, int n, int col, int row);
void	replace_same_col(int **grid, int n, int col, int row);
int	check_n_building(int tab[4]);

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
	for (int i = 0; i < 32; i+=2)
	{
		hint->hint_arr[j] = argv_one[i] - '0';
		j++;
	}
}

void	print_hint(t_hint *hint)
{
	for (int i = 0; i < 4; i++)
		printf("%d ",  hint->top[i]);
	printf("\n");
	for (int i = 0; i < 4; i++)
		printf("%d ",  hint->bot[i]);
	printf("\n");
	for (int i = 0; i < 4; i++)
		printf("%d ",  hint->left[i]);
	printf("\n");
	for (int i = 0; i < 4; i++)
		printf("%d ",  hint->right[i]);
	printf("\n");
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

void	solve_one(int **grid, t_hint *hint)
{
	for (int k = 0; k < 4; k++)
	{
		if (hint->top[k] == 1)
			grid[0][k] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->bot[k] == 1)
			grid[3][k] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->left[k] == 1)
			grid[k][0] = 4;
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->right[k] == 1)
			grid[k][3] = 4;
	}
}

void	solve_four(int **grid, t_hint *hint)
{
	for (int k = 0; k < 4; k++)
	{
		if (hint->top[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[i][k] = i + 1;
			}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->bot[k] == 4)
		{
			for (int i = 3; i >= 0; i--)
			{
				grid[i][k] = (4 - i);
			}
		}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->left[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[k][i] = i + 1;
			}
	}
	for (int k = 0; k < 4; k++)
	{
		if (hint->right[k] == 4)
			for (int i = 0; i < 4; i++)
			{
				grid[k][i] = 4 - i;
			}
	}
}

bool	is_same_n_row(int **grid, int row)
{
	int k = 1;
	for (int col = 0; col < 4; col++)
	{
		k = col + k;
		if (k  > 3)
			break;
		while (k < 4)
		{
			if (grid[row][col] == grid[row][k])
			{
				printf("same number in col %d and col %d\n", col, k);
				return true;
			}
			k++;
		}
		k = 1;
	}
	return false;
}

bool	is_same_n_col(int **grid, int col)
{
	int k = 1;
	for (int row = 0; row < 4; row++)
	{
		k = row + k;
		if (k  > 3)
			break;
		while (k < 4)
		{
			if (grid[col][row] == grid[k][col])
			{
				printf("same number in row %d and row %d\n", row, k);
				return true;
			}
			k++;
		}
		k = 1;
	}
	return false;
}

int	check_n_building(int tab[4])
{
	int buildings = 0;
	int max = 0;
	for (int i = 0; i < 4; i++)
	{
		if (tab[i] == 0)
		{
			return 0;
		}
		if (tab[i] > max)
		{
			max = tab[i];
			buildings++;
		}
	}
	return buildings;
}

bool	is_cell_free(int **grid, int try_n, int row, int col, t_hint *hint)
{
	if (try_n == 3)
	{
		if (row == 0)
		{
			if (hint->top[col] == 3)
				return false;
		}
		if (row == 3)
		{
			if (hint->bot[col] == 3)
				return false;
		}
		if (col == 0)
		{
			if (hint->left[row] == 3)
				return false;
		}
		if (col == 3)
		{
			if (hint->right[row] == 3)
				return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (grid[i][col] == try_n)
		{
			return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (grid[row][i] == try_n)
		{
			return false;
		}
	}
	int tab_test[4];
	for (int i=0;i<4;i++)
		tab_test[i] = grid[i][col];	
	tab_test[row] = try_n;
	int n = check_n_building(tab_test);
	if (n != hint->top[col] && n != 0)
		return false;

	for (int i = 0; i < 4; i++)
		tab_test[3 - i] = grid[i][col];	
	tab_test[3 - row] = try_n;
	n = check_n_building(tab_test);
	if (n != hint->bot[col] && n != 0)
		return false;

	for (int i=0; i<4; i++)
		tab_test[i] = grid[row][i];	
	tab_test[col] = try_n;
	n = check_n_building(tab_test);
	if (n != hint->left[row] && n != 0)
		return false;

	for (int i = 0; i < 4; i++)
		tab_test[3 - i] = grid[row][i];	
	tab_test[3 - col] = try_n;
	n = check_n_building(tab_test);
	if (n != hint->right[row] && n != 0)
		return false;


	return true;
}

bool	create_combo_matrix(int **grid, t_hint *hint)
{
	bool flag = 0;
	t_combinaison combo[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			combo[i][j].size = 0;
			for (int k = 0; k < 4; k++)
			{
				combo[i][j].tab[k] = 0;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (grid[i][j] == 0)
			{
				for (int k = 1; k <= 4; k++)
				{
					if (is_cell_free(grid, k, i, j, hint))
					{
						int n = combo[i][j].size;
						combo[i][j].tab[n] = k;
						combo[i][j].size++;
					}
				}
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (combo[i][j].size == 1)
			{
				grid[i][j] = combo[i][j].tab[0];
				printf("grid[%d][%d] = %d\n", i, j, combo[i][j].tab[0]);
				flag = 1;
			}
		}
	}
	return flag;
}

void	solve_grid(int **grid, t_hint *hint)
{
	solve_one(grid, hint);
	solve_four(grid, hint);
	is_cell_free(grid, 3, 0, 0, hint);
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

	hint->top = calloc(4, sizeof(int));
	hint->bot = calloc(4, sizeof(int));
	hint->left = calloc(4, sizeof(int));
	hint->right = calloc(4, sizeof(int));
	
	return hint;
}

void	fill_hint_arr(t_hint *hint)
{
	for (int i = 0; i < 4; i++)
	{
		hint->top[i] = hint->hint_arr[i];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->bot[i] = hint->hint_arr[i+4];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->left[i] = hint->hint_arr[i+8];
	}
	for (int i = 0; i < 4; i++)
	{
		hint->right[i] = hint->hint_arr[i+12];
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
	printf("HINTS:\n");
	print_hint(hint);
	printf("GRID:\n");
	solved_grid = create_solved_grid();
	print_grid(solved_grid);
	solve_grid(solved_grid, hint);
	printf("PRE FILED SOLVED GRID:\n");
	print_grid(solved_grid);

	int i = 0;
	bool flag = 0;
	do
	{
		flag = create_combo_matrix(solved_grid, hint);
		i++;
	}
	while (flag && i < 100); 
	print_grid(solved_grid);
	printf("i: %d\n", i);

//	same_n_row(solved_grid, 1);
//	printf("expected output:\ncol 0 and 3;\ncol 2 and 3;\n");

}
