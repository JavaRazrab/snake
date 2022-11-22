#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <random>
#include <ctime>
#include <queue>

#pragma execution_character_set( "utf-8" )

#define WINDOW_HIGH 30
#define WINDOW_WIDTH 60
#define DX 1
#define DY 1

using namespace std;

bool isGameOver = false, pause = false;

short xApple, yApple;

short field[30][30];

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(short x, short y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if(size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console,&lpCursor);
}

void drawRectangle(){
    gotoxy(0,0);
    for(int i = 0;i < WINDOW_WIDTH + DX; i++)
        cout << "▄";
    gotoxy(0, WINDOW_HIGH+DY);
    for(int i = 0;i < WINDOW_WIDTH + DX; i++)
        cout << "▀";
    for(int i = 1;i < WINDOW_HIGH+DY; i++){
        gotoxy(0,i);
        cout << "█";
        for(int j=1;j<WINDOW_WIDTH;j++)
            cout << " ";
        cout << "█";
    }
}

void drawApple(){
    gotoxy(xApple+DX, yApple+DY);
    cout << "a";
}

struct BeautyText {
    vector<string> strings;
    BeautyText()= default;
    BeautyText(vector<string>* _strings){
        strings = *_strings;
    }
};

void drawAnyAt(short x, short y, BeautyText* beautyText, bool fromLeft){
    short dx = 0;
    if(!fromLeft)
        dx = beautyText->strings[0].length();
    for(int i=0;i<beautyText->strings.size();i++){
        gotoxy(x-dx,y+i);
        cout << beautyText->strings[i];
    }
}

void drawYourScore(){
    auto* beautyText = new BeautyText();
    beautyText->strings.emplace_back("__  ______  __  _____    _____________  ___  ____  _ ");
    beautyText->strings.emplace_back("\\ \\/ / __ \\/ / / / _ \\  / __/ ___/ __ \\/ _ \\/ __/ (_)");
    beautyText->strings.emplace_back(" \\  / /_/ / /_/ / , _/ _\\ \\/ /__/ /_/ / , _/ _/  _  ");
    beautyText->strings.emplace_back(" /_/\\____/\\____/_/|_| /___/\\___/\\____/_/|_/___/ (_) ");
    drawAnyAt(65,2,beautyText,true);
}

void drawPauseMenu(short cursor){
    auto* beautyText = new BeautyText();
    beautyText->strings.emplace_back(" _____________________________ ");
    beautyText->strings.emplace_back("/    ___  ___  __  __________ \\");
    beautyText->strings.emplace_back("|   / _ \\/ _ |/ / / / __/ __/ |");
    beautyText->strings.emplace_back("|  / ___/ __ / /_/ /\\ \\/ _/   |");
    beautyText->strings.emplace_back("| /_/  /_/ |_\\____/___/___/   |");
    beautyText->strings.emplace_back("|                             |");
    beautyText->strings.emplace_back("|                             |");
    if(cursor == 0)
        beautyText->strings.emplace_back("|        > CONTINUE <         |");
    else
        beautyText->strings.emplace_back("|          CONTINUE           |");
    beautyText->strings.emplace_back("|                             |");
    beautyText->strings.emplace_back("|                             |");
    if(cursor == 1)
        beautyText->strings.emplace_back("|         > QUITE <           |");
    else
        beautyText->strings.emplace_back("|           QUITE             |");
    beautyText->strings.emplace_back("\\_____________________________/");
    drawAnyAt(WINDOW_WIDTH/2-beautyText->strings[0].length()/2,WINDOW_HIGH/2-7,beautyText,true);
    //drawAnyAt(WINDOW_WIDTH/2-beautyText->s1.length()/2,WINDOW_HIGH/2,beautyText,true);
}

void makeScore(BeautyText* stringScore, int score){
    string s1, s2, s3, s4;
    int l = (int)log10(score);
    while(l>=0){
        switch(score/(int)pow(10, l)){
            case 0:
                s1+="  ___  ";
                s2+=" / _ \\ ";
                s3+="/ // / ";
                s4+="\\___/  ";
                break;
            case 1:
                s1+="   ___ ";
                s2+="  <  / ";
                s3+="  / /  ";
                s4+=" /_/   ";
                break;
            case 2:
                s1+="   ___ ";
                s2+="  |_  |";
                s3+=" / __/ ";
                s4+="/____/ ";
                break;
            case 3:
                s1+="   ____";
                s2+="  |_  /";
                s3+=" _/_ < ";
                s4+="/____/ ";
                break;
            case 4:
                s1+="  ____ ";
                s2+=" / / / ";
                s3+="/_  _/ ";
                s4+=" /_/   ";
                break;
            case 5:
                s1+="   ____";
                s2+="  / __/";
                s3+=" /__ \\ ";
                s4+="/____/ ";
                break;
            case 6:
                s1+="  ____";
                s2+=" / __/";
                s3+="/ _ \\ ";
                s4+="\\___/ ";
                break;
            case 7:
                s1+=" ____ ";
                s2+="/_  / ";
                s3+=" / /  ";
                s4+="/_/   ";
                break;
            case 8:
                s1+="  ___ ";
                s2+=" ( _ )";
                s3+="/ _  |";
                s4+="\\___/ ";
                break;
            case 9:
                s1+="  ___ ";
                s2+=" / _ \\";
                s3+=" \\_, /";
                s4+="/___/ ";
                break;
        }
        score = score%(int)pow(10,l);
        l--;
    }
    stringScore->strings.emplace_back(s1);
    stringScore->strings.emplace_back(s2);
    stringScore->strings.emplace_back(s3);
    stringScore->strings.emplace_back(s4);
}

void drawScore(int score){
    BeautyText* stringScore = new BeautyText();
    makeScore(stringScore, score);
    drawAnyAt(WINDOW_WIDTH+3,7,new BeautyText(new vector<string>{"                                                     ",
                                   "                                                     ",
                                   "                                                     ",
                                   "                                                     "}),true);
    drawAnyAt(110,7,stringScore,false);
}

struct SnakePart {
    short x, y;
    SnakePart* next;
    SnakePart() : x(0), y(0), next(nullptr){}
    SnakePart(short _x, short _y, SnakePart* _next) : x(_x), y(_y), next(_next){}
};

struct SnakeBody{
    SnakePart* head;
    SnakePart* tail;
    short length, dx, dy;

    SnakeBody(){
        head = new SnakePart(WINDOW_WIDTH/2, WINDOW_HIGH/2, nullptr);
        tail = new SnakePart(WINDOW_WIDTH/2-1, WINDOW_HIGH/2, head);
        length = 2;
        dx = 2;
        dy = 0;
    }

    void move(){
        gotoxy(head->x+DX, head->y+DY);
        cout << "+";
        head->next = new SnakePart((head->x+dx+WINDOW_WIDTH)%WINDOW_WIDTH,
                                   (head->y+dy+WINDOW_HIGH)%WINDOW_HIGH, nullptr);
        head = head->next;
        gotoxy(head->x+DX, head->y+DY);
        cout << "C";
        gotoxy(tail->x+DX, tail->y+DY);
        cout << " ";
        tail = tail->next;
    }

    void newPart(){
        tail = new SnakePart(tail->x, tail->y, tail);
        length++;
    }

    void cut(SnakePart* part){
        auto* _part = new SnakePart(0,0,tail);
        do{
            _part = _part->next;
            gotoxy(_part->x+DX, _part->y+DY);
            cout << " ";
        }while(_part != part);
        tail = part->next;
    }

    void checkCut(){
        SnakePart* part = tail;
        bool f = false;
        short l=1;
        while(part != head && !f){
            if(head->x == part->x && head->y == part->y){
                f = true;
                cut(part);
                length-=l;
                drawScore(length*50);
            }
            part = part->next;
            l++;
        }
    }

    bool isInSnake(int x, int y) const{
        SnakePart* part = tail;
        while(part != head){
            if(x == part->x && y == part->y)
                return true;
            part = part->next;
        }
        return false;
    }

    bool checkEatApple(){
        if(head->x == xApple && head->y == yApple){
            newPart();
            drawScore(length*50);
            do{
                xApple = rand()%(WINDOW_WIDTH/2)*2;
                yApple = rand()%WINDOW_HIGH;
            }while(isInSnake(xApple, yApple));
            drawApple();
            return true;
        }
        return false;
    }

    void draw(){
        SnakePart* part = tail;
        while(part != head){
            gotoxy(part->x+DX, part->y+DY);
            cout << "+";
            part = part->next;
        }
        gotoxy(part->x+DX, part->y+DY);
        cout << "C";
    }
};

void checkPlayKey(SnakeBody* snake){
    if(kbhit()){
        switch (getch()){
            case 'a':
            case 'A':
                if(snake->dx==0) {
                    snake->dx = -2;
                    snake->dy = 0;
                }
                break;
            case 'w':
            case 'W':
                if(snake->dy==0){
                    snake->dx = 0;
                    snake->dy = -1;
                }
                break;
            case 'd':
            case 'D':
                if(snake->dx==0) {
                    snake->dx = 2;
                    snake->dy = 0;
                }
                break;
            case 's':
            case 'S':
                if(snake->dy==0){
                    snake->dx = 0;
                    snake->dy = 1;
                }
                break;
            case 27:
                pause = true;
        }
    }
}

void checkPauseKey(short &cursor){
    if(kbhit()){
        switch (getch()){
            case 's':
            case 'S':
                cursor = (cursor+1)%2;
                drawPauseMenu(cursor);
                break;
            case 'w':
            case 'W':
                cursor = (2+cursor-1)%2;
                drawPauseMenu(cursor);
                break;
            case 13:
                pause = false;
                if(cursor == 1)
                    isGameOver = true;
                break;
            case 27:
                pause = false;
        }
    }
}

void createApple(){
    xApple = rand()%(WINDOW_WIDTH/2)*2;
    yApple = rand()%WINDOW_HIGH;
    drawApple();
}

void pauseMenu(){
    short cursor = 0;
    drawPauseMenu(cursor);
    while(pause){
        checkPauseKey(cursor);
    }
}

struct Coordinates{
    short x, y;
    Coordinates(){
        x = 0;
        y = 0;
    }
    Coordinates(short _x, short _y){
        x = _x;
        y = _y;
    }
};

void fillField(short x, short y, short n, queue<Coordinates*>* q){
    auto* cords = new Coordinates();
    short xsize = sizeof(field), ysize = sizeof(field[0]);
    if(field[(x+xsize-1)%xsize][y] == 0 || field[(x+xsize-1)%xsize][y] == -3){
        field[(x+xsize-1)%xsize][y] = n;
        cords->x = (x+xsize-1)%xsize;
        cords->y = y;
        q->push(cords);
        if(field[(x+xsize-1)%xsize][y] == -3)
            return;

    }
    if(field[x][(y+ysize-1)%ysize] == 0 || field[x][(y+ysize-1)%ysize] == -3){
        field[x][(y+ysize-1)%ysize] = n;
        cords->x = x;
        cords->y = (y+ysize-1)%ysize;
        q->push(cords);
        if(field[x][(y+ysize-1)%ysize] == -3)
            return;

    }
    if(field[(x+xsize+1)%xsize][y] == 0 || field[(x+xsize+1)%xsize][y] == -3){
        field[(x+xsize+1)%xsize][y] = n;
        cords->x = (x+xsize+1)%xsize;
        cords->y = y;
        q->push(cords);
        if(field[(x+xsize+1)%xsize][y] == -3)
            return;

    }
    if(field[x][(y+ysize+1)%ysize] == 0 || field[x][(y+ysize+1)%ysize] == -3){
        field[x][(y+ysize+1)%ysize] = n;
        cords->x = x;
        cords->y = (y+ysize+1)%ysize;
        q->push(cords);
        if(field[x][(y+ysize+1)%ysize] == -3)
            return;

    }
    q->pop();
    if(!q->empty()){
        fillField(q->front()->x,q->front()->y,n+1,q);
    }
    cout << 1;
}

void refreshField(SnakeBody* snake){
    for(int i=0;i<WINDOW_HIGH;i++)
        for(int j=0;j<WINDOW_WIDTH/2;j++)
            field[i][j] = 0;
    field[xApple][yApple] = -3;
    SnakePart* part = snake->tail;
    while(part != snake->head){
        field[part->x][part->y]=-2;
        part = part->next;
    }
    field[part->x][part->y]=-1;
}

vector<Coordinates*>* findTheWay(SnakeBody* snake){
    refreshField(snake);
    auto* q = new queue<Coordinates*>;
    q->push(new Coordinates());
    fillField(snake->head->x,snake->head->y, 1, q);
    auto* v = new vector<Coordinates*>;
    short n, x, y;
    if(field[xApple][yApple] != -3){
        n = field[xApple][yApple];
        x = xApple; y = yApple;
    }else{
        n = 0;
        for(short i=0;i<sizeof(field);i++){
            for(short j=0;j<sizeof(field[0]);j++){
                if (field[i][j] > n){
                    n = field[i][j];
                    x = i;
                    y = j;
                }
            }
        }
    }
    gotoxy(100,25);
    cout << "!!!" << n;
    while(n > 0){
        if (x<sizeof(field)-1 && field[x+1][y] == n-1){
            n--;
            v->push_back(new Coordinates(-2, 0));
        } else if (x>0 && field[x-1][y] == n-1){
            n--;
            v->push_back(new Coordinates(2, 0));
        } else if (y<sizeof(field[0])-1 && field[x][y+1] == n-1){
            n--;
            v->push_back(new Coordinates(0, -1));
        } else {
            n--;
            v->push_back(new Coordinates(0, 1));
        }
    }
    return v;
}

void playPlayer(SnakeBody* snake){
    while(!isGameOver){
        checkPlayKey(snake);
        snake->move();
        snake->checkCut();
        snake->checkEatApple();
        Sleep(100);
        if(pause){
            pauseMenu();
            if(!isGameOver){
                drawRectangle();
                snake->draw();
                drawApple();
            }
        }
    }
}

void playAI(SnakeBody* snake){
    auto* wayToApple = findTheWay(snake);
    while(!isGameOver){
        cout << wayToApple->size();
        snake->dx = wayToApple->back()->x;
        snake->dy = wayToApple->back()->y;
        wayToApple->pop_back();
        snake->move();
        snake->checkCut();
        Sleep(100);
        if(snake->checkEatApple())
            wayToApple = findTheWay(snake);
    }
}

void game_loop(){
    drawRectangle();
    createApple();
    auto* snake = new SnakeBody();
    drawYourScore();
    drawScore(snake->length*50);
    /*snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();
    snake->newPart();snake->newPart();snake->newPart();snake->newPart();*/
    setcursor(false, 20);

    //playAI(snake);
    playPlayer(snake);
}

int main() {
    MoveWindow(GetConsoleWindow(), 300, 200, 1000, 600, TRUE);
    SetConsoleOutputCP( 65001 );
    srand(time(nullptr));
    game_loop();
    return 0;
}
