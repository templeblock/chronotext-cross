#include "chr/ResourceBuffer.h"

#if defined(CHR_FS_APK)
  #include "android/JNI.h"
#elif defined(CHR_FS_JS_EMBED) || defined(CHR_FS_JS_PRELOAD)
  #include <sys/mman.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

using namespace std;

namespace chr
{
  ResourceBuffer::~ResourceBuffer()
  {
    unlock();
  }

  bool ResourceBuffer::lock(const fs::path &relativePath)
  {
    unlock();

    #if defined(CHR_FS_RC)
      if (!locked)
      {
        auto basePath = fs::path("res") / relativePath;
        auto found = win::RESOURCES.find(basePath.generic_string());

        if (found != win::RESOURCES.end())
        {
          int resId = found->second;
          HRSRC infoHandle = ::FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);

          if (infoHandle)
          {
            HGLOBAL handle = ::LoadResource(NULL, infoHandle);

            if (handle)
            {
              _size = ::SizeofResource(NULL, infoHandle);
              _data = ::LockResource(handle);

              locked = true;
              return true;
            }
          }
        }
      }
    #elif defined(CHR_FS_APK)
      asset = AAssetManager_open(android::assetManager, relativePath.c_str(), AASSET_MODE_BUFFER);

      if (asset)
      {
        _size = AAsset_getLength(asset);
        _data = AAsset_getBuffer(asset);

        locked = true;
        return true;
      }
    #elif defined(CHR_FS_JS_EMBED) || defined(CHR_FS_JS_PRELOAD)
      auto basePath = fs::path("res") / relativePath;
      auto fd = open(basePath.c_str(), O_RDONLY);

      if (fd != -1)
      {
        struct stat stats;
        
        if ((fstat(fd, &stats) != -1) && (stats.st_size > 0))
        {
          _size = stats.st_size;
          _data = mmap(nullptr, _size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
          
          close(fd);
          
          if (_data != MAP_FAILED)
          {
            locked = true;
            return true;
          }
          
          _size = 0;
          _data = nullptr;
        }
      }
    #else
      assert(hasFileResources());

      auto basePath = getResourceFilePath(relativePath);
      fs::ifstream in(basePath, ios::binary | ios::ate);

      if (in)
      {
        auto fileSize = in.tellg();
        in.seekg(0, ios::beg);

        char *fileData = new char[fileSize];
        in.read(fileData, fileSize);

        _size = fileSize;
        _data = fileData;

        locked = true;
        return true;
      }
    #endif

    return false;
  }
        
  void ResourceBuffer::unlock()
  {
    #if defined(CHR_FS_APK)
      if (asset)
      {
        AAsset_close(asset);
        asset = nullptr;
      }
    #elif defined(CHR_FS_JS_EMBED) || defined(CHR_FS_JS_PRELOAD)
      if (_data && _size)
      {
        munmap(_data, _size);
      }
    #elif !defined(CHR_FS_RC)
      assert(hasFileResources());

      if (_data && _size)
      {
        delete[] (char*)_data;
      }
    #endif

    #if !defined(CHR_FS_RC)
      _size = 0;
      _data = nullptr;

      locked = false;
    #endif
  }

  size_t ResourceBuffer::size() const
  {
    return _size;
  }

  const void* ResourceBuffer::data() const
  {
    return _data;
  }
}
