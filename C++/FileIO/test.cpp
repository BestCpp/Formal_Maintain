
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Date
{
public:
    Date(int year = 1,int month = 1,int day = 1)
        :_year(year),_month(month),_day(day)
    {}
    
    friend ostream& operator<<(ostream& out,const Date& d);
    friend istream& operator>>(istream& out,Date& d);

private:
    int _year;
    int _month;
    int _day;
};

ostream& operator<<(ostream& out,const Date& d)
{
    out << d._year << " " << d._month << " " << d._day;
    return out;
}

istream& operator>>(istream& in,Date& d)
{
    in >> d._year >> d._month >> d._day;
    return in;
}

int main()
{
    //熟悉stringIO  -> 可以把所有东西转化成字符串
    Date d1(2023,4,22);
    ostringstream oss;
    oss << d1 << endl;

    //string s1 = oss.str();
    //cout << s1 << endl;

    Date d2;
    istringstream iss(oss.str());
    iss >> d2;
    cout << d2 << endl;
    return 0;
}


// int main()
// {
//     //熟悉文件IO
//     Date d(2023,4,11);
//     ofstream ofs("./test.txt");
//     ofs << d << endl;

//     Date d1;
//     ifstream ifs("./test.txt");
//     ifs >> d1;
//     cout << d1 << endl;
//     return 0;
// }