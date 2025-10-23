#pragma once

#include "Object.h"
#include <memory>
#include <vector>

namespace WW
{
  class Scene
  {
  public:
    Scene();
    ~Scene();

    void AddObject(std::shared_ptr<Object> o);

    std::shared_ptr<Object> CreateObject(const std::string& name = "Object");

    void Render();

  private:
    std::vector<std::shared_ptr<Object>> objects;
  };

} // namespace WW
