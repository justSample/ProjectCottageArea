#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include<time.h>

#define SAVE_LOAD_LENGTH 300
#define ARR_LENGTH 1024
#define CHAR_LENGTH 25
#define FILE_NAME "CottageArea.txt"

struct stDate {
    int day;
    int month;
    int year;
};

typedef struct stDate Date;

struct stPlant 
{
    int Id;
    char Name[CHAR_LENGTH];
    Date DateOfPlanting; //Дата посадки
    Date DateMaturationPlant; //Дата созревания
    int PlantingCount;
    _Bool LoveShade;
    int CountDays;
};

typedef struct stPlant Plant;
//typedef struct date dt;

void Init();
void MainLoop();

void AddRow();
void RemoveRow();
void PrintAllPlants();
void FindRecord();

void PrintThreeScorospelki();
void PrintPlantAfterDate();

void Save();
void Load();

void PrintPlant(Plant* plant);
void PrintTitle();
void RemoveNL(char* msg);
char* DateToChars(const Date dt);
char* IntToStr(const int* number);
char* BoolToText(const _Bool* bl);
Date* ConsoleReadData();
int* DaysDifference(const Date* dBegin, const Date* dEnd);

Plant* _plants;

int main(void)
{
    //strcpy - Скопировать
    //strcat - Добавить
    //strtok - split C#
    //strstr - contains
    //void* ptr

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char* locale = setlocale(LC_ALL, "");


    Init();
    Load();
    MainLoop();

    printf("\nКонец программы.\n");
    return 0;
}

void Init() {

    _plants = malloc(sizeof(Plant) * ARR_LENGTH);

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        _plants[i].Id = -1;
        _plants[i].LoveShade = 0;
    }

}

void MainLoop() {

    while (1)
    {
        printf("\nГлавное меню\n");

        printf("1. Вывести все растения\n");
        printf("2. Добавление нового растения\n");
        printf("3. Удаление растения\n");
        printf("4. Поиск\n");
        printf("5. Сохранить\n");
        printf("6. Загрузить\n");

        printf("\n");

        printf("-1: Выход\n");

        printf("\n");

        printf("Выбор: ");
        int userSelect;
        scanf("%d", &userSelect);

        //printf("Пользователь выбрал: %d\n", userSelect);

        switch (userSelect) {

        case -1:
            return;

        case 1:
            PrintAllPlants();
            break;
        case 2:
            AddRow();
            break;
        case 3:
            RemoveRow();
            break;
        case 4:
            FindRecord();
            break;
        case 5:
            Save();
            break;
        case 6:
            Load();
            break;

        default:
            printf("Такого нету. Попробуйте другой!\n");
            break;
        }

    }

}

void AddRow() {

    Plant newPlant;

    newPlant.Id = -1;
    
    getchar();

    printf("Наименование растения: ");
    fgets(newPlant.Name, (sizeof(newPlant.Name) / sizeof(newPlant.Name[0])), stdin);
    RemoveNL(newPlant.Name);

    printf("Дата посадки.\n");

    Date* dt = ConsoleReadData();
    
    newPlant.DateOfPlanting.day = dt->day;
    newPlant.DateOfPlanting.month = dt->month;
    newPlant.DateOfPlanting.year = dt->year;

    free(dt);

    printf("Расчётная дата созревания.\n");

    dt = ConsoleReadData();

    newPlant.DateMaturationPlant.day = dt->day;
    newPlant.DateMaturationPlant.month = dt->month;
    newPlant.DateMaturationPlant.year = dt->year;

    free(dt);

    printf("Кол-во высаженных экземпляров: ");
    scanf("%d", &newPlant.PlantingCount);


    printf("Относится ли растение к тенелюбивым? (0 - нет, 1 - да): ");
    scanf("%d", &newPlant.LoveShade);

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id != -1) continue;

        newPlant.Id = (i + 1);
        _plants[i] = newPlant;
        printf("Запись добавлена под номер: %d\n", _plants[i].Id);
        return;

    }

}

void RemoveRow() {
    int id = -1;
    printf("Выберите ID для удаления: ");
    scanf("%d", &id);
    if (id <= 0) {
        printf("Такого ID нету.\n");
        return;
    }

    _plants[id - 1].Id = -1;
}

void FindRecord() {
    //а) трех наиболее скороспелых тенелюбивых растений; 
    //б) общего количества растений высаженных после заданной даты.
    while (1) {
        printf("Что хотите найти?\n");
        printf("1. Три наиболее скороспелых тенелюбивых растения\n");
        printf("2. Общее количество растений высаженных после заданной даты\n");
        printf("-1. Выход.\n");

        printf("Ваш выбор: ");
        int userSelect;

        scanf("%d", &userSelect);

        switch (userSelect) {
        case 1:
            PrintThreeScorospelki();
            break;
        case 2:
            PrintPlantAfterDate();
            break;
        case -1: return;
        default:
            printf("Выбор не правильный. Попробуй ещё раз!\n\n");
            break;
        }
    }
}

void PrintThreeScorospelki() {

    int countArray = 0;
    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;
        if (_plants[i].LoveShade == 0) continue;
        countArray++;
    }

    //а) трех наиболее скороспелых тенелюбивых растений; 
    printf("Всего тенелюбивых: %d\n", countArray);

    Plant* plantsLoveShadow = malloc(sizeof(Plant) * countArray);

    int index = 0;
    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;
        if (_plants[i].LoveShade == 0) continue;
        plantsLoveShadow[index] = _plants[i];
        index++;
    
    }

    //Сортировка пузырьком
    for (int i = 0; i < countArray - 1; i++) {
        for (int j = 0; j < countArray - i - 1; j++)
        {
            //Чтоб сменить с 'большего к меньшему' на 'меньшего к большему' смени знак < на > и наоборот
            if (plantsLoveShadow[j].CountDays > plantsLoveShadow[j + 1].CountDays)
            {
                Plant temp;
                temp = plantsLoveShadow[j];
                plantsLoveShadow[j] = plantsLoveShadow[j + 1];
                plantsLoveShadow[j + 1] = temp;
                j = 0;
                i = 0;
            }
        }
    }

    //Если захотим, можно все распечатать. Вместо 3 - countArray
    printf("|%-3s|%-25s|%-12s|%-15s|%-8s|%-11s|%-12s|\n", "ID", "Назв. растения", "Дата посадки", "Дата созревания", "Высажено", "Тенелюбивое", "Дней нужно");
    for (int i = 0; i < 3; i++)
    {
        char* dtp = DateToChars(plantsLoveShadow[i].DateOfPlanting);
        char* dmp = DateToChars(plantsLoveShadow[i].DateMaturationPlant);
        char* ls = BoolToText(plantsLoveShadow[i].LoveShade);

        printf("|%-3d|%-25s|%-12s|%-15s|%-8d|%-11s|%-12d|\n", 
            plantsLoveShadow[i].Id,
            plantsLoveShadow[i].Name,
            dtp,
            dmp,
            plantsLoveShadow[i].PlantingCount,
            ls,
            plantsLoveShadow[i].CountDays
        );

        free(dtp);
        free(dmp);
        free(ls);
    }

    free(plantsLoveShadow);
}

int* DaysDifference(const Date* dBegin, const Date* dEnd) {

    int* years = malloc(sizeof(int));
    int* month = malloc(sizeof(int));
    int* totalMonth = malloc(sizeof(int));
    int* days = malloc(sizeof(int));
    int* totalDays = malloc(sizeof(int));
    *totalDays = 0;

    *years = dEnd->year - dBegin->year;

    if (*years > 0) 
    {
        *totalMonth = (dEnd->month + (12 * *years)) - (dBegin->month + (12 * (*years - 1)));
        *month = dEnd->month;
    }
    else 
    {
        *totalMonth = dEnd->month - dBegin->month;
        *month = dEnd->month;
    }

    if (dEnd->day > dBegin->day) 
    {
        *days = dEnd->day - dBegin->day;
    }
    else 
    {
        *days = dBegin->day - dEnd->day;
        *totalMonth -= 1;
    }



    for (int i = 1; i <= *totalMonth; i++)
    {
        switch ((*month - i) % 12)
        {
            case 0:
            case 2:
            case 4:
            case 6:
            case 7:
            case 9:
            case 11:
                *totalDays += 31;
                break;
            case 1:
                *totalDays += 28;
                break;
            default:
                *totalDays += 30;
                break;
        }
    }

    for (int i = 0; i < *days; i++)
    {
        *totalDays += 1;
    }

    free(years);
    free(month);
    free(days);
    
    return totalDays;
    //free(totalDays);
}

void PrintPlantAfterDate() {

    //Общее количество растений высаженных после заданной даты
    printf("Ваша дата.\n");
    Date* userDate = ConsoleReadData();

    int allCountPlants = 0;

    //TODO: Баг завязанный на времени
    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;

        if (_plants[i].DateOfPlanting.year >= userDate->year) {
            if (_plants[i].DateOfPlanting.month >= userDate->month) {
                if (_plants[i].DateOfPlanting.day > userDate->day) {
                    allCountPlants += _plants[i].PlantingCount;
                }
            }
        }
    }

    char* charDate = DateToChars(*userDate);
    printf("Всего растений посажено после %s: %d\n", charDate, allCountPlants);

    free(charDate);
    free(userDate);
}


Date* ConsoleReadData() {

    Date* dt = malloc(sizeof(Date));

    while (1) {
        printf("Введите год: ");
        scanf("%d", &dt->year);
        if (dt->year <= 0)
        {
            printf("Неправильный год.\n");
        }
        else
        {
            break;
        }
    }

    while (1) {
        printf("Введите месяц: ");
        scanf("%d", &dt->month);
        if (dt->month > 12 || dt->month <= 0) {
            printf("Формат месяца не правильный\n");
        }
        else 
        {
            break;
        }
    }

    while (1) {
        printf("Введите день: ");
        scanf("%d", &dt->day);

        int result = 0;

        switch (dt->month)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:

                if (dt->day >= 1 && dt->day <= 31)
                {
                    result = 1;
                }

                break;

            case 2:
                if (dt->day >= 1 && dt->day <= 28)
                {
                    result = 1;
                }
                break;

            default:

                if (dt->day >= 1 && dt->day <= 30)
                {
                    result = 1;
                }

                break;
        }

        if (result == 0) {
            printf("Не существующий день. Заного.\n");
        }
        else {
            break;
        }
    }

    return dt;
}

void PrintAllPlants() {

    printf("\n");
    PrintTitle();

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;
        PrintPlant(&_plants[i]);
    }
}

void PrintTitle() {

    printf("\n");
    printf("|%-3s|%-25s|%-12s|%-15s|%-8s|%-11s|\n", "ID", "Назв. растения", "Дата посадки", "Дата созревания", "Высажено", "Тенелюбивое");

}

void PrintPlant(Plant* plant) 
{
    char* dtPlanting = DateToChars(plant->DateOfPlanting);
    char* dtMaturationPlant = DateToChars(plant->DateMaturationPlant);
    char* blText = BoolToText(plant->LoveShade);

    printf("|%-3d|%-25s|%-12s|%-15s|%-8d|%-11s|\n", 
        plant->Id,
        plant->Name,

        dtPlanting,
        dtMaturationPlant,

        plant->PlantingCount,
        blText
        );

    free(dtPlanting);
    free(dtMaturationPlant);
    free(blText);
}

char* DateToChars(const Date dt) {
    char* dtText = malloc((char)12);

    if (dt.day >= 10) {
        strcpy(dtText, IntToStr(&dt.day));
    }
    else 
    {
        strcpy(dtText, "0");
        strcat(dtText, IntToStr(&dt.day));
    }
    strcat(dtText, ".");

    if (dt.month >= 10) {
        strcat(dtText, IntToStr(&dt.month));
    }
    else 
    {
        strcat(dtText, "0");
        strcat(dtText, IntToStr(&dt.month));
    }
    strcat(dtText, ".");

    strcat(dtText, IntToStr(&dt.year));

    return dtText;
}

char* BoolToText(const _Bool* bl) {
    char* unswer = malloc((char)4);

    if (bl == 0) {
        strcpy(unswer, "Нет");
    }
    else 
    {
        strcpy(unswer, "Да");
    }

    return unswer;
}

void RemoveNL(char* msg) {
    msg[strcspn(msg, "\n\r")] = 0;
}

char* IntToStr(const int* number)
{
    char* numStr[15];

    sprintf(numStr, "%d", *number);

    return numStr;
}

void Save()
{
    FILE* file = fopen(FILE_NAME, "w");

    if (file == NULL)
    {
        printf("Файл не найден.\n");
        return;
    }

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;

        char txtSave[SAVE_LOAD_LENGTH];

        //Save Name
        strcpy(txtSave, _plants[i].Name);
        strcat(txtSave, ";");

        //Save DateOfPlanting
        strcat(txtSave, IntToStr(&_plants[i].DateOfPlanting.day));
        strcat(txtSave, ";");
        strcat(txtSave, IntToStr(&_plants[i].DateOfPlanting.month));
        strcat(txtSave, ";");
        strcat(txtSave, IntToStr(&_plants[i].DateOfPlanting.year));
        strcat(txtSave, ";");

        //Save DateMaturationPlant
        strcat(txtSave, IntToStr(&_plants[i].DateMaturationPlant.day));
        strcat(txtSave, ";");
        strcat(txtSave, IntToStr(&_plants[i].DateMaturationPlant.month));
        strcat(txtSave, ";");
        strcat(txtSave, IntToStr(&_plants[i].DateMaturationPlant.year));
        strcat(txtSave, ";");

        //Save PlantingCount
        strcat(txtSave, IntToStr(&_plants[i].PlantingCount));
        strcat(txtSave, ";");

        //Save LoveShade
        strcat(txtSave, IntToStr(&_plants[i].LoveShade));
        strcat(txtSave, ";");

        //New line
        strcat(txtSave, "\n");

        //Write
        fputs(txtSave, file);
    }

    fclose(file);
}

void Load()
{
    FILE* file = fopen(FILE_NAME, "r");

    if (file == NULL) 
    {
        printf("Файл не найден.\n");
        return;
    }

    char text[SAVE_LOAD_LENGTH];

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        _plants[i].Id = -1;
    }

    while (fgets(text, SAVE_LOAD_LENGTH, file) != NULL)
    {
        RemoveNL(text);

        char* plantsText = strtok(text, ";");

        for (int i = 0; i < ARR_LENGTH; i++)
        {
            if (_plants[i].Id != -1) continue;

            _plants[i].Id = (i + 1);

            //name
            strcpy(_plants[i].Name, plantsText);
            plantsText = strtok(NULL, ";");

            //datePlanting
            _plants[i].DateOfPlanting.day = atoi(plantsText);
            plantsText = strtok(NULL, ";");
            _plants[i].DateOfPlanting.month = atoi(plantsText);
            plantsText = strtok(NULL, ";");
            _plants[i].DateOfPlanting.year = atoi(plantsText);
            plantsText = strtok(NULL, ";");
            
            //Date da
            _plants[i].DateMaturationPlant.day = atoi(plantsText);
            plantsText = strtok(NULL, ";");
            _plants[i].DateMaturationPlant.month = atoi(plantsText);
            plantsText = strtok(NULL, ";");
            _plants[i].DateMaturationPlant.year = atoi(plantsText);
            plantsText = strtok(NULL, ";");

            //CountPlanting
            _plants[i].PlantingCount = atoi(plantsText);
            plantsText = strtok(NULL, ";");

            //Love
            _plants[i].LoveShade = atoi(plantsText);
            plantsText = strtok(NULL, ";");

            _plants[i].CountDays = *DaysDifference(&_plants[i].DateOfPlanting, &_plants[i].DateMaturationPlant);

            break;
        }

    }

    fclose(file);
}