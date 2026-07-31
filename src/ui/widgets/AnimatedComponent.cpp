#include "screen/ui/widgets/AnimatedComponent.hpp"
#include <chrono>

AnimatedComponent::AnimatedComponent(ftxui::Component child, Pet &pet)
    : pet_(pet) {
  Add(child);
}

ftxui::Element AnimatedComponent::OnRender() {
  return ftxui::ComponentBase::OnRender();
}

void AnimatedComponent::OnAnimation(ftxui::animation::Params &params) {
  auto dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      params.duration());
  pet_.animationPlayer().update(dt_ms);
  ftxui::animation::RequestAnimationFrame();
  ftxui::ComponentBase::OnAnimation(params);
}
