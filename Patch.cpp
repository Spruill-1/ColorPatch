#include "pch.h"
#include <Microsoft.Graphics.Canvas.native.h>

namespace interop
{
    using namespace ABI::Microsoft::Graphics::Canvas;
}
namespace winrt
{
    using namespace winrt::Windows::UI::Xaml;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::Graphics::Canvas;
    using namespace winrt::Microsoft::Graphics::Canvas::Brushes;
    using namespace winrt::Microsoft::Graphics::Canvas::UI::Xaml;
    using namespace winrt::Windows::Graphics::DirectX;
}

Patch::Patch(
    winrt::CanvasDevice device) : 
    m_swapchain(nullptr), m_sourceEffect(nullptr), m_matrixEffect(nullptr)//, m_brush(nullptr)
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
        //m_brush = nullptr;
        m_sourceEffect = nullptr;
        m_matrixEffect = nullptr;

        return;
    }
    else if (recreate)
    {
        m_swapchain = winrt::CanvasSwapChain(
            m_device.GetSharedDevice(),
            size.Width, size.Height,
            winrt::CanvasControl().Dpi(),
            winrt::DirectXPixelFormat::R16G16B16A16Float,
            2,
            winrt::CanvasAlphaMode::Ignore
        );
        
        // Get a handle to the underlying swapchain resource
        {
            winrt::com_ptr<IDXGISwapChain1> nativeSwapchain = nullptr;
            auto nativeWrapper = m_swapchain.as<interop::ICanvasResourceWrapperNative>();
            winrt::check_hresult(nativeWrapper->GetNativeResource(nullptr, 0, IID_PPV_ARGS(&nativeSwapchain)));
            auto nativeSwapchain3 = nativeSwapchain.as<IDXGISwapChain3>();

            // Check the swapchain's color space support - scRGB is required for this app
            UINT colorSpaceSupport = 0;
            winrt::check_hresult(nativeSwapchain3->CheckColorSpaceSupport(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709, &colorSpaceSupport));
            winrt::check_bool(colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT);
            winrt::check_hresult(nativeSwapchain3->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709));
        }

        m_height = size.Height;
        m_width = size.Width;
    }
    else if (resize)
    {
        m_swapchain.ResizeBuffers(size.Width, size.Width, winrt::CanvasControl().Dpi());

        m_height = size.Height;
        m_width = size.Width;
    }

    /*
    if (!m_brush)
    {
        m_brush = winrt::CanvasSolidColorBrush::CreateHdr(m_device, { m_red, m_green, m_blue, 1.0f });
    }
    */

    if (!m_sourceEffect)
    {
        m_sourceEffect = winrt::Effects::ColorSourceEffect();
        m_sourceEffect.ColorHdr({ 1.f, 1.f, 1.f, 1.f });
    }

    if (!m_matrixEffect)
    {
        m_matrixEffect = winrt::Effects::ColorMatrixEffect();

        winrt::Effects::Matrix5x4 mat{ 0 };
        mat.M11 = m_red;
        mat.M22 = m_green;
        mat.M33 = m_blue;
        mat.M44 = 1.f;
        m_matrixEffect.ColorMatrix(mat);
        m_matrixEffect.ClampOutput(false);

        m_matrixEffect.Source(m_sourceEffect);
    }
}

void Patch::Draw(winrt::Windows::Foundation::Size size)
{
    RecreateIfNeeded(size);

    if (!m_swapchain) return;

    auto ds = m_swapchain.CreateDrawingSession(winrt::Windows::UI::Colors::AliceBlue());

    ds.DrawImage(m_matrixEffect);

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

    //if (m_brush) m_brush.ColorHdr({ m_red, m_green, m_blue, 1.0f });

    if (m_sourceEffect) 
    {
        winrt::Effects::Matrix5x4 mat{ 0 };
        mat.M11 = m_red;
        mat.M22 = m_green;
        mat.M33 = m_blue;
        mat.M44 = 1.f;
        m_matrixEffect.ColorMatrix(mat);
        m_matrixEffect.ClampOutput(false);
    }

    Draw(winrt::Size(m_width, m_height));
}
