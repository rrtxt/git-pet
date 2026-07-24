#pragma once
#include "core/Pet.hpp"
#include <ftxui/dom/elements.hpp>
#include <git/Repository.hpp>

ftxui::Element GitCard(Pet &pet, const Repository &repo, int activeView, bool showMenu,
                       int selectedMenuItem);

