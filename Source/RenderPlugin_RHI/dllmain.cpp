#include <Constants.hpp>
#include <Devices/ScreenDevice.hpp>
#include <GameFramework.hpp>
#include <Resources/MaterialManager.hpp>

namespace RenderPlugin
{
struct RenderPlugin_RHI : public GameFramework::RenderPlugin
{
  explicit RenderPlugin_RHI(const GameFramework::IPluginLoader & loader);
  virtual ~RenderPlugin_RHI() = default;

  virtual GameFramework::ScreenDeviceUPtr CreateScreenDevice(
    GameFramework::IWindow & window) override;

  virtual void Tick() override;

private:
  std::unique_ptr<RHI::IContext> m_context;
};

void RenderLog(RHI::LogMessageStatus status, const std::string & message)
{
  switch (status)
  {
    case RHI::LogMessageStatus::LOG_INFO:
      std::printf("INFO: - %s\n", message.c_str());
      break;
    case RHI::LogMessageStatus::LOG_WARNING:
      std::printf("WARNING: - %s\n", message.c_str());
      break;
    case RHI::LogMessageStatus::LOG_ERROR:
      std::printf("ERROR: - %s\n", message.c_str());
      break;
    case RHI::LogMessageStatus::LOG_DEBUG:
      std::printf("DEBUG: - %s\n", message.c_str());
      break;
  }
}

RenderPlugin_RHI::RenderPlugin_RHI(const GameFramework::IPluginLoader & loader)
{
  GameFramework::GetFileManager().Mount(g_shadersDirectory,
                                        GameFramework::CreateDirectoryMountPoint(
                                          loader.Path() / g_shadersDirectory));
  RHI::GpuTraits gpuTraits{};
  gpuTraits.require_presentation = true;
  m_context = CreateContext(gpuTraits, RenderLog);
}

GameFramework::ScreenDeviceUPtr RenderPlugin_RHI::CreateScreenDevice(
  GameFramework::IWindow & window)
{
  return std::make_unique<ScreenDevice>(*m_context, window);
}

void RenderPlugin_RHI::Tick()
{
  GetMaterialManager().DoTasks();
  m_context->ClearResources();
  m_context->TransferPass();
}

} // namespace RenderPlugin

/// creates global game instance
PLUGIN_API std::unique_ptr<GameFramework::IPluginInstance> CreateInstance(
  const GameFramework::IPluginLoader & loader)
{
  return std::make_unique<RenderPlugin::RenderPlugin_RHI>(loader);
}
