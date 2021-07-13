#include <string>




/// <summary>
/// Progress callback structure
/// </summary>
struct ProgressCallback
{
    /// <summary>
    /// Total bytes to compress
    /// </summary>
    uint64_t total;
    /// <summary>
    /// Progress of the operation
    /// </summary>
    int progress;
    /// <summary>
    /// Compression ration
    /// </summary>
    int ratio;
    /// <summary>
    /// File currently processed
    /// </summary>
    std::string fileProcessed;
};