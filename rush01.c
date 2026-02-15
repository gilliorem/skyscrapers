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
	//for (int i = 0; argv_one[i]; i+=2)
	//for (int i = 0; argv_one[i]; i++)
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

void	solve_grid_one(int **grid, t_hint *hint)
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

void	solve_grid_four(int **grid, t_hint *hint)
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
				replace_same_row(grid, grid[row][col], col, row);
				return true;
			}
			k++;
		}
		k = 1;
	}
	return false;
}

bool	is_cell_free(int **grid, int try_n, int row, int col, int **hint)
{
	for (int i = 0; i < 4; i++)
	{
		if (grid[i][col] == try_n)
			return false;		
	}
	for (int i = 0; i < 4; i++)
	{
		if (grid[row][i] == try_n)
			return false;
	}
	int tab_test[4];
	for (int i=0;i<4;i++)
		tab_test[i] = grid[i][col];	
	tab_test[row] = try_n;
	int n = check_n_building(tab_test);
	if (n != )
	return true;
}

bool	same_n_col(int **grid, int col)
{
	int k = 1;
	for (int row = 0; row < 4; row++)
	{
		k = row + k;
		if (k  > 3)
			break;
//		printf("col: %d\n", col);
//		printf("k: %d\n", k);
//		printf("cell %d\n", grid[col][col]);
		while (k < 4)
		{
			if (grid[col][row] == grid[k][col])
			{
				printf("same number in row %d and row %d\n", row, k);
				replace_same_col(grid, grid[col][row], col, row);
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
		if (tab[i] > max)
		{
			max = tab[i];
			buildings++;
		}
	}
	return buildings;
}

// if hint == 3
// 	- first cell is 1 OR 2 (cannot be 4 or 1)
// 		(if it'3 I can not only see 3 and 4; if its 4, can only see 4)
// 		
// 	- when all the buildings are placed, we will see 3 buildings
//	

void	solve_three(int **grid, t_hint *hint)
{
	for (int i = 0; i < 4; i++)
	{
		if (hint->top[i] == 3)
		{
			grid[0][i] = 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (hint->bot[i] == 3)
		{
			grid[3][i] = 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (hint->left[i] == 3)
		{
			grid[i][0] = 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (hint->right[i] == 3)
		{
			grid[i][3] = 1;
		}
	}
}

void	replace_same_row(int **grid, int n, int col, int row)
{
	if (n == 1)
		n++;
	if (n == 2)
		n++;
	if (n == 3)
		n--;
	grid[col][row] = n;
}

void	replace_same_col(int **grid, int n, int col, int row)
{
	if (n == 1)
		n++;
	if (n == 2)
		n++;
	if (n == 3)
		n--;
	grid[col][row] = n;
}

void	fill_last_cell_col(int **grid, int col)
{
	int sum  = 0;
	for (int i = 0; i < 4; i++)
		sum += grid[i][col];
	if (sum == 7)
	{
		for (int i = 0; i < 4; i++)
			if (grid[i][col] == 0)
				grid[i][col] = 3;
	}
	printf("sum:%d\n", sum);
}

void	fill_last_cell_row(int **grid, int row)
{
	int sum  = 0;
	for (int i = 0; i < 4; i++)
		sum += grid[row][i];
	if (sum == 7)
	{
		for (int i = 0; i < 4; i++)
			if (grid[row][i] == 0)
				grid[row][i] = 3;
	}
	printf("sum:%d\n", sum);
}

// the constraints: 
// 	the same number cannnot be place twice in the same col/row

void	solve_grid(int **grid, t_hint *hint)
{
	solve_grid_one(grid, hint);
	solve_grid_four(grid, hint);
	solve_three(grid, hint);
	for (int i = 0; i < 4; i++)
		same_n_row(grid, i);
	for (int i = 0; i < 4; i++)
		same_n_col(grid, i);
	for (int i = 0; i < 4; i++)
		fill_last_cell_col(grid, i);
	for (int i = 0; i < 4; i++)
		fill_last_cell_row(grid, i);
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
	printf("SOLVED GRID:\n");
	solved_grid = create_solved_grid();
	print_grid(solved_grid);
	solve_grid(solved_grid, hint);
	printf("PRE FILED SOLVED GRID:\n");
	print_grid(solved_grid);

//	same_n_row(solved_grid, 1);
//	printf("expected output:\ncol 0 and 3;\ncol 2 and 3;\n");

}
