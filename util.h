#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <filesystem>
#include <array>
#include <vector>

namespace util {

  template<typename T> struct Vector2
  {
    T x, y;
    T* data() { return &x; }
    T& operator[](size_t _i) { assert(_i < 2); return (&x)[_i]; }
    constexpr T const& operator[](size_t _i) const { assert(_i < 2); return (&x)[_i]; }

    constexpr Vector2() = default;
    constexpr Vector2(const Vector2& _v) : x(_v.x), y(_v.y) {}
    constexpr explicit Vector2(T _s) : x(_s), y(_s) {}
    constexpr explicit Vector2(T _x, T _y) : x(_x), y(_y) {}
    constexpr Vector2(std::initializer_list<T> init)
    {
      int i = 0;
      for (auto v : init)
      {
        this->operator[](i++) = v;
      }
    }
  };

  template<typename T> struct Vector4
  {
    T x, y, z, w;
    T& operator[](unsigned _i) { assert(_i < 4); return (&x)[_i]; }
    constexpr T const& operator[](unsigned _i) const { assert(_i < 4); return (&x)[_i]; }

    constexpr Vector4() = default;
    constexpr Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    constexpr explicit Vector4(T _s) : x(_s), y(_s), z(_s), w(_s) {}
    constexpr explicit Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
  };

  template<typename Type, unsigned NumDimensions> struct Image
  {
    using type = Type;
    auto GetNumPixels() const { return mElements.size(); }
    Type& At(uint32_t index) { return mElements[index]; }
    Type* GetData() { return mElements.data(); }
    const Type* GetData() const { return mElements.data(); }
    std::vector<Type>& GetVector() { return mElements; }
    const std::vector<Type>& GetVector() const { return mElements; }
    auto GetDimension(unsigned i) const { return mDimensions[i]; }
    void SetDimension(unsigned i, unsigned length) { mDimensions[i] = length; }
    auto GetSize() const { return mElements.size() * sizeof(Type); }
    void Resize() {
      unsigned size = 1;
      for (auto d : mDimensions)
        size *= d;
      mElements.resize(size);
    }

  private:
    std::array<unsigned, NumDimensions> mDimensions = {};
    std::vector<Type> mElements;
  };

  //-----------------------------------------------------------------------------

  template<typename T> struct Image2D : Image<T, 2>
  {
    Image2D(unsigned w, unsigned h)
    {
      this->SetDimension(0, w);
      this->SetDimension(1, h);
      this->Resize();
    }
    auto GetWidth() const { return this->GetDimension(0); }
    auto GetHeight() const { return this->GetDimension(1); }
    T& At(unsigned x, unsigned y) { return this->GetVector()[GetWidth() * y + x]; }
    const T& At(unsigned x, unsigned y) const { return this->GetVector()[GetWidth() * y + x]; }
  };

  using Uchar4 = Vector4<unsigned char>;
  using Int2 = Vector2<int>;
  using Uint2 = Vector2<unsigned>;
  using ImageRGBA8 = Image2D<Uchar4>;

  bool SaveImage(const std::filesystem::path& path, const ImageRGBA8& image)
  {
    auto s = path.string();
    auto c = s.c_str();
    const char* filename = s.c_str();
    const int bytes_per_pixel = 4;

    int result = stbi_write_png(
      filename, image.GetWidth(), image.GetHeight(), bytes_per_pixel,
      reinterpret_cast<const void*>(image.GetData()),
      image.GetWidth() * bytes_per_pixel
    );
    return (result > 0);
  }

}