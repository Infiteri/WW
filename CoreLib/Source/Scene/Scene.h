#pragma once

#include "Object.h"
#include <memory>
#include <vector>

namespace WW
{
  using ObjectList = std::vector<std::shared_ptr<Object>>;

  class Scene
  {
  public:
    Scene();
    ~Scene();

    void AddObject(std::shared_ptr<Object> o);

    std::shared_ptr<Object> CreateObject(const std::string &name = "Object");

    void Render();

    inline ObjectList &GetObjects() { return objects; };

  private:
    ObjectList objects;
  };

} // namespace WW
