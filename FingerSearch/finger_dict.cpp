#include "finger_dict.h"

namespace finger_print_search
{

FingerDict::FingerDict()
{
    for (int i = 0; i < 8; i++)
    {
        m_pFinger[i] = new std::vector<unsigned int>[65536];
    }
}


FingerDict::~FingerDict()
{
    for (int i = 0; i < 8; i++)
    {
        delete[] m_pFinger[i];
    }
}

bool FingerDict::ReadFingerprint(std::string strPath)
{
    return true;
}

bool FingerDict::AddFingerData(const FingerData& data, const unsigned int id)
{
    using namespace std;

    const unsigned short* p = (const unsigned short*)data.data;

    for (int i = 0; i < 8; i++)
    {
        vector<unsigned int>* pVct = &m_pFinger[i][p[i]];

        for (vector<unsigned int>::iterator iter = pVct->end(); iter != pVct->begin(); iter++)
        {
            if (*(iter - 1) < id)
                pVct->insert(iter, id);
        }
    }

    return true;
}

std::vector<unsigned int> FingerDict::SearchFingerprint(const FingerData& data)
{
    using namespace std;
    const unsigned short* p = (const unsigned short*)data.data;

    list<unsigned int> listID;
    listID.insert(listID.end(), m_pFinger[0][p[0]].begin(), m_pFinger[0][p[0]].begin());

    for (int i = 1; i < 8; i++)
    {
        list<unsigned int>::iterator itList = listID.begin();
        vector<unsigned int>::iterator itVct = m_pFinger[i][p[i]].begin();
        
        while (itList != listID.end() && itVct != m_pFinger[i][p[i]].end())
        {
            if (*itList > *itVct)
            {
                listID.insert(itList, *itVct);
                itVct++;
            }
            else
                itList++;
        }

        if (itVct != m_pFinger[i][p[i]].end())
            listID.insert(itList, itVct, m_pFinger[i][p[i]].end());
    }

    int id = 0;
    int iCount = 0;
    vector<unsigned int> vctResult;
    listID.push_back(0xFFFFFFFF);
    for (list<unsigned int>::iterator iter = listID.begin(); iter != listID.end(); iter++)
    {
        if (id != *iter)
        {
            if (iCount >= 2)
            {
                if (IsSimilar(data, m_mapFinger.find(id)->second))
                    vctResult.push_back(id);
            }

            id = *iter;
            iCount = 1;
        }
        else
            iCount++;
    }

    return vctResult;
}

bool FingerDict::IsSimilar(const FingerData& data1, const FingerData& data2)
{
    unsigned char c;
    int countOne = 0;
    for (int i = 0; i < 16; i++)
    {
        c = data1.data[i] ^ data2.data[i];

        countOne += CountOne(c);
    }

    if (countOne < 6)
        return true;
    else
        return false;
}

int FingerDict::CountOne(int x)
{
    int sum = 0;
    while (x)
    {
        sum++;
        x = x&(x - 1);
    }

    return sum;
}
}