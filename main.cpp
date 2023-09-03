#include "util.h"
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <thread>

util::Uchar4 Pixel(util::Uint2 inPos, util::Uint2 inRes, float inProgress)
{
  unsigned r = unsigned(float(inPos.x) / (inRes.x - 1) * 255);
  unsigned g = unsigned(float(inPos.y) / (inRes.y - 1) * 255);
  unsigned b = unsigned(inProgress * 255);
  return util::Uchar4(r, g, b, 255);
}

void Render(float inProgress, util::ImageRGBA8& outImage)
{
  unsigned w = outImage.GetWidth();
  unsigned h = outImage.GetHeight();
  for (unsigned y = 0; y < h; ++y)
  {
    for (unsigned x = 0; x < w; ++x)
    {
      outImage.At(x, y) = Pixel({ x, y }, { w, h }, inProgress);
    }
  }
}

void Export(int inFrame, const util::ImageRGBA8& inImage)
{
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(3) << inFrame;
  const std::filesystem::path current_path = std::filesystem::current_path();
  std::string filename = ss.str() + ".png";
  std::string filepath = (current_path / filename).string();

  util::SaveImage(filepath, inImage);
  std::cout << filename << std::endl;
}

int main(int argc, char** argv)
{
  std::cout << "start." << std::endl;
  const int cMaxFrameNum = 600;
  std::vector<std::thread> threads(cMaxFrameNum);
  for (int i = 0; i < cMaxFrameNum; ++i)
  {
    threads.push_back(std::thread(
      [](int inFrame, int inMaxFrameNum)
      {
        util::ImageRGBA8 image(512, 512);
        float progress = float(inFrame) / (cMaxFrameNum - 1);
        Render(progress, image);
        Export(inFrame, image);
      }, i, cMaxFrameNum
    ));
    threads.back().join();
  }
  std::cout << "done." << std::endl;
  return 0;
}