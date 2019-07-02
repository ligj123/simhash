#ifndef SIM_HASH_H
#define SIM_HASH_H
#include <string>
#include <vector>
namespace text_finger_print{

struct FingerData{
    unsigned char data[16];
};

class SimHash
{
public:
    SimHash();
    ~SimHash();

    FingerData GenerateFingerPrinter(const std::string text);
    FingerData GenerateFingerPrinter(const std::wstring text);

protected:
//    void SpliteText(const std::wstring& text, std::vector<std::wstring>& vct);
//    void CountWeight(const std::vector<std::wstring>& vct, FingerData& data);

    void SpliteText(const std::wstring& text, std::wstring& strResult);
    void CountWeight(const std::wstring& text, FingerData& data);
private:
    const int m_iCenterWeight = 10;
};

}
#endif