#ifndef FINDER_DICT_H
#define FINDER_DICT_H
#include <string>
#include <vector>
#include <unordered_map>

namespace finger_print_search
{
struct FingerData{
    unsigned char data[16];
};

class FingerDict
{
public:
    FingerDict();
    ~FingerDict();

    bool ReadFingerprint(std::string strPath);
    std::vector<unsigned int> SearchFingerprint(const FingerData& data);
    bool AddFingerData(const FingerData& data, const unsigned int id);

protected:
    bool IsSimilar(const FingerData& data1, const FingerData& data2);
    int CountOne(int x);

protected:
    std::vector<unsigned int>* m_pFinger[8];
    std::unordered_map<unsigned int, FingerData> m_mapFinger;
};

}
#endif // FINDER_DICT_H