#pragma once
#include "core/Pet.hpp"
#include <ftxui/dom/elements.hpp>
#include <git/Repository.hpp>

using namespace ftxui;

Element GitCard(Pet &pet, const Repository &repo);
