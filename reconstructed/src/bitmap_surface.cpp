#include "bitmap_surface.h"

namespace drain_storm {

BitmapSurface::~BitmapSurface() {
    Reset();
}

bool BitmapSurface::Load(const std::filesystem::path& path) {
    Reset();
    bitmap_ = static_cast<HBITMAP>(LoadImageW(
        nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE));
    if (bitmap_ == nullptr) {
        return false;
    }

    BITMAP details{};
    if (GetObjectW(bitmap_, sizeof(details), &details) == 0) {
        Reset();
        return false;
    }
    size_.cx = details.bmWidth;
    size_.cy = details.bmHeight;

    memoryDeviceContext_ = CreateCompatibleDC(nullptr);
    if (memoryDeviceContext_ == nullptr) {
        Reset();
        return false;
    }
    previousBitmap_ = SelectObject(memoryDeviceContext_, bitmap_);
    return previousBitmap_ != nullptr && previousBitmap_ != HGDI_ERROR;
}

void BitmapSurface::DrawCentered(HDC destination, const RECT& bounds) const {
    if (!IsLoaded()) {
        return;
    }
    const int x = bounds.left + ((bounds.right - bounds.left) - size_.cx) / 2;
    const int y = bounds.top + ((bounds.bottom - bounds.top) - size_.cy) / 2;
    BitBlt(destination, x, y, size_.cx, size_.cy, memoryDeviceContext_, 0, 0, SRCCOPY);
}

void BitmapSurface::DrawRegion(HDC destination, int destinationX, int destinationY,
                               int sourceX, int sourceY, int width, int height) const {
    if (!IsLoaded()) {
        return;
    }
    BitBlt(destination, destinationX, destinationY, width, height,
           memoryDeviceContext_, sourceX, sourceY, SRCCOPY);
}

void BitmapSurface::Reset() noexcept {
    if (memoryDeviceContext_ != nullptr && previousBitmap_ != nullptr) {
        SelectObject(memoryDeviceContext_, previousBitmap_);
    }
    if (memoryDeviceContext_ != nullptr) {
        DeleteDC(memoryDeviceContext_);
    }
    if (bitmap_ != nullptr) {
        DeleteObject(bitmap_);
    }
    bitmap_ = nullptr;
    memoryDeviceContext_ = nullptr;
    previousBitmap_ = nullptr;
    size_ = {};
}

} // namespace drain_storm
