#ifndef ANIMATED_COMPONENT_HPP
#define ANIMATED_COMPONENT_HPP

#include "core/Pet.hpp"
#include <chrono>
#include <ftxui/component/animation.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

class AnimatedComponent : public ftxui::ComponentBase {
public:
  AnimatedComponent(ftxui::Component child, Pet &pet);

  ftxui::Element OnRender() override;
  void OnAnimation(ftxui::animation::Params &params) override;

private:
  Pet &pet_;
};

#endif // ANIMATED_COMPONENT_HPP
