#include "chr/io/BinaryOutputStream.h"
#include "chr/glm.h"

#include <fcntl.h> // FOR MINGW

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    BinaryOutputStream::~BinaryOutputStream()
    {
      close();
    }

    void BinaryOutputStream::close()
    {
      if (out)
      {
        delete codedOutput;
        codedOutput = nullptr;

        delete rawOutput;
        rawOutput = nullptr;

        delete out;
        out = nullptr;
      }
    }

    BinaryOutputStream::BinaryOutputStream(const fs::path &filePath)
    {
      out = new fs::ofstream(filePath, ios::out | ios::binary);

      if (out->good())
      {
        rawOutput = new protobuf::io::OstreamOutputStream(out);
        codedOutput = new protobuf::io::CodedOutputStream(rawOutput);
      }
    }

    template<>
    void BinaryOutputStream::write(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void BinaryOutputStream::write(int32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void BinaryOutputStream::write(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void BinaryOutputStream::write(int64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void BinaryOutputStream::write(float value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value));
      }
    }

    template<>
    void BinaryOutputStream::write(double value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(encodeDouble(value));
      }
    }

    template<>
    void BinaryOutputStream::write(glm::vec2 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value.x));

        if (good())
        {
          codedOutput->WriteLittleEndian32(encodeFloat(value.y));
        }
      }
    }

    template<>
    void BinaryOutputStream::write(glm::vec3 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value.x));

        if (good())
        {
          codedOutput->WriteLittleEndian32(encodeFloat(value.y));

          if (good())
          {
            codedOutput->WriteLittleEndian32(encodeFloat(value.z));
          }
        }
      }
    }

    void BinaryOutputStream::writeVarint32(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint32(value);
      }
    }

    void BinaryOutputStream::writeVarint64(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint64(value);
      }
    }

    void BinaryOutputStream::writeBytes(uint8_t *data, size_t size)
    {
      codedOutput->WriteRaw(data, size);
    }

    void BinaryOutputStream::writeString(const string &value, size_t size)
    {
      if (!value.empty())
      {
        if (good())
        {
          codedOutput->WriteRaw(value.data(), size);
        }
      }
    }

    void BinaryOutputStream::writeString(const string &value)
    {
      if (!value.empty())
      {
        if (good())
        {
          codedOutput->WriteVarint32(value.size());

          if (good())
          {
            codedOutput->WriteRaw(value.data(), value.size());
          }
        }
      }
    }

    bool BinaryOutputStream::good() const
    {
      return codedOutput && !codedOutput->HadError();
    }
  }
}