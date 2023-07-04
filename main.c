#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<stdbool.h>

//Print out the map.Take 0 in the array as the floor.
//Take 1 in the array as the wall.Take 2 in the array as a person.
//Take 3 in the array as the target location.Take 4 in the array as a box.

int** function_map_size(char* string)
{
	FILE* fp;
	int a, row = 0, line = 0;
	if ((fp = fopen(string, "r")) == NULL)                                       //This part does not need double quotes so that you can directly quote the variable name to modify the file name in the map
	{
		puts("Fail to open file!");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{
			a = fgetc(fp);
			if (a == '\n')
			{
				row++;
			}
			else if (a != ' ' && a != '\n')
			{
				line++;
			}
		}
		row = row + 1;
		line = line / row;                                                          //Because the calculation will be repeated, this part is required to calculate the exact row and column
		int** p = (int**)malloc(row * sizeof(int*));
		for (int i = 0;i < row;i++)
		{
			p[i] = (int*)malloc(line * sizeof(int));
		}
		fclose(fp);
		fp = fopen(string, "r");
		int  i = 0, k = 0;                                                         //Because there are spaces in the text, we have to assign a value using a variable
		while (!feof(fp))
		{
			a = fgetc(fp);
			if (a != ' ' && a != '\n')
			{
				p[i][k] = a - 48;
				k++;
			}
			else if (a == '\n')
			{
				i++;
				k = 0;                                                             //Changed line k needs to be reset to 0
			}
		}
		fclose(fp);
		return p;
	}
}

void function_rank(int step, int level)
{
	FILE* fp;
	char name[20];
	if ((fp = fopen("rank.txt", "a+")) == NULL)
	{
		puts("Fail to open file!");
		exit(0);
	}
	else
	{
		gets(name);
		fprintf(fp, "name:%s    step:%d     level:%d\n", name, step, level);
		fclose(fp);
	}
}

int map_row(char* string)
{
	FILE* fp;
	int a, row = 0;
	if ((fp = fopen(string, "r")) == NULL)
	{
		puts("Fail to open file!");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{
			a = fgetc(fp);
			if (a == '\n')
			{
				row++;
			}
		}
		row = row + 1;
		fclose(fp);
		return row;
	}
}

int map_line(char *string)
{
	FILE* fp;
	int a, row = 0, line = 0;
	if ((fp = fopen(string, "r")) == NULL)
	{
		puts("Fail to open file!");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{
			a = fgetc(fp);
			if (a == '\n')
			{
				row++;
			}
			else if (a != ' ' && a != '\n')
			{
				line++;
			}
		}
		row = row + 1;
		line = line / row;
		fclose(fp);
		return line;
	}
}

void print_map(int** map, int row, int line)
{
	for (int i = 0;i < row;i++)
	{
		for (int j = 0;j < line;j++)
		{
			switch (map[i][j])
			{
			case 0:printf(" ");break;  //floorw
			case 1:printf("=");break;  //wall
			case 2:printf("+");break;  //person
			case 3:printf("$");break;  //location
			case 4:printf("#");break;  //box
			case 5:printf("@");break;  //2+3 when person stand in the location
			case 7:printf("*");break;  //3+4 when box stand in the location
			}
		}
		printf("\n");
	}
}

int move(int step, int** map, int row, int line)
{
	char push;
	int x, y;                                                       //x and y as the location of person
	for (int i = 0;i < row;i++)                                     //The number of loops here needs to be changed, when we use dynamic storage
		for (int j = 0;j < line;j++)                                //The number of loops here needs to be changed, when we use dynamic storage
		{
			if (map[i][j] == 2 || map[i][j] == 5)                   //Find the location of people on the map for situational judgment
			{
				x = i;
				y = j;
			}
		}
	push = getch();                                                 //This function does not belong to the function in stdio.h, its function is to return the input value without pressing the Enter key
                                                                    //It will wait for you to press any key, assign the ASCII code corresponding to the key character to ch, and then execute the following statement.


	switch (push)                                                  //This function that controls the movement of the character will only count the number of steps after accepting wsad and does not respond to other characters

	{

		//control the person to up
	case 'W':
	case 'w':
		if (map[x - 1][y] == 0 || map[x - 1][y] == 3)                                  //When there is an open space or location in front of a person
		{
			map[x][y] = map[x][y] - 2;
			map[x - 1][y] = map[x - 1][y] + 2;
			step++;
		}
		else if (map[x - 1][y] == 4)                                                   //When a person is in front of a box
		{
			if (map[x - 2][y] == 1 || map[x - 2][y] == 4 || map[x - 2][y] == 7)        //before the box is wall or box
			{
				map[x - 1][y] = 4;
			}
			else
			{
				map[x - 2][y] = map[x - 2][y] + 4;
				map[x - 1][y] = map[x - 1][y] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		else if (map[x - 1][y] == 7)                                                   //If the box is already at the destination
		{
			if (map[x - 2][y] == 1 || map[x - 2][y] == 4 || map[x - 2][y] == 7)
			{
				map[x - 1][y] = 7;
			}
			else
			{
				map[x - 2][y] = map[x - 2][y] + 4;
				map[x - 1][y] = map[x - 1][y] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		break;

		//control the person to down
	case 'S':
	case 's':
		if (map[x + 1][y] == 0 || map[x + 1][y] == 3)                                 //When there is an open space or location in front of a person
		{
			map[x][y] = map[x][y] - 2;
			map[x + 1][y] = map[x + 1][y] + 2;
			step++;
		}
		else if (map[x + 1][y] == 4)                                                  //When a person is in front of a box
		{
			if (map[x + 2][y] == 1 || map[x + 2][y] == 4|| map[x + 2][y] == 7)         //before the box is wall or box
			{
				map[x + 1][y] = 4;
			}
			else
			{
				map[x + 2][y] = map[x + 2][y] + 4;
				map[x + 1][y] = map[x + 1][y] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		else if (map[x + 1][y] == 7)                                                   //If the box is already at the destination
		{
			if (map[x + 2][y] == 1 || map[x + 2][y] == 4 || map[x + 2][y] == 7)
			{
				map[x + 1][y] = 7;
			}
			else
			{
				map[x + 2][y] = map[x + 2][y] + 4;
				map[x + 1][y] = map[x + 1][y] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		break;

		//control the person to right
	case 'D':
	case 'd':
		if (map[x][y + 1] == 0 || map[x][y + 1] == 3)                                 //When there is an open space or location in front of a person
		{
			map[x][y] = map[x][y] - 2;
			map[x][y + 1] = map[x][y + 1] + 2;
			step++;
		}
		else if (map[x][y + 1] == 4)                                                   //When a person is in front of a box
		{
			if (map[x][y + 2] == 1 || map[x][y + 2] == 4|| map[x][y + 2] == 7)          //before the box is wall or box
			{
				map[x][y + 1] = 4;
			}
			else
			{
				map[x][y + 2] = map[x][y + 2] + 4;
				map[x][y + 1] = map[x][y + 1] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		else if (map[x][y + 1] == 7)                                                    //If the box is already at the destination
		{
			if (map[x][y + 2] == 1 || map[x][y + 2] == 4 || map[x][y + 2] == 7)
			{
				map[x][y + 1] = 7;
			}
			else
			{
				map[x][y + 2] = map[x][y + 2] + 4;
				map[x][y + 1] = map[x][y + 1] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		break;

		//control the person to left
	case 'A':
	case 'a':
		if (map[x][y - 1] == 0 || map[x][y - 1] == 3)                               //When there is an open space or location in front of a person
		{
			map[x][y] = map[x][y] - 2;
			map[x][y - 1] = map[x][y - 1] + 2;
			step++;

		}
		else if (map[x][y - 1] == 4)                                               //When a person is in front of a box
		{
			if (map[x][y - 2] == 1 || map[x][y - 2] == 4|| map[x][y - 2] == 7)      //before the box is wall or box
			{
				map[x][y - 1] = 4;
			}
			else
			{
				map[x][y - 2] = map[x][y - 2] + 4;
				map[x][y - 1] = map[x][y - 1] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		else if (map[x][y - 1] == 7)                                               //If the box is already at the destination
		{
			if (map[x][y - 2] == 1 || map[x][y - 2] == 4 || map[x][y - 2] == 7)
			{
				map[x][y - 1] = 7;
			}
			else
			{
				map[x][y - 2] = map[x][y - 2] + 4;
				map[x][y - 1] = map[x][y - 1] - 2;
				map[x][y] = map[x][y] - 2;
				step++;
			}
		}
		break;
	}
	return step;
}

bool win(bool judge, int** map, int row, int line)                                       //This defines a boolean variable for judgment because the victory condition I only need to judge true or false
{
	int bnum = 0, wnum = 0;                                                                //One of these two variables represents the number of destinations, and the other represents the number of boxes that coincide with the destination
	for (int i = 0;i < row;i++)
		for (int j = 0;j < line;j++)
		{
			if (map[i][j] == 4 || map[i][j] == 7)
				bnum++;
		}
	for (int i = 0;i < row;i++)
		for (int j = 0;j < line;j++)
		{
			if (map[i][j] == 7)
				wnum++;
		}
	if (bnum == wnum)
	{
		judge = false;
		return judge;
	}
}

void function_rank_print()
{
	FILE* fp;
	char ch;
	if ((fp = fopen("rank.txt", "r")) == NULL)
	{
		puts("Fail to open file!");
		exit(0);
	}
	else
	{
		while (!feof(fp))
		{
			ch = fgetc(fp);
			printf("%c", ch);
		}
		fclose(fp);
	}
}

void function_menu_print()
{
	int num;
	printf("enter the corresponding number to execute the corresponding content.\n");
	printf("1.start the game.\n");
	printf("2.view player data.\n");
	printf("3.play rules.\n");
	printf("4.exit the game.\n");
	while (1)
	{
		scanf("%d", &num);
		if (num == 1)                                                                         //End the loop by entering 1 directly to break
		{
			system("cls");
			break;
		}
		else if (num == 2)
		{
			system("cls");
			function_rank_print();
			printf("enter 9 to return to the previous page.\n");
		}
		else if (num == 3)
		{
			system("cls");
			printf("'w'go up\n");
			printf("'s'go down\n");
			printf("'d'go right\n");
			printf("'a'go left\n");
			printf("players can not pull boxs\n");
			printf("you can not quit the game while in the game\n");
			printf("enter 9 to return to the previous page.\n");
		}
		else if (num == 4)                                                                 //Detect that the input is 4 and end all programs directly
		{
			system("cls");
			exit(0);
		}
		else if (num == 9)
		{
			system("cls");
			printf("enter the corresponding number to execute the corresponding content.\n");
			printf("1.start the game.\n");
			printf("2.view player data.\n");
			printf("3.play rules.\n");
			printf("4.exit the game.\n");
		}
		else
		{
			system("cls");
			printf("invalid input\n");
			printf("enter 9 to return to the previous page.\n");
		}
	}
}

void function_map_changed(char* string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] >= 48 && string[i] <= 57)                                           //Restricted by the way I name the map file, nothing needs to be changed except for the number
		{
			string[i] = string[i] + 1;
		}
		i++;
	}
}

int function_end(char* string, int step,int level)
{
	char ch;
	scanf("%c", &ch);                                                                        //This line handles the characters just read from the keyboard in the move function
	while (1)
	{
		if (level == 3)
		{
			return level;
		}
		printf("whether to enter the next map? Y or N\n");
		scanf("%c", &ch);
		if (ch == 'Y' || ch == 'y')
		{
			system("cls");
			function_map_changed(string);
			return level;
		}
		else if (ch == 'N' || ch == 'n')
		{
			int i = 5;                                                                       //This is used to end the map loop
			system("cls");
			printf("please enter your information\n");
			scanf("%c", &ch);                                                                //Because in this part you just entered the carriage return and need to use it to process it
			function_rank(step, level);
			system("cls");
			printf("press any key to return to the main menu\n");
			scanf("%c", &ch);
			system("cls");
			return i;
		}
		else
		{
			system("cls");
			printf("invalid input\n");
		}
	}
}

int main()
{
	while (1)
	{
		char random;                                                                  //Acts as any key to return to the main menu
		char string[100] = "map1.txt";                                                //File pathname
		int i = 0;		                                                              //level or map number
		function_menu_print();                                                        //Menu
		int step = 0;                                                                //Step count
		for (i = 1;i <4 ;i++)                                                        //It is better to use i equal to 1 and less than 4 to treat i as a map number
		{
			int** map = function_map_size(string);                                   //Mapping and size statistics
			bool judge = true;                                                       //Win or lose judgment
			while (judge)                                                            //In-game action
			{
				print_map(map, map_row(string), map_line(string));
				step = move(step, map, map_row(string), map_line(string));
				system("cls");                                                        //After each operation, we refresh this line to see the new state
				judge = win(judge, map, map_row(string), map_line(string));
			}
			printf("you win!\n");
			printf("step:%d\n", step);
			i = function_end(string, step, i);
			free(map[0]);                                                             //Release the size here to prepare for the next map rebuild
			free(map);
		}
		i--;                                                                          //Use i-- to ensure that the subsequent output level is 3
		if (i == 3)
		{
			printf("You have cleared the game, please enter your information\n");
			function_rank(step, i);
			system("cls");
			printf("press any key to return to the main menu\n");
			scanf("%c", &random);                                                     //The insertion here is for a more beautiful display, and there is no requirement for the data entered here
			system("cls");
		}
	}
	return 0;
}
