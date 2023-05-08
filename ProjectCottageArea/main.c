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
    Date DateOfPlanting; //���� �������
    Date DateMaturationPlant; //���� ����������
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
    //strcpy - �����������
    //strcat - ��������
    //strtok - split C#
    //strstr - contains
    //void* ptr

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char* locale = setlocale(LC_ALL, "");


    Init();
    Load();
    MainLoop();

    printf("\n����� ���������.\n");
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
        printf("\n������� ����\n");

        printf("1. ������� ��� ��������\n");
        printf("2. ���������� ������ ��������\n");
        printf("3. �������� ��������\n");
        printf("4. �����\n");
        printf("5. ���������\n");
        printf("6. ���������\n");

        printf("\n");

        printf("-1: �����\n");

        printf("\n");

        printf("�����: ");
        int userSelect;
        scanf("%d", &userSelect);

        //printf("������������ ������: %d\n", userSelect);

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
            printf("������ ����. ���������� ������!\n");
            break;
        }

    }

}

void AddRow() {

    Plant newPlant;

    newPlant.Id = -1;
    
    getchar();

    printf("������������ ��������: ");
    fgets(newPlant.Name, (sizeof(newPlant.Name) / sizeof(newPlant.Name[0])), stdin);
    RemoveNL(newPlant.Name);

    printf("���� �������.\n");

    Date* dt = ConsoleReadData();
    
    newPlant.DateOfPlanting.day = dt->day;
    newPlant.DateOfPlanting.month = dt->month;
    newPlant.DateOfPlanting.year = dt->year;

    free(dt);

    printf("��������� ���� ����������.\n");

    dt = ConsoleReadData();

    newPlant.DateMaturationPlant.day = dt->day;
    newPlant.DateMaturationPlant.month = dt->month;
    newPlant.DateMaturationPlant.year = dt->year;

    free(dt);

    printf("���-�� ���������� �����������: ");
    scanf("%d", &newPlant.PlantingCount);


    printf("��������� �� �������� � �����������? (0 - ���, 1 - ��): ");
    scanf("%d", &newPlant.LoveShade);

    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id != -1) continue;

        newPlant.Id = (i + 1);
        _plants[i] = newPlant;
        printf("������ ��������� ��� �����: %d\n", _plants[i].Id);
        return;

    }

}

void RemoveRow() {
    int id = -1;
    printf("�������� ID ��� ��������: ");
    scanf("%d", &id);
    if (id <= 0) {
        printf("������ ID ����.\n");
        return;
    }

    _plants[id - 1].Id = -1;
}

void FindRecord() {
    //�) ���� �������� ����������� ����������� ��������; 
    //�) ������ ���������� �������� ���������� ����� �������� ����.
    while (1) {
        printf("��� ������ �����?\n");
        printf("1. ��� �������� ����������� ����������� ��������\n");
        printf("2. ����� ���������� �������� ���������� ����� �������� ����\n");
        printf("-1. �����.\n");

        printf("��� �����: ");
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
            printf("����� �� ����������. �������� ��� ���!\n\n");
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

    //�) ���� �������� ����������� ����������� ��������; 
    printf("����� �����������: %d\n", countArray);

    Plant* plantsLoveShadow = malloc(sizeof(Plant) * countArray);

    int index = 0;
    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (_plants[i].Id == -1) continue;
        if (_plants[i].LoveShade == 0) continue;
        plantsLoveShadow[index] = _plants[i];
        index++;
    
    }

    //���������� ���������
    for (int i = 0; i < countArray - 1; i++) {
        for (int j = 0; j < countArray - i - 1; j++)
        {
            //���� ������� � '�������� � ��������' �� '�������� � ��������' ����� ���� < �� > � ��������
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

    //���� �������, ����� ��� �����������. ������ 3 - countArray
    printf("|%-3s|%-25s|%-12s|%-15s|%-8s|%-11s|%-12s|\n", "ID", "����. ��������", "���� �������", "���� ����������", "��������", "�����������", "���� �����");
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

    //����� ���������� �������� ���������� ����� �������� ����
    printf("���� ����.\n");
    Date* userDate = ConsoleReadData();

    int allCountPlants = 0;

    //TODO: ��� ���������� �� �������
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
    printf("����� �������� �������� ����� %s: %d\n", charDate, allCountPlants);

    free(charDate);
    free(userDate);
}


Date* ConsoleReadData() {

    Date* dt = malloc(sizeof(Date));

    while (1) {
        printf("������� ���: ");
        scanf("%d", &dt->year);
        if (dt->year <= 0)
        {
            printf("������������ ���.\n");
        }
        else
        {
            break;
        }
    }

    while (1) {
        printf("������� �����: ");
        scanf("%d", &dt->month);
        if (dt->month > 12 || dt->month <= 0) {
            printf("������ ������ �� ����������\n");
        }
        else 
        {
            break;
        }
    }

    while (1) {
        printf("������� ����: ");
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
            printf("�� ������������ ����. ������.\n");
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
    printf("|%-3s|%-25s|%-12s|%-15s|%-8s|%-11s|\n", "ID", "����. ��������", "���� �������", "���� ����������", "��������", "�����������");

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
        strcpy(unswer, "���");
    }
    else 
    {
        strcpy(unswer, "��");
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
        printf("���� �� ������.\n");
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
        printf("���� �� ������.\n");
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