#include<fstream>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>


const int x_max = 100;
const int y_max = 25;

void GoToXY(int x, int y);
void CursorVisible(bool visible);


    enum Rotation
    {
        left,
        right,
        up,
        down
    };

    struct Position
    {
        int x;
        int y;
    };

       class Field
    {
    public:
        char field[y_max][x_max + 1];
        void CreateBoundaries(char ch);
        void Show();
        void SpownPrice();
    };

    class Snake
    {
    private:
        int length;
        Position snakePos[1000];
        Rotation snakeRot;
    public:
        Snake(int _length, Rotation _snakeRot) :
            length(_length), snakeRot(_snakeRot) {};

        void ToStartValues();
        void Move(char field[y_max][x_max + 1]);
        void AddTail();
        bool IsGameOver();
        void SaveToFile();
    };
    void Field::CreateBoundaries(char ch)
    {
        for (int y = 0; y < y_max; y++)
        {
            for (int x = 0; x < x_max; x++)
            {
                if (y == 0 || y == y_max - 1 ||
                    x == 0 || x == x_max - 1)
                {
                    field[y][x] = ch;
                }
                else
                {
                    field[y][x] = ' ';
                }
            }
            field[y][x_max] = '\0';
        }
    }

    void Field::Show()
    {
        GoToXY(0, 0);

        for (int y = 0; y < y_max; y++)
        {
            std::cout << field[y] << std::endl;
        }
    }

    void Field::SpownPrice()
    {
        int x = rand() % (x_max - 2) + 1;
        int y = rand() % (y_max - 2) + 1;

        bool isSpown = false;
        while (true)
        {
            if (x % 2 == 0) isSpown = true;

            if (field[y][x] == 'O' || field[y][x] == '*') isSpown = false;

            if (isSpown) break;

            x = rand() % (x_max - 2) + 1;
        }

        field[y][x] = '$';
    }

    void Snake::ToStartValues()
    {
        for (int i = 0; i < length; i++)
        {
            snakePos[i].x = 0;
            snakePos[i].y = 0;
        }

        length = 1;

        snakePos[0] = { x_max / 2, y_max / 2 };

        GoToXY(snakePos[0].x, snakePos[0].y);
        putchar('O');
    }

    

    void Snake::AddTail()
    {
        snakePos[length].x = snakePos[length - 1].x;
        snakePos[length].y = snakePos[length - 1].y;

        length++;
    }
    void Snake::Move(char field[y_max][x_max + 1])
    {
        Sleep(100);
        if (GetAsyncKeyState('W'))
        {
            snakeRot = up;
        }
        else if (GetAsyncKeyState('S'))
        {
            snakeRot = down;
        }
        else if (GetAsyncKeyState('A'))
        {
            snakeRot = left;
        }
        else if (GetAsyncKeyState('D'))
        {
            snakeRot = right;
        }

        field[snakePos[length - 1].y][snakePos[length - 1].x] = ' ';

        for (int i = length; i > 0; i--)
        {
            snakePos[i] = snakePos[i - 1];
        }

        switch (snakeRot)
        {
        case up:
            snakePos[0].y = snakePos[0].y - 1;
            break;

        case down:
            snakePos[0].y = snakePos[0].y + 1;
            break;

        case left:
            snakePos[0].x = snakePos[0].x - 2;
            break;

        case right:
            snakePos[0].x = snakePos[0].x + 2;
            break;
        }

        if (field[snakePos[0].y][snakePos[0].x] == '$')
        {
            AddTail();
            SaveToFile();
        }
        field[snakePos[0].y][snakePos[0].x] = 'O';

        if (length > 1)
        {
            field[snakePos[1].y][snakePos[1].x] = '*';
        }
    }
    void Snake::SaveToFile() 
    {
        std::string main_File = "Game.txt";
        Snake s(0, up);

        std::ofstream Game_Snake(main_File);
        Game_Snake<<"Your score is:\t" << length;
        std::cout << "\033[1;32m We saved your date correctly!\033[0m";
        Game_Snake.close();
    };
    bool Snake::IsGameOver()
    {
        bool isGameOver = false;

        if (snakePos[0].x <= 0 || snakePos[0].x >= x_max - 1 ||
            snakePos[0].y <= 0 || snakePos[0].y >= y_max - 1)
        {
            isGameOver = true;
        }

        for (int i = 2; i < length; i++)
        {
            if (snakePos[0].x == snakePos[i].x &&
                snakePos[0].y == snakePos[i].y)
            {
                isGameOver = true;
            }
        }

        return isGameOver;
    }

    void GoToXY(int x, int y)
    {
        COORD pos = { x, y };
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(handle, pos);
    }

    void CursorVisible(bool visible)
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(handle, &structCursorInfo);
        structCursorInfo.bVisible = visible;
        SetConsoleCursorInfo(handle, &structCursorInfo);
    }

int main()
{
    //My code
    int n;

    std::cout << "\033[1;32m If you want to play a game press 1 or press 2 to break:\033[0m"; std::cin >> n;
    //To start a game:
    if (n == 1)
    {
        Field f;

        f.CreateBoundaries(1);
        f.Show();
        f.SpownPrice();

        srand(time(NULL));

        CursorVisible(false);

        Field gameField;
        Snake snake(1, up);

        gameField.CreateBoundaries(219);
        snake.ToStartValues();

        for (int i = 0; i < 20; i++)
            gameField.SpownPrice();

        while (true)
        {
            if (snake.IsGameOver()) break;

            snake.Move(gameField.field);
            gameField.Show();

            Sleep(85);
        }

        getchar();

        int text;
        std::cout << "If you want to save your file press 1: ";
        std::cin >> text;

        // Save data into file:
        if (text == 1)
        {
            Snake snake(0, up);
            snake.SaveToFile();

        }
        else { system("cls"); };
    }
    
    //Break:
    if (n == 2)
    {
        std::cout << "\033[1;31m Goodbye! See you next time...\033[0m";
        return 0;
    }
}
