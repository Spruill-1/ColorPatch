#include "pch.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Microsoft::Graphics::Canvas::Brushes;
using namespace winrt::Microsoft::Graphics::Canvas::UI::Xaml;
using namespace winrt::Windows::Graphics::DirectX;

Patch::Patch(
    CanvasDevice device) : 
    m_swapchain(nullptr), m_brush(nullptr)
{
    m_device = device.GetSharedDevice();
}

Patch::~Patch()
{

}

void Patch::RecreateIfNeeded(winrt::Windows::Foundation::Size size)
{
    bool destroy = size.Height <= 0 || size.Width <= 0;
    bool resize = size.Height != m_height || size.Width != m_width;
    bool recreate = !m_swapchain;

    if (destroy)
    {
        m_swapchain = nullptr;
        m_brush = nullptr;

        return;
    }
    else if (recreate)
    {
        m_swapchain = CanvasSwapChain(
            m_device,
            size.Width, size.Height,
            CanvasControl().Dpi(),
            DirectXPixelFormat::R16G16B16A16Float,
            2,
            CanvasAlphaMode::Ignore
        );

        m_height = size.Height;
        m_width = size.Width;
    }
    else if (resize)
    {
        m_swapchain.ResizeBuffers(size.Width, size.Width, CanvasControl().Dpi());

        m_height = size.Height;
        m_width = size.Width;
    }

    if (!m_brush)
    {
        m_brush = CanvasSolidColorBrush::CreateHdr(m_device, { m_red, m_green, m_blue, 1.0f });
    }
}

void Patch::Draw(winrt::Windows::Foundation::Size size)
{
    RecreateIfNeeded(size);

    if (!m_swapchain) return;

    auto ds = m_swapchain.CreateDrawingSession(winrt::Windows::UI::Colors::AliceBlue());

    ds.FillRectangle(0, 0, m_width, m_height, m_brush);

    m_swapchain.Present();
}

winrt::Microsoft::Graphics::Canvas::CanvasSwapChain Patch::GetSwapChain()
{
    return m_swapchain;
}

void Patch::SetColors(float red, float green, float blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;

    if (m_brush) m_brush.ColorHdr({ m_red, m_green, m_blue, 1.0f });

    Draw(Size(m_width, m_height));
}
