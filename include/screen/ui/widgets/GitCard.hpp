#pragma once
#include "core/Pet.hpp"
#include <ftxui/dom/elements.hpp>
#include <git/Repository.hpp>
#include <utils/image.hpp>

using namespace ftxui;

Element GitCard(Pet &pet, const Repository &repo, const Image &petImage, int activeView, bool showMenu, int selectedMenuItem);
