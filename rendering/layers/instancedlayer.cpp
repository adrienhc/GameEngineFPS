#include "instancedlayer.h"

InstancedLayer::InstancedLayer(Camera* camera, Shader* shader)
:SceneLayer(new BatchInstanced(), camera, shader)
{}

InstancedLayer::~InstancedLayer()
{}