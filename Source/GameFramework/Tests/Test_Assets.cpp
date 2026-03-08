#include <fstream>
#include <string>

#include <Assets/AssetsRegistry.hpp>
#include <Assets/Utils.hpp>
#include <catch2/catch_test_macros.hpp>

static const std::filesystem::path s_testDir = "./AssetsTest";
static constexpr std::wstring_view s_MainModule = L"MainData";
static constexpr std::wstring_view s_ModModule1 = L"Mod1";
static constexpr std::wstring_view s_ModModule2 = L"Mod2";

namespace
{

void WriteAssetToBase(std::ofstream & f, GameFramework::AssetType type, std::filesystem::path path)
{
  f << GameFramework::Uuid::MakeRandomUuid().ToString() << ";"
    << GameFramework::details::AssetTypeToString(type) << ";" << path.string() << std::endl;
}

struct TestContext final
{
  TestContext();
  ~TestContext();
};

TestContext::TestContext()
{
  using namespace GameFramework;
  std::error_code ec;
  std::filesystem::create_directory(s_testDir, ec);
  if (ec)
  {
    auto msg = ec.message();
    throw std::runtime_error("Failed to create test directory");
  }
  std::filesystem::create_directory(s_testDir / s_MainModule);
  {
    std::ofstream f(s_testDir / s_MainModule / ".assetList");
    f << "uuid;type;path" << std::endl;
    WriteAssetToBase(f, AssetType::ShaderBinary, "Shaders/cube_frag.spv");
    WriteAssetToBase(f, AssetType::ShaderBinary, "Shaders/cube_vert.spv");
    WriteAssetToBase(f, AssetType::Picture, "Textures/Cube.png");
    WriteAssetToBase(f, AssetType::Picture, "Textures/Cube_n.png");
  }
  std::filesystem::create_directory(s_testDir / s_ModModule1);
  {
    std::ofstream f(s_testDir / s_ModModule1 / ".assetList");
    f << "uuid;type;path" << std::endl;
    WriteAssetToBase(f, AssetType::ShaderBinary, "Shaders/cube_frag.spv");
    WriteAssetToBase(f, AssetType::Picture, "Textures/Cube.png");
  }
  std::filesystem::create_directory(s_testDir / s_ModModule2);
  {
    std::ofstream f(s_testDir / s_ModModule2 / ".assetList");
    f << "uuid;type;path" << std::endl;
    WriteAssetToBase(f, AssetType::ShaderBinary, "Shaders/cube_vert.spv");
    WriteAssetToBase(f, AssetType::Picture, "Textures/Cube.png");
  }
}

TestContext::~TestContext()
{
  std::error_code ec;
  std::filesystem::remove_all(s_testDir, ec);
}

static ::TestContext ctx;
} // namespace

using namespace GameFramework;

TEST_CASE("Upload databases", "[AssetsRegistry]")
{
  GetAssetsRegistry().LoadDatabase(s_testDir / s_MainModule);
  GetAssetsRegistry().LoadDatabase(s_testDir / s_ModModule1);
  GetAssetsRegistry().LoadDatabase(s_testDir / s_ModModule2);

  auto * asset = GetAssetsRegistry().GetAsset("Shaders/cube_frag.spv");
  REQUIRE(asset != nullptr);
  REQUIRE(asset->GetType() == AssetType::ShaderBinary);

  GetAssetsRegistry().DeleteDatabase(s_testDir / s_MainModule);
  GetAssetsRegistry().DeleteDatabase(s_testDir / s_ModModule1);
  GetAssetsRegistry().DeleteDatabase(s_testDir / s_ModModule2);
}
