#include "md5.h"
#include <boost\filesystem.hpp>
#include <iostream>
#include <sstream>
#include <boost\locale.hpp>
#include "simhash.h"
#include <iomanip>

using namespace text_finger_print;
using namespace boost::filesystem;
using namespace std;
using namespace boost::locale;

std::string hexdigest(FingerData data)
{ 
    std::stringstream ss;
    ss << "0x";

    for (int i = 0; i < 16; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)data.data[i];

    return ss.str();
}
int Count(int x)
{
    int sum = 0;
    while (x)
    {
        sum++;
        x = x&(x - 1);
    }

    return sum;
}

int main()
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << 11;
    cout << ss.str();

    directory_iterator end_iter;
    int index = 0;
    FingerData data;

    for (directory_iterator iter("D:\\ÓßÇé·ÖÎö\\TrainingText\\tmp"); iter != end_iter; iter++)
    {
        index++;
        string strFile = iter->path().string();
        ifstream ifs(strFile);
        stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        wstring str = conv::to_utf<wchar_t>(ss.str(), string("gb2312"));

        SimHash simhash;
        FingerData fd = simhash.GenerateFingerPrinter(str);
        cout << strFile.substr(strFile.rfind('_')) << "\n" << hexdigest(fd) << endl;

        if (index > 1)
        {
            unsigned char c;
            int countOne = 0;
            for (int i = 0; i < 16; i++)
            {
                c = fd.data[i] ^ data.data[i];

                countOne += Count(c);
            }

            cout << "Compare Result: " << countOne << endl;
        }

        memcpy(data.data, fd.data, 16);
    }

    return 0;
}

