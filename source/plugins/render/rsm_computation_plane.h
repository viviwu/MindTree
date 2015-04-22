#ifndef RSM_COMPUTATION_PLANE_H
#define RSM_COMPUTATION_PLANE_H

#include "light_accumulation_plane.h"
#include "shadow_mapping.h"

namespace MindTree {
namespace GL {

class RSMIndirectPlane : public LightAccumulationPlane
{
public:
    RSMIndirectPlane();

    void setSearchRadius(double radius);
    void setIntensity(double intensity);
    void setSamples(int samples);

protected:
    void init(std::shared_ptr<ShaderProgram> program);
    void drawLight(const LightPtr light, 
                   std::shared_ptr<ShaderProgram> program);

private:
    void initSamplingTexture();

    std::shared_ptr<Texture> _samplingPattern;
    std::atomic<double> _searchRadius;
    std::atomic<double> _intensity;
    std::atomic<int> _numSamples;

    std::atomic<bool> _samplesChanged;
};

class RSMGenerationBlock : public ShadowMappingRenderBlock
{
public:
    RSMGenerationBlock();

protected:
    virtual std::weak_ptr<RenderPass> createShadowPass(std::shared_ptr<SpotLight> spot);

private:
};

class RSMEvaluationBlock : public RenderBlock
{
public:
    RSMEvaluationBlock(RSMGenerationBlock *shadowBlock);
    void init();

    void setCamera(std::shared_ptr<Camera> cam);

    void setGeometry(std::shared_ptr<Group> grp);

private:
    RSMIndirectPlane *_rsmIndirectHighResPlane;
    RSMIndirectPlane *_rsmIndirectLowResPlane;

    std::weak_ptr<RenderPass> _rsmIndirectPass;
    std::weak_ptr<RenderPass> _rsmIndirectLowResPass;
    std::weak_ptr<RenderPass> _rsmInterpolatePass;

    std::atomic<int> _downSampling;

    RSMGenerationBlock *_shadowBlock;
};

}
}
#endif
