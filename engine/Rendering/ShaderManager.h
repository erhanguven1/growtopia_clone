//
// Created by erhan güven on 30.08.2023.
//

#ifndef GROWTOPIA_CLONE_SHADERMANAGER_H
#define GROWTOPIA_CLONE_SHADERMANAGER_H

#include <bgfx/bgfx.h>
#include <unordered_map>
#include <initializer_list>
#include <fstream>
#include <glm/vec2.hpp>

namespace Engine
{
    enum class EngineShaderPrograms
    {
        Default,
    };

    class ShaderProgram
    {
        friend class ShaderManager;
    public:
        inline bgfx::ProgramHandle getProgramHandle(){ return m_programHandle; }
        void setUniform(const char* name, void* value)
        {
            assert(m_uniformHandles.find(name) != m_uniformHandles.end());
            bgfx::setUniform(m_uniformHandles.at(name), value);
        }
        void addUniform(const char* name, const bgfx::UniformType::Enum& uniformType)
        {
            m_uniformHandles[name] = bgfx::createUniform(name,uniformType);
        }
        bgfx::UniformHandle* getUniform(const char* name)
        {
            return &m_uniformHandles[name];
        }
        ~ShaderProgram()
        {
            bgfx::destroy(m_programHandle);
            for(auto uniformHandlePair : m_uniformHandles)
            {
                bgfx::destroy(uniformHandlePair.second);
            }
        }
    protected:
        explicit ShaderProgram(bgfx::ProgramHandle& pHandle) : m_programHandle(pHandle) {}
        bgfx::ProgramHandle m_programHandle;
        std::unordered_map<const char*, bgfx::UniformHandle> m_uniformHandles;
    };

    class ShaderManager
    {
    public:
        ShaderManager()
        {
            instance = this;
        }
        inline static ShaderManager* getInstance(){ return instance; }
        void initShaders(std::initializer_list<EngineShaderPrograms> engineShaderPrograms)
        {
            for(auto i : engineShaderPrograms)
            {
                const std::string shader_directory_path = getShaderDirectoryPath(bgfx::getRendererType());

                std::string shaderFileName = shaderFileNames[i];
                std::string vertexShaderName = "/vs_" + shaderFileName + ".bin";
                std::string fragmentShaderName = "/fs_" + shaderFileName + ".bin";

                auto m_vertex_shader = loadShader(shader_directory_path + vertexShaderName);
                auto m_fragment_shader = loadShader(shader_directory_path + fragmentShaderName);

                auto programHandle = bgfx::createProgram(m_vertex_shader, m_fragment_shader, true);
                ShaderProgram* shaderProgram = nullptr;

                switch (i)
                {
                    case EngineShaderPrograms::Default:
                        shaderProgram = new ShaderProgram(programHandle);
                        shaderProgram->addUniform("a_pos", bgfx::UniformType::Vec4);
                        shaderProgram->addUniform("a_scale", bgfx::UniformType::Vec4);
                        shaderProgram->addUniform("s_tex", bgfx::UniformType::Sampler);
                        shaderProgram->addUniform("a_color", bgfx::UniformType::Vec4);
                        break;
                }

                shaderPrograms[i] = shaderProgram;
            }
        }
        inline ShaderProgram* getProgram(const EngineShaderPrograms& programName){ return shaderPrograms[programName];}
    private:
        inline static ShaderManager* instance;
        std::unordered_map<EngineShaderPrograms, ShaderProgram*> shaderPrograms;
        std::unordered_map<EngineShaderPrograms, const char*> shaderFileNames
        {
            {EngineShaderPrograms::Default,"default"}
        };

        std::string getShaderDirectoryPath(const bgfx::RendererType::Enum renderer_type)
        {
            const std::string render_type_directory_name = [&]()
            {
                switch (renderer_type)
                {
                    case bgfx::RendererType::OpenGL: return "/Users/erhanguven/CLionProjects/growtopia_clone/glsl";
                    default: throw std::runtime_error("Renderer type not supported.");
                }
            }();
            return render_type_directory_name;
        }

        bgfx::ShaderHandle loadShader(const std::string& shader_path)
        {
            std::ifstream input_stream(shader_path, std::ios::binary);
            if (!input_stream.is_open())
            {
                throw std::runtime_error("Failed to open a shader file.");
            }

            input_stream.seekg(0, std::ios::end);
            const long file_size = input_stream.tellg();
            input_stream.seekg(0);
            const bgfx::Memory* mem = bgfx::alloc(file_size + 1);
            input_stream.read(reinterpret_cast<char*>(mem->data), file_size);
            mem->data[mem->size - 1] = '\0';

            return bgfx::createShader(mem);
        }
    };

} // Engine

#endif //GROWTOPIA_CLONE_SHADERMANAGER_H
