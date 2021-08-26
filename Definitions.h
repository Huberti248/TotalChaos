#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
//#include <SDL_gpu.h>
//#include <SFML/Network.hpp>
//#include <SFML/Graphics.hpp>
#include <algorithm>
#include <atomic>
#include <codecvt>
#include <functional>
#include <locale>
#include <mutex>
#include <thread>
#ifdef __ANDROID__
#include "vendor/PUGIXML/src/pugixml.hpp"
#include <android/log.h> //__android_log_print(ANDROID_LOG_VERBOSE, "TotalChaos", "Example number log: %d", number);
#include <jni.h>
#else
#include <filesystem>
#include <pugixml.hpp>
#ifdef __EMSCRIPTEN__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif
using namespace std::chrono_literals;
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "MathUtils.h"
#include "Movement.h"
#include "Menu.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Clock.h"

// NOTE: Remember to uncomment it on every release
//#define RELEASE

#if defined _MSC_VER && defined RELEASE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//240 x 240 (smart watch)
//240 x 320 (QVGA)
//360 x 640 (Galaxy S5)
//640 x 480 (480i - Smallest PC monitor)

#define PLAYER_SPEED 0.3
#define BULLET_SPEED 0.5
#define ENEMY_SPEED 0.1
#define PLANET_SPEED 0.1
#define BULLET_SPAWN_DELAY_IN_MS 500
#define ENEMY_SPAWN_DELAY_IN_MS 1000
#define BULLET_SPLIT_DELAY_IN_MS 1050
#define MINIMUM_INTERVAL_BULLET_MS 2000
#define MAXIMUM_INTERVAL_BULLET_MS 7000
#define BULLET_COLLISION_DELAY_IN_MS 100
#define PLANET_SPAWN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MIN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MAX_DELAY_IN_MS 90000
#define PORTAL_SPAWN_DELAY_IN_MS 5000
#define STREAK_BOMB_REQUIREMENT 20
#define BOMB_RADIUS 215


SDL_Window* window;
SDL_Renderer* renderer;
int windowWidth = 1280;
int windowHeight = 720;

SDL_Point mousePos;
SDL_Point realMousePos;
bool keys[SDL_NUM_SCANCODES];
bool buttons[SDL_BUTTON_X2 + 1];
TTF_Font* robotoF;
bool running = true;

