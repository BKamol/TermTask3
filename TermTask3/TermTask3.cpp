#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
//ФИО, дата_рождения(отдельная структура), образование(В, С, Н), семейное_положение(Х, Ж), кол-во_детей(0-9), профессия(A-F)
//адрес(город улица дом кв, отдельная структура), телефон

struct date
{
    int day;
    int month;
    int year;
};

struct address
{
    char city[21];
    char street[20];
    int dom;
    int kv;
};

struct person
{
    char f_name[20];
    char s_name[20];
    char t_name[20];
    date b_date;
    char education;
    char family_status;
    int children;
    char profession;
    address adr;
    int phone;
};

void show_person(person pers)
{
    cout << pers.f_name << ' ' << pers.s_name << ' ' << pers.t_name << ' ';
    cout << pers.b_date.day << ' ' << pers.b_date.month << ' ' << pers.b_date.year << ' ';
    cout << pers.education << ' ' << pers.family_status << ' ' << pers.children << ' ' << pers.profession << ' ';
    cout << pers.adr.city << ' ' << pers.adr.street << ' ' << pers.adr.dom << ' ' << pers.adr.kv << ' ' << pers.phone << endl;
}

void string_to_date(string word, date& b_date) //записывает данные из строки в адрес
{
    if (word.length() == 8)
    {
        int num = 0;
        num = num * 10 + (word[0] - '0');
        num = num * 10 + (word[1] - '0');
        b_date.day = num;
        num = 0;
        num = num * 10 + (word[2] - '0');
        num = num * 10 + (word[3] - '0');
        b_date.month = num;
        num = 0;
        num = num * 10 + (word[4] - '0');
        num = num * 10 + (word[5] - '0');
        num = num * 10 + (word[6] - '0');
        num = num * 10 + (word[7] - '0');
        b_date.year = num;
    }
    else
    {
        cout << "Wrong length of date" << endl;
    }
    
}

void text_to_bin() //Создание бинарного файла из структур
{
    ifstream in("base.txt");
    ofstream out("base.bin", ios::binary);

    person pers;
    string word;
    while (in >> pers.f_name)
    {
        in >> pers.s_name;
        in >> pers.t_name;
        in >> word;
        string_to_date(word, pers.b_date);
        in >> pers.education;
        in >> pers.family_status;
        in >> pers.children;
        in >> pers.profession;
        in >> pers.adr.city;
        in >> pers.adr.street;
        in >> pers.adr.dom;
        in >> pers.adr.kv;
        in >> pers.phone;
        out.write((char*)&pers, sizeof(pers));
        show_person(pers);
    }

    in.close();
    out.close();
}

void reading_from_bin()
{
    //Формирует два рабочих файла с людьми, у которых образование выше начального, больше одного ребенка.
    ifstream in("base.bin", ios::binary);
    ofstream out1("file1.bin", ios::binary);
    ofstream out2("file2.bin", ios::binary);

    person pers;
    while (in.read((char*)&pers, sizeof(pers)))
    {
        if (pers.children >= 1 && pers.education != 'Н')
        {
            if (pers.adr.dom % 2 == 0)
            {
                out1.write((char*)&pers, sizeof(pers)); //Запись в первый рабочий файл
            }
            else
            {
                out2.write((char*)&pers, sizeof(pers)); //Запись во второй рабочий файл
            }
        }
    }

    in.close();
    out1.close();
    out2.close();
}

void retrieving_city_codes(string cities[], int city_codes[])
{
    //Записывает в два массива названия городов и их коды
    ifstream in("city.txt");
    string city;
    int i = 0;
    int code = 0;
    while (in >> city)
    {
        in >> code;
        cities[i] = city;
        city_codes[i] = code;
        //cout << cities[i] << ' ' << city_codes[i] << endl;
        i++;
    }

    in.close();
}

int search_code(string city, string cities[], int city_codes[])
{
    //Вернет код переданного города или -1 если города в списке нет
    int l = 0;
    int r = 48;
    int mid;
    bool flag = false;
    while (l <= r && !flag)
    {
        mid = (l + r) / 2;
        if (city == cities[mid])
        {
            flag = true;
        }
        else if (city > cities[mid])
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    if (flag) return city_codes[mid];
    return -1;
}

void printing_results(string cities[], int city_codes[])
{
    //Фамилия и инициалы, код города, номер дома
    //Стоит отметить, что Казань и Зеленодольск в файле перепутаны местами
    ifstream in1("file1.bin", ios::binary);
    ifstream in2("file2.bin", ios::binary);

    person pers;
    int code;
    string initials;

    cout << "|  " << setw(20) << left << "Фамилия и инициалы" << "| " << "Код города" << " | " << "Номер дома" << " |" << endl;
    cout << setfill('-') << setw(50) << "" << endl;
    cout << setfill(' ');

    while (in1.read((char*)&pers, sizeof(pers)))
    {
        initials = string(pers.f_name) + ' ' + string(pers.s_name)[0] + '.' + string(pers.t_name)[0] + '.';
        code = search_code(pers.adr.city, cities, city_codes);
        cout << "|  " << setw(20) << left << initials << "|   " << setw(8) << code << " |    " << setw(7) << pers.adr.dom << " |" << endl;
    }
    cout << setfill('-') << setw(50) << "" << endl;
    cout << setfill(' ');


    cout << "|  " << setw(20) << left << "Фамилия и инициалы" << "| " << "Код города" << " | " << "Номер дома" << " |" << endl;
    cout << setfill('-') << setw(50) << "" << endl;
    cout << setfill(' ');

    while (in2.read((char*)&pers, sizeof(pers)))
    {
        initials = string(pers.f_name) + ' ' + string(pers.s_name)[0] + '.' + string(pers.t_name)[0] + '.';
        code = search_code(pers.adr.city, cities, city_codes);
        cout << "|  " << setw(20) << left << initials << "|   " << setw(8) << code << " |    " << setw(7) << pers.adr.dom << " |" << endl;
    }
    cout << setfill('-') << setw(50) << "" << endl;
    cout << setfill(' ');

    in1.close();
    in2.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    /*date d;
    string_to_date("07101941", d);
    cout << d.day << ' ' << d.month << ' ' << d.year << endl;*/
    //text_to_bin();
    //reading_from_bin();

    string cities[49];
    int city_codes[49];
    retrieving_city_codes(cities, city_codes);

    printing_results(cities, city_codes);
}

