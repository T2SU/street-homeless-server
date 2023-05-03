#pragma once

#define synchronized(_mutex) for (auto _ul = std::unique_lock<decltype(_mutex)>(_mutex); _ul; _ul.unlock())