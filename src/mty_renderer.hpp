#pragma once
#include <memory>
#include <shader.hpp>
#include "font.hpp"
#include <GLFW/glfw3.h>

#include "mty_state.hpp"
#include <memory>


namespace MintyRenderer {
    class ICellRenderer {
    public:
        virtual ~ICellRenderer() = default;
        virtual void render() = 0;
        virtual void update() = 0;
    };

    class OpenGLCellRenderer : public ICellRenderer {
    private:
        MintyState* state;
        int cols, rows;
        int framebuffer_w, framebuffer_h;
        unsigned int quadVBO, quadVAO;
        unsigned int gridTexID = 0;
        unsigned int glyphMetadataTexID;
        std::unique_ptr<Shader> cellProgram;
        std::unique_ptr<Font> &font;
        void generateGridTexture();
        void updateGridTexture();
    public:
        OpenGLCellRenderer(MintyState* , std::unique_ptr<Font> &);
        ~OpenGLCellRenderer();
        void render() override;
        void update() override;
    };
}
