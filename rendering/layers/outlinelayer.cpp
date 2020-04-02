#include "outlinelayer.h"

OutlineLayer::OutlineLayer(Camera* camera, Shader* shader)
:AbstractLayer(new BatchOutline(), camera, shader)
{}

OutlineLayer::~OutlineLayer()
{}

