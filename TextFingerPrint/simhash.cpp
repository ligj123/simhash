#include "simhash.h"
#include "md5.h"
#include <boost/locale.hpp>

namespace text_finger_print{

SimHash::SimHash()
{
}


SimHash::~SimHash()
{
}

FingerData SimHash::GenerateFingerPrinter(const std::string text)
{
    return GenerateFingerPrinter(boost::locale::conv::to_utf<wchar_t>(text, "utf-8"));
}

FingerData SimHash::GenerateFingerPrinter(const std::wstring text)
{
    using namespace std;
    FingerData data;
    memset(data.data, 0, 16);
    wstring strResult;
    SpliteText(text, strResult);
    CountWeight(strResult, data);

    //vector<wstring> vct;
    //SpliteText(text, vct);
    //CountWeight(vct, data);

    return data;
}

void SimHash::SpliteText(const std::wstring& text, std::wstring& result)
{
    using namespace std;
    const wstring sBreak = L"\t\r\n¡£¡¢¡°¡±¡¨¡´¡µ¡¶¡·¡¸¡¹¡º¡»¡¾¡¿¨“¡ş¡²¡³¡¼¡½¨”¨•©o©p©q©r©s©t©u©v©w©x©y©z©{©|©}©~©€©©‚©ƒ©„©…©†©‡©ˆ£¡£¢££¡ç£¥£¦£§£¨£©£ª£«£¬£­£®£¯£º£»£¼£½£¾£¿£Û£Ü£İ£Ş£ß£à£û£ü£ı¡«!\"#$%&'()*+,-./:;<=>?[\\]^_`{|}~";

    result.clear();
    for (wstring::const_iterator iter = text.cbegin(); iter != text.cend(); iter++)
    {
        wchar_t c = *iter;
        if (c >= 0xFF01 && c <= 0xFF65)
            c -= 0xFEE0;
        if (c >= 'A' && c <= 'Z')
            c -= 32;

        if (sBreak.find(c) == wstring::npos)
        {
            if (c != ' ')
                result += c;
            continue;
        }
    }
}

void SimHash::CountWeight(const std::wstring& text, FingerData& data)
{
    int count = text.size();
    double arWeight[128];
    memset(arWeight, 0, sizeof(int)* 128);
    const wchar_t *pChar = text.c_str();

    for (int i = 0; i < count; i++)
    {
        int pos = 0;
        if (i < count / 2)
            pos = i + 1;
        else
            pos = count - i;

        //weight = weight * m_iCenterWeight / count + 1;
        double weight = 0;
        if (pos < count / 8)
            weight = 1;
        else if (pos < count * 3 / 8)
            weight = (double)(pos - count / 8) * 4 * m_iCenterWeight / count + 1;
        else
            weight = m_iCenterWeight + 1;

        //if (i % 10 == 0)
        //    std::cout << "i=" << i << "\tWeight=" << weight << std::endl;

        MD5 md5;
        md5.update((const unsigned char*)(pChar + i), sizeof (wchar_t));
        md5.finalize();
        unsigned char ar[16];
        memcpy(ar, md5.GetDigest(), 16);

        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                if ((ar[j] & (1 << k)) == 0)
                    arWeight[j * 8 + k] -= weight;
                else
                    arWeight[j * 8 + k] += weight;
            }
        }
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (arWeight[i * 8 + j] > 0)
                data.data[i] |= 1 << j;
            else
                data.data[i] &= ~(1 << j);
        }
    }
}

//void SimHash::SpliteText(const std::wstring& text, std::vector<std::wstring>& vct)
//{
//    using namespace std;
//    const wstring sBreak = L"\t\r\n¡£¡¢¡°¡±¡¨¡´¡µ¡¶¡·¡¸¡¹¡º¡»¡¾¡¿¨“¡ş¡²¡³¡¼¡½¨”¨•©o©p©q©r©s©t©u©v©w©x©y©z©{©|©}©~©€©©‚©ƒ©„©…©†©‡©ˆ£¡£¢££¡ç£¥£¦£§£¨£©£ª£«£¬£­£®£¯£º£»£¼£½£¾£¿£Û£Ü£İ£Ş£ß£à£û£ü£ı¡«!\"#$%&'()*+,-./:;<=>?[\\]^_`{|}~";
//
//    wstring str;
//
//    for (wstring::const_iterator iter = text.cbegin(); iter != text.cend(); iter++)
//    {
//        wchar_t c = *iter;
//        if (c >= 0xFF01 && c <= 0xFF65)
//            c -= 0xFEE0;
//        if (c >= 'A' && c <= 'Z')
//            c -= 32;
//
//        if (sBreak.find(c) == wstring::npos)
//        {
//            if (c != ' ')
//                str += c;
//            continue;
//        }
//
//        if (str.size() == 0)
//            continue;
//
//        vct.push_back(str);
//        str.clear();
//    }
//}
//
//void SimHash::CountWeight(const std::vector<std::wstring>& vct, FingerData& data)
//{
//    int count = vct.size();
//    int arWeight[128];
//    memset(arWeight, 0, sizeof(int)* 128);
//
//    for (int i = 0; i < count; i++)
//    {
//        int weight = 0;
//        if (i < count / 2)
//            weight = i + 1;
//        else
//            weight = count - i;
//
//        weight = weight * m_iCenterWeight / count + 1;
//        weight *= vct[i].size();
//
//        MD5 md5;
//        md5.update((const unsigned char*)vct[i].c_str(), vct[i].size() * sizeof (wchar_t));
//        md5.finalize();
//        unsigned char ar[16];
//        memcpy(ar, md5.GetDigest(), 16);
//
//        for (int j = 0; j < 16; j++)
//        {
//            for (int k = 0; k < 8; k++)
//            {
//                if ((ar[j] & (1 << k)) == 0)
//                    arWeight[j * 8 + k] -= weight;
//                else
//                    arWeight[j * 8 + k] += weight;
//            }
//        }
//    }
//
//    for (int i = 0; i < 16; i++)
//    {
//        for (int j = 0; j < 8; j++)
//        {
//            if (arWeight[i * 8 + j] > 0)
//                data.data[i] |= 1 << j;
//            else
//                data.data[i] &= ~(1 << j);
//        }
//    }
//}
}