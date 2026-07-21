#pragma once
#include "core/Pet.hpp"
#include <ftxui/dom/elements.hpp>
#include <utils/image.hpp>

using namespace ftxui;

Element PetView(Pet &pet, const Image &petImage);
