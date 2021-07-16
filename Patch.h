#pragma once
class Patch
{
public:
    Patch(winrt::Microsoft::Graphics::Canvas::CanvasDevice device);

    ~Patch();

    void Draw(winrt::Windows::Foundation::Size size);
    winrt::Microsoft::Graphics::Canvas::CanvasSwapChain GetSwapChain();
    void SetColors(float red, float green, float blue);

private:
    void RecreateIfNeeded(winrt::Windows::Foundation::Size size);

private:
    winrt::Microsoft::Graphics::Canvas::CanvasSwapChain m_swapchain;
    winrt::Microsoft::Graphics::Canvas::CanvasDevice m_device;
    //winrt::Microsoft::Graphics::Canvas::Brushes::CanvasSolidColorBrush m_brush;
    winrt::Microsoft::Graphics::Canvas::Effects::ColorSourceEffect m_sourceEffect;
    float m_red = 0.f, m_green = 0.f, m_blue = 0.f;
    float m_width = 0.f, m_height = 0.f;
};

