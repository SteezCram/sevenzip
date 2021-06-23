#include <string>




struct ProgressCallback
{
    uint64_t total;
    int progress;
    int ratio;
    std::string fileProcessed;
};