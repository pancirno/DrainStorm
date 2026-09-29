#pragma once

#include <windows.h>

#include <filesystem>

namespace drain_storm {

class BitmapSurface {
public:
    BitmapSurface() = default;
    ~BitmapSurface();

    BitmapSurface(const BitmapSurface&) = delete;
    BitmapSurface& operator=(const BitmapSurface&) = delete;

    bool Load(const std::filesystem::path& path);
    void DrawCentered(HDC destination, const RECT& bounds) const;
    void DrawRegion(HDC destination, int destinationX, int destinationY,
                    int sourceX, int sourceY, int width, int height) const;
    [[nodiscard]] bool IsLoaded() const noexcept { return bitmap_ != nullptr; }

private:
    void Reset() noexcept;

    HBITMAP bitmap_{};
    HDC memoryDeviceContext_{};
    HGDIOBJ previousBitmap_{};
    SIZE size_{};
};

} // namespace drain_storm
