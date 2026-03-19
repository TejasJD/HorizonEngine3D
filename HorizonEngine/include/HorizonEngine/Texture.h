#ifndef HORIZON_ENGINE_TEXTURE_H
#define HORIZON_ENGINE_TEXTURE_H

#include <cstdint>
#include <filesystem>
#include <memory>

namespace Hzn
{

class Texture
{
  private:
    class Key
    {
        friend class Texture;
    };

  public:
    static auto Create(std::filesystem::path const &filePath) -> std::shared_ptr<Texture>;

    Texture(Key)
    {
    }

    Texture(Texture const &) = delete;
    Texture &operator=(Texture const &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;
    ~Texture();

    auto Cleanup() -> void;

    auto Bind(uint32_t slot) const -> void;

  private:
    int32_t mWidth{0};
    int32_t mHeight{0};
    int32_t mComponents{0};
    uint32_t mTextureID{0U};
};

} // namespace Hzn

#endif // HORIZON_ENGINE_TEXTURE_H