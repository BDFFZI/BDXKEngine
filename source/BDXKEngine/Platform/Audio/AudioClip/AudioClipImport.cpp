#include "AudioClipImport.h"

#include <fstream>

namespace BDXKEngine
{
    ObjectPtr<AudioClip> AudioClipImport::Wav(const std::string& fileName)
    {
        std::ifstream audioFile = std::ifstream{fileName, std::ifstream::binary};
        if (audioFile.is_open() == false)throw std::exception("音频文件打开失败");

        //RIFF
        audioFile.ignore(4);
        int fileSize;
        audioFile.read(reinterpret_cast<char*>(&fileSize), sizeof fileSize);

        //WAVE | XWMA
        audioFile.ignore(4);

        //fmt
        audioFile.ignore(4);
        int fmtSize;
        audioFile.read(reinterpret_cast<char*>(&fmtSize), sizeof fmtSize);
        std::vector<char> fmt = {};
        fmt.resize(fmtSize);
        audioFile.read(fmt.data(), fmtSize);

        //data
        audioFile.ignore(4);
        int dataSize;
        audioFile.read(reinterpret_cast<char*>(&dataSize), sizeof dataSize);
        std::vector<char> data = {};
        data.resize(dataSize);
        audioFile.read(data.data(), dataSize);

        return AudioClip::Create(fmt, data);
    }
}
