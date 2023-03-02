#pragma once

#include <memory>
#include <entt/entt.hpp>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

//These usings are very similiar to the usage of typedef in c
using Entity = entt::entity;
